#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/IRender.hpp>

#include "imgui.h"
#include "ImGuiRenderer.hpp"
#include <stdio.h>

// Desktop GL has glDrawElementsBaseVertex() which GL ES and WebGL don't have.
#if defined(IMGUI_IMPL_OPENGL_ES2) || defined(IMGUI_IMPL_OPENGL_ES3)
#define IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX     0
#else
#define IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX     1
#endif

namespace {
  // OpenGL Data
  static char         g_GlslVersionString[32] = "";
  static uint         g_FontTexture = 0;
  ShaderProgramRef    g_Shader;
  static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;
  enum AtribLoc : GLuint
  {
    Position,
    UV,
    Color,
  };

  class BackupGlState
  {
  public:
    BackupGlState()
    {
      // Backup GL state
      glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
      glActiveTexture(GL_TEXTURE0);
      glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  #ifdef GL_SAMPLER_BINDING
      glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
  #endif
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  #ifndef IMGUI_IMPL_OPENGL_ES2
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);
  #endif
  #ifdef GL_POLYGON_MODE
      glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
  #endif
      glGetIntegerv(GL_VIEWPORT, last_viewport);
      glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
      glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
      glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
      glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
      glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
      glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
      glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
      last_enable_blend = glIsEnabled(GL_BLEND);
      last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
      last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
      last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
  #if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__) && 0
      glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&last_clip_origin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
      if (last_clip_origin == GL_UPPER_LEFT)
          clip_origin_lower_left = false;
  #endif


    }
    ~BackupGlState()
    {
      // Restore modified GL state
      glUseProgram(last_program);
      glBindTexture(GL_TEXTURE_2D, last_texture);
  #ifdef GL_SAMPLER_BINDING
      glBindSampler(0, last_sampler);
  #endif
      glActiveTexture(last_active_texture);
  #ifndef IMGUI_IMPL_OPENGL_ES2
      glBindVertexArray(last_vertex_array_object);
  #endif
      glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
      glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
      glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
      if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
      if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
      if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
      if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
  #ifdef GL_POLYGON_MODE
      glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
  #endif
      glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
      glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);

    }

  private:
    GLenum last_active_texture; 
    GLint last_program; 
    GLint last_texture; 
    GLint last_sampler; 
    GLint last_array_buffer; 
    GLint last_vertex_array_object; 
    GLint last_polygon_mode[2]; 
    GLint last_viewport[4];
    GLint last_scissor_box[4];
    GLenum last_blend_dst_rgb;
    GLenum last_blend_src_rgb;
    GLenum last_blend_src_alpha;
    GLenum last_blend_dst_alpha;
    GLenum last_blend_equation_rgb;
    GLenum last_blend_equation_alpha;
    GLboolean last_enable_blend;
    GLboolean last_enable_cull_face;
    GLboolean last_enable_depth_test;
    GLboolean last_enable_scissor_test;
    //bool clip_origin_lower_left = true;
    //GLenum last_clip_origin = 0;
  };
}

// Functions
bool    ImGuiOpenglRender::Init(IRenderer *pRenderer, const char* glsl_version)
{
    m_pRender = pRenderer;
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_opengl3";
#if IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
#endif

    // Dummy construct to make it easily visible in the IDE and debugger which GL loader has been selected. 
    // The code actually never uses the 'gl_loader' variable! It is only here so you can read it!
    // If auto-detection fails or doesn't select the same GL loader file as used by your application, 
    // you are likely to get a crash below.
    // You can explicitly select a loader by using '#define IMGUI_IMPL_OPENGL_LOADER_XXX' in imconfig.h or compiler command-line.
    const char* gl_loader = "Unknown";
    IM_UNUSED(gl_loader);
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    gl_loader = "GL3W";
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    gl_loader = "GLEW";
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    gl_loader = "GLAD";
#elseif IMGUI_IMPL_OPENGL_LOADER_CUSTOM
    gl_loader = "Custom";
#endif

    if (!gladLoadGL())
      return false;

    // Make a dummy GL call (we don't actually need the result)
    // IF YOU GET A CRASH HERE: it probably means that you haven't initialized the OpenGL function loader used by this code.
    // Desktop OpenGL 3/4 need a function loader. See the IMGUI_IMPL_OPENGL_LOADER_xxx explanation above.
    GLint current_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture);

    return true;
}

void ImGuiOpenglRender::Shutdown()
{
    DestroyDeviceObjects();
}

void ImGuiOpenglRender::NewFrame()
{
    if (!g_Shader)
        CreateDeviceObjects();
}
// OpenGL3 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.
void    ImGuiOpenglRender::RenderDrawData(ImDrawData* draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    BackupGlState scopedBackup;

    // Setup desired GL state
    // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to. VAO are not shared among GL contexts)
    // The renderer would actually work without any VAO bound, but then our VertexAttrib calls would overwrite the default one currently bound.
    GLuint vertex_array_object = 0;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glGenVertexArrays(1, &vertex_array_object);
#endif
    SetupRenderState(draw_data, fb_width, fb_height, vertex_array_object);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        // Upload vertex/index buffers
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    SetupRenderState(draw_data, fb_width, fb_height, vertex_array_object);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
                    // Apply scissor/clipping rectangle
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__) && 0
                        glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
#else
                        glScissor((int)clip_rect.x, (int)clip_rect.y, (int)clip_rect.z, (int)clip_rect.w); // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)
#endif
                    // Bind texture, Draw
                    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
#if IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX
                    glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)), (GLint)pcmd->VtxOffset);
#else
                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
#endif
                }
            }
        }
    }

    // Destroy the temporary VAO
#ifndef IMGUI_IMPL_OPENGL_ES2
    glDeleteVertexArrays(1, &vertex_array_object);
#endif

}

void ImGuiOpenglRender::SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height, uint vertex_array_object)
{
  // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
  m_pRender->SetState(IRenderer::State::BLEND, true);
  //glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_pRender->SetState(IRenderer::State::CULL_FACE, false);
  //glDisable(GL_CULL_FACE);
  m_pRender->SetState(IRenderer::State::DEPTH_TEST, false);
  //glDisable(GL_DEPTH_TEST);
  m_pRender->SetState(IRenderer::State::SCISSOR_TEST, false);
  //glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

  // Setup viewport, orthographic projection matrix
  // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
  m_pRender->SetViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  //glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  float L = draw_data->DisplayPos.x;
  float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
  float T = draw_data->DisplayPos.y;
  float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
  glm::mat4 ortho_projection = glm::ortho(L, R, B, T);
  //glUseProgram(g_ShaderHandle);
  g_Shader->Use();
  g_Shader->Uniform(0, "Texture");
  //glUniform1i(g_AttribLocationTex, 0);
  g_Shader->Uniform(ortho_projection, "ProjMtx");
  //glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
#ifdef GL_SAMPLER_BINDING
  //glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif

  (void)vertex_array_object;
#ifndef IMGUI_IMPL_OPENGL_ES2
  glBindVertexArray(vertex_array_object);
#endif

  // Bind vertex/index buffers and setup attributes for ImDrawVert
  glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
  glEnableVertexAttribArray(AtribLoc::Position);
  glEnableVertexAttribArray(AtribLoc::UV);
  glEnableVertexAttribArray(AtribLoc::Color);
  glVertexAttribPointer(AtribLoc::Position, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(AtribLoc::UV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(AtribLoc::Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
}

bool ImGuiOpenglRender::CreateFontsTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}

void ImGuiOpenglRender::DestroyFontsTexture()
{
    if (g_FontTexture)
    {
        ImGuiIO& io = ImGui::GetIO();
        glDeleteTextures(1, &g_FontTexture);
        io.Fonts->TexID = 0;
        g_FontTexture = 0;
    }
}

bool    ImGuiOpenglRender::CreateDeviceObjects()
{
    // Backup GL state
    GLint last_texture, last_array_buffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    GLint last_vertex_array;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
#endif

    // Create shaders
	g_Shader = gEnv->pRenderer->Sh_Load("gui.vs", "gui.frag");

    //g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
    //g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");

    // Create buffers
    glGenBuffers(1, &g_VboHandle);
    glGenBuffers(1, &g_ElementsHandle);

    CreateFontsTexture();

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(last_vertex_array);
#endif

    return true;
}

void    ImGuiOpenglRender::DestroyDeviceObjects()
{
    if (g_VboHandle)        { glDeleteBuffers(1, &g_VboHandle); g_VboHandle = 0; }
    if (g_ElementsHandle)   { glDeleteBuffers(1, &g_ElementsHandle); g_ElementsHandle = 0; }
    #if 0
    if (g_ShaderHandle && g_VertHandle) { glDetachShader(g_ShaderHandle, g_VertHandle); }
    if (g_ShaderHandle && g_FragHandle) { glDetachShader(g_ShaderHandle, g_FragHandle); }
    if (g_VertHandle)       { glDeleteShader(g_VertHandle); g_VertHandle = 0; }
    if (g_FragHandle)       { glDeleteShader(g_FragHandle); g_FragHandle = 0; }
    if (g_ShaderHandle)     { glDeleteProgram(g_ShaderHandle); g_ShaderHandle = 0; }
    #endif

    DestroyFontsTexture();
}
