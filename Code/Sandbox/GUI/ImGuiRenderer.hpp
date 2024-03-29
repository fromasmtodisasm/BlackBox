#if 0
// dear imgui: Renderer for modern OpenGL with shaders / programmatic pipeline
// - Desktop GL: 2.x 3.x 4.x
// - Embedded GL: ES 2.0 (WebGL 1.0), ES 3.0 (WebGL 2.0)
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID in imgui.cpp.
//  [x] Renderer: Desktop GL only: Support for large meshes (64k+ vertices) with 16-bits indices.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.

// About GLSL version:
//  The 'glsl_version' initialization parameter should be NULL (default) or a "#version XXX" string.
//  On computer platform the GLSL version default to "#version 130". On OpenGL ES 3 platform it defaults to "#version 300 es"
//  Only override if your GL version doesn't handle this GLSL version. See GLSL version table at the top of imgui_impl_opengl3.cpp.

#pragma once
struct IRenderer;

class IMGUI_IMPL_API ImGuiOpenglRender
{
public:
// Backend API
  bool Init(IRenderer *pRenderer, const char* glsl_version = NULL);
  void Shutdown();
  void NewFrame();
  static void RenderDrawData(ImDrawData* draw_data);
  static void SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height, uint vertex_array_object);

  static void InitPlatformInterface();
  static void RenderWindow(ImGuiViewport* viewport, void*);

  // (Optional) Called by Init/NewFrame/Shutdown
  bool CreateFontsTexture();
  void DestroyFontsTexture();
  bool CreateDeviceObjects();
  void DestroyDeviceObjects();
private:
  static IRenderer* m_pRender;

};

using ImGuiRender = ImGuiOpenglRender;

#endif
