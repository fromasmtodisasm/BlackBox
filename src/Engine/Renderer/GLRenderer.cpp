
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>

#include <BlackBox/Renderer/Render.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IWindow.hpp>

#include <BlackBox/Renderer/VertexBuffer.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <SDL2/SDL.h>

#include <sstream>

#pragma warning(push)
#pragma warning(disable : 4244)

static const int SIZEOF_INDEX = sizeof(short);

namespace
{
	enum AttributeLocation : GLuint {
		position = 0,
		normal = 1,
		uv = 2,
		tangent = 3,
		btangent = 4,
		color = 5
	};
	struct VertexAttributePointer
	{
		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const void* pointer;

		VertexAttributePointer(
			GLuint index,
			GLint size,
			GLenum type,
			GLboolean normalized,
			GLsizei stride,
			const void* pointer
		)
			:
			index(index),
			size(size),
			type(type),
			normalized(normalized),
			stride(stride),
			pointer(pointer)
		{}

	};

	inline VertexAttributePointer GetPositionAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			position, 3, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat],	reinterpret_cast<GLvoid*>(0)
		);
	}
	inline VertexAttributePointer GetNormalAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			normal, 3, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat], reinterpret_cast<GLvoid*>(gBufInfoTable[vertexFormat].OffsNormal)
		);
	}
	inline VertexAttributePointer GetColorAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			color, 4, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat],	reinterpret_cast<GLvoid*>(gBufInfoTable[vertexFormat].OffsColor)
		);
	}
	inline VertexAttributePointer GetUVAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			uv, 2, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat],	reinterpret_cast<GLvoid*>(gBufInfoTable[vertexFormat].OffsTC)
		);
	}

	void SetAttribPointer(VertexAttributePointer vap)
	{
		gl::EnableVertexAttribArray(vap.index);
		gl::VertexAttribPointer(
				vap.index, vap.size, vap.type, vap.normalized, vap.stride, vap.pointer 
				);
	}
	void DisableAttributes(CVertexBuffer* vb)
	{
		SVertBufComps vbc;
		GetVertBufComps(&vbc, vb->m_vertexformat);

		gl::DisableVertexAttribArray(position);
		if (vbc.m_bHasNormals)
		{
			gl::DisableVertexAttribArray(normal);
		}
		if (vbc.m_bHasTC)
		{
			gl::DisableVertexAttribArray(uv);
		}
		if (vbc.m_bHasColors)
		{
			gl::DisableVertexAttribArray(color);
		}

	}
	void EnableAttributes(CVertexBuffer* vb)
	{
		SVertBufComps vbc;
		auto& vf = vb->m_vertexformat;
		GetVertBufComps(&vbc, vf);

		SetAttribPointer(GetPositionAttributePointer(vf));
		if (vbc.m_bHasNormals)
		{
			SetAttribPointer(GetNormalAttributePointer(vf));
		}
		if (vbc.m_bHasTC)
		{
			SetAttribPointer(GetUVAttributePointer(vf));
		}
		if (vbc.m_bHasColors)
		{
			SetAttribPointer(GetColorAttributePointer(vf));
		}
	}

	GLenum toGlPrimitive(RenderPrimitive rp)
	{
		switch (rp)
		{
		case RenderPrimitive::LINES: return GL_LINES;
		case RenderPrimitive::LINE_STRIP: return GL_LINE_STRIP;
		case RenderPrimitive::TRIANGLES: return GL_TRIANGLES;
		case RenderPrimitive::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		default: return -1;
		}
	}

}

GLRenderer::GLRenderer(ISystem* engine) :
  m_pSystem(engine), m_viewPort(0, 0, 0, 0)
{
}

GLRenderer::~GLRenderer()
{
}

IWindow* GLRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
  IWindow* result = m_Window = window;
  if (window == nullptr)
    return nullptr;
  //=======================
  InitConsoleVariables();
  InitConsoleCommands();
  //=======================
  if (isDebug && r_debug->GetIVal() == 1)
    glContextType = AttributeType::DEBUG;
  else
    glContextType = AttributeType::CORE;
  //TODO: FIX THIS
#if 0
  sf::ContextSettings settings(zbpp, sbits, antialiassing, majorVersion, minorVersion, glContextType);
  if (!m_Window->create(reinterpret_cast<void*>(&settings)))
    return nullptr;
#endif
  if (!m_Window->init(x, y, width, height, cbpp, zbpp, sbits, fullscreen))
    return nullptr;
  if (!OpenGLLoader())
    return nullptr;
  context = SDL_GL_GetCurrentContext();
  m_HWND = (HWND)window->getHandle();
  // now you can make GL calls.
  gl::ClearColor({0, 0, 0, 0});
  gl::Clear(GL_COLOR_BUFFER_BIT);
  m_Window->swap();
  //=======================
  m_pSystem->GetIConsole()->AddConsoleVarSink(this);
  m_pSystem->GetIInput()->AddEventListener(this);
  //=======================
  glInit();
  printHardware();
  m_ScreenQuad = new Quad();
  //=======================
  ProgramDesc pd = {
    "screen_quad",
    ShaderDesc("screenshader.vs"),
    ShaderDesc("screenshader.frag")
  };
  //pd.vs.macro["STORE_TEXCOORDS"] = "1";
	if (!MaterialManager::instance()->loadProgram(pd, false))
	{
    m_pSystem->Log("Error of loading screen shader");
    return nullptr;
	}
  m_ScreenShader = MaterialManager::instance()->getProgram(pd.name);
  if (!m_ScreenShader)
  {
    m_pSystem->Log("Error of creating screen shader");
    return nullptr;
  }
  //m_ScreenShader->Create();
  m_ScreenShader->Use();
  m_ScreenShader->Uniform(0, "screenTexture");
  m_ScreenShader->Unuse();

  //cam_width->Set(GetWidth());
  //cam_height->Set(GetHeight());
  return result;
}

bool GLRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
  return false;
}

void GLRenderer::Release()
{
  delete this;
}

void GLRenderer::BeginFrame(void)
{
  gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::Update(void)
{
	m_FrameID++;
}

void GLRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
}

void GLRenderer::SetViewport(int x, int y, int width, int height)
{
  gl::ViewPort(glm::vec4(x, y, width, height));
}

void GLRenderer::SetScissor(int x, int y, int width, int height)
{
}

void GLRenderer::SetCamera(const CCamera& cam)
{
	m_Camera = cam;
}

const CCamera& GLRenderer::GetCamera()
{
  return m_Camera;
}

bool GLRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
  return false;
}

void GLRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void GLRenderer::Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info)
{
  info.font->RenderText(szText, posX, posY, 1.0, info.color);
}

int GLRenderer::SetPolygonMode(int mode)
{
  return 0;
}

int GLRenderer::GetWidth()
{
  return m_Window->getWidth();
}

int GLRenderer::GetHeight()
{
  return m_Window->getHeight();
}

void GLRenderer::ScreenShot(const char* filename)
{
  GLint drawFboId = 0;// , readFboId = 0;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);

  gl::BindFramebuffer(0);
  GLint dims[4] = { 0 };
  glGetIntegerv(GL_VIEWPORT, dims);
  GLint width = dims[2];
  GLint height = dims[3];

  uint8_t* pixels = new uint8_t[3 * width * height];
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

  stbi_flip_vertically_on_write(1);
  stbi_write_png(filename, width, height, 3, pixels, width * 3);

  //stbi_image_free(img);
  gl::BindFramebuffer(drawFboId);
}

void GLRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

void GLRenderer::glInit()
{
  CBaseShaderProgram::use_cache = GetISystem()->GetIConsole()->GetCVar("sh_use_cache");
  fillSates();
  if (glContextType == AttributeType::DEBUG || r_debug->GetIVal() == 1)
  {
		m_pSystem->Log("Create debug render context");
    glDebug = std::make_shared<OpenglDebuger>("out/glDebug.txt");
    SetState(State::DEBUG_OUTPUT, true);
    SetState(State::DEBUG_OUTPUT_SYNCHRONOUS, true);
  }
  SetState(State::POLYGON_OFFSET_FILL, true);
  SetState(State::PROGRAM_POINT_SIZE , true);
  glLineWidth(1);
  SetState(State::DEPTH_TEST, true);
  SetState(State::BLEND, true);
  SetState(State::CULL_FACE, true);
  SetCullMode(CullMode::BACK);

  m_Hardware.vendor = 			gl::GetString(GL_VENDOR);
  m_Hardware.render = 			gl::GetString(GL_RENDERER);
  m_Hardware.version = 			gl::GetString(GL_VERSION);
  m_Hardware.glsl_version = gl::GetString(GL_SHADING_LANGUAGE_VERSION);
}

void GLRenderer::fillSates()
{
#define STATEMAP(k,v) stateMap.insert(std::make_pair(k,v))
  STATEMAP(State::DEPTH_TEST, GL_DEPTH_TEST);
  STATEMAP(State::CULL_FACE, GL_CULL_FACE);
  STATEMAP(State::BLEND, GL_BLEND);
  STATEMAP(State::DEBUG_OUTPUT, GL_DEBUG_OUTPUT);
  STATEMAP(State::DEBUG_OUTPUT_SYNCHRONOUS, GL_DEBUG_OUTPUT_SYNCHRONOUS);
  STATEMAP(State::POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_FILL);
  STATEMAP(State::FRAMEBUFFER_SRGB, GL_FRAMEBUFFER_SRGB);
  STATEMAP(State::SCISSOR_TEST, GL_SCISSOR_TEST);
  STATEMAP(State::STENCIL_TEST, GL_STENCIL_TEST);
  STATEMAP(State::PROGRAM_POINT_SIZE, GL_PROGRAM_POINT_SIZE);

#undef STATEMAP
}

void GLRenderer::InitConsoleVariables()
{
  translateImageY = CREATE_CVAR("ty", 0.0f, 0);
  translateImageX = CREATE_CVAR("tx", 0.0f, 0);
  scaleImageX = CREATE_CVAR("sx", 1.0f, 0);
  scaleImageY = CREATE_CVAR("sy", 1.0f, 0);
  needTranslate = CREATE_CVAR("nt", 1, 0, "Translate or not 2d background of console");
  needFlipY = CREATE_CVAR("nfy", 1, 0, "Flip or not 2d background of console");
  test_proj = CREATE_CVAR("test_proj", "test proj empty", 0);
  render_via_viewport = CREATE_CVAR("rvv", 0, 0, "Rendering use view port, if 1 else with projection matrix");

  r_debug = GET_CVAR("r_debug");
  cam_width = GET_CVAR("r_cam_w");
  cam_height = GET_CVAR("r_cam_h");
}

void GLRenderer::InitConsoleCommands()
{
  /*
  REGISTER_COMMAND(
    "set_ws",
    R"(set2dvec("r_cam_w", "r_cam_h", %1, %2))",
    VF_NULL,
    "Set size of camera"
  );
  */
}

CVertexBuffer* GLRenderer::CreateBuffer(int vertexcount, int vertexformat, const char* szSource, bool bDynamic/* = false*/)
{
	assert(vertexformat >= VERTEX_FORMAT_P3F && vertexformat < VERTEX_FORMAT_NUMS);
	SVertexStream stream;
	CVertexBuffer *buffer = new CVertexBuffer;
	stream.m_bDynamic = bDynamic;
	stream.m_VData = CreateVertexBuffer(vertexformat, vertexcount);

	gl::GenVertexArrays(1, &buffer->m_Container);
	gl::BindVertexArray(buffer->m_Container);
    {
		gl::GenBuffer(&stream.m_VertBuf.m_nID);

		gl::BindBuffer(GL_ARRAY_BUFFER, stream.m_VertBuf.m_nID);
		gl::BufferData(GL_ARRAY_BUFFER, vertexcount * gVertexSize[vertexformat], nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		buffer->m_bDynamic = bDynamic;
		buffer->m_NumVerts = vertexcount;
		buffer->m_vertexformat = vertexformat;
		buffer->m_VS[VSF_GENERAL] = stream;
		m_VertexBufferPool.push_back({ false, buffer });
		EnableAttributes(buffer);
    }
	gl::BindVertexArray(0);
	debuger::vertex_array_label(stream.m_VertBuf.m_nID, szSource);
	return buffer;
}

void GLRenderer::ReleaseBuffer(CVertexBuffer* bufptr)
{
}

void GLRenderer::DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start/* = 0*/, int vert_stop/* = 0*/, CMatInfo* mi/* = NULL*/)
{
	assert(glIsVertexArray(src->m_Container));
  assert(src != nullptr);
	auto to_draw = vert_stop - vert_start;
	gl::BindVertexArray(src->m_Container);

  auto gl_mode = toGlPrimitive(static_cast<RenderPrimitive>(prmode));
  if (indicies != nullptr)
  {
    assert(numindices != 0);
    gl::DrawElements(gl_mode, numindices, GL_UNSIGNED_SHORT, reinterpret_cast<GLushort*>(offsindex));
  }
  else
  {
    gl::DrawArrays(gl_mode, 0, to_draw == 0 ? src->m_NumVerts : to_draw);
  }
	gl::BindVertexArray(0);
}

void GLRenderer::UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs/* = 0*/, int Type/* = 0*/)
{
	gl::BindBuffer(GL_ARRAY_BUFFER, dest->m_VS[VSF_GENERAL].m_VertBuf.m_nID);
	gl::BufferSubData(GL_ARRAY_BUFFER, nOffs, vertexcount * gVertexSize[dest->m_vertexformat], src);
	gl::BindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLRenderer::CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount)
{
	assert(dest != nullptr);
	assert(src != nullptr);

	SVertexStream stream;
	stream.m_bDynamic = false;
	stream.m_nBufOffset = 0;
	stream.m_nItems = indexcount;

	gl::GenBuffer(&stream.m_VertBuf.m_nID);
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, stream.m_VertBuf.m_nID);
	gl::BufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(short), src, GL_STATIC_DRAW);
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void GLRenderer::UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock/* = true*/)
{
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, dest->m_VertBuf.m_nID);
	gl::BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexcount * SIZEOF_INDEX, src);
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLRenderer::ReleaseIndexBuffer(SVertexStream* dest)
{
}

void GLRenderer::ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz)
{
}

int GLRenderer::UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4])
{
	return 0;
}

int GLRenderer::UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz)
{
	return 0;
}

void GLRenderer::GetModelViewMatrix(float* mat)
{
}

void GLRenderer::GetModelViewMatrix(double* mat)
{
}

void GLRenderer::GetProjectionMatrix(double* mat)
{
}

void GLRenderer::GetProjectionMatrix(float* mat)
{
}

Vec3 GLRenderer::GetUnProject(const Vec3& WindowCoords, const CCamera& cam)
{
	return Vec3();
}

int GLRenderer::GetFrameID(bool bIncludeRecursiveCalls/* = true*/)
{
	return m_FrameID;
}

void GLRenderer::printHardware()
{
  std::stringstream hardware_info;
  hardware_info << "Hardware render info\n" <<
                   "Vendor: [" << m_Hardware.vendor << "]\n"
                   "Render: [" << m_Hardware.render << "]\n"
                   "Version: [" << m_Hardware.version << "]\n"
                   "Shader Language Version: [" << m_Hardware.glsl_version << "]\n";
  m_pSystem->Log(hardware_info.str().c_str());
}

void GLRenderer::SetRenderTarget(int nHandle)
{
  if (m_CurrentTarget != nHandle)
    gl::BindFramebuffer(nHandle);
}

IShader* GLRenderer::Sh_Load(ShaderDesc const& desc)
{
  return dynamic_cast<IShader*>(CShader::load(desc));
}

void GLRenderer::DrawFullscreenQuad()
{
  m_ScreenQuad->draw();
}

void GLRenderer::SetCullMode(CullMode mode/* = CullMode::BACK*/)
{
  if (mode == CullMode::FRONT_AND_BACK)
    gl::CullFace(GL_FRONT_AND_BACK);
  else
    gl::CullFace(GL_FRONT + static_cast<unsigned int>(mode));
}

bool GLRenderer::DeleteContext(WIN_HWND hWnd)
{
  return false;
}

bool GLRenderer::CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX/* = 1*/, int SSY/* = 1*/)
{
  return false;
}

bool GLRenderer::SetCurrentContext(WIN_HWND hWnd)
{
  return false;
}

void GLRenderer::MakeMainContextActive()
{
}

WIN_HWND GLRenderer::GetCurrentContextHWND()
{
  return m_HWND;
}

bool GLRenderer::IsCurrentContextMainVP()
{
  return false;
}

int GLRenderer::GetCurrentContextViewportHeight() const
{
  return 0;
}

int GLRenderer::GetCurrentContextViewportWidth() const
{
  return 0;
}

void GLRenderer::SetClearColor(const Vec3& vColor)
{
  m_clearColor = vColor;
  gl::ClearColor(Vec4(vColor, 1));
}

void GLRenderer::ClearDepthBuffer()
{
  //glCheck(glClearBufferfv(GL_DEPTH, 0, &m_clearDepth));

  glCheck(glClearDepthf(1.f));
  gl::Clear(GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::ClearColorBuffer(const Vec3 vColor)
{
  glCheck(glClearBufferfv(GL_COLOR, 0, &glm::vec4(vColor, 1)[0]));
}

void GLRenderer::SetState(State state, bool enable)
{
  auto it = stateMap.find(state);
  if (it != stateMap.end())
  {
    if (enable)
      gl::Enable(it->second);
    else
      gl::Disable(it->second);
  }
}

void GLRenderer::DrawFullScreenImage(int texture_id)
{
  auto
    width = GetWidth(),
    height = GetHeight();
  gl::BindFramebuffer(0);
  SetViewport(0, 0, width, height);
  m_ScreenShader->Use();
  auto proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
  auto transform = glm::scale(proj, glm::vec3(width, height, 1));
  m_ScreenShader->Uniform(transform, "transform");
  SetState(State::DEPTH_TEST, false);
  m_ScreenShader->BindTextureUnit2D(texture_id, 0);
  m_ScreenQuad->draw();
}

bool GLRenderer::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
  if (!strcmp(pVar->GetName(), "r_Width"))
  {
    m_Window->changeSize(pVar->GetFVal(), GetHeight());
  }
  else if (!strcmp(pVar->GetName(), "r_Height"))
  {
    m_Window->changeSize(GetWidth(), pVar->GetFVal());
  }
  if (!strcmp(pVar->GetName(), "r_cam_w"))
  {
    printf("");
  }
  else if (!strcmp(pVar->GetName(), "r_debug"))
  {
    //OpenglDebuger::SetIgnore(!r_debug->GetIVal());
    OpenglDebuger::SetIgnore(!(bool)std::stoi(sNewValue));
  }
  return false;
}

void GLRenderer::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

void GLRenderer::DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)
{
  bool flipY = true;
  float
    width = GetWidth(),
    height = GetHeight();
  //gl::BindFramebuffer(0);
  SetState(State::BLEND, true);
  SetState(IRenderer::State::CULL_FACE, false);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_ScreenShader->Use();
  m_ScreenShader->Uniform(a, "alpha");

  glm::mat4 model(1.0);
  auto uv_projection = glm::mat4(1.0);
  glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1000.0f);

  model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.f));
  model = glm::scale(model, { w, h, 1.f });

  if (needFlipY->GetIVal() == 1)
  {
    uv_projection = glm::scale(glm::mat4(1.0), glm::vec3(1.0f, -1.0f, 1.0f));
  }
  else
  {
    uv_projection = glm::scale(glm::mat4(1.0), glm::vec3(1.f, 1.f, 1.0f));
  }
  uv_projection = glm::translate(uv_projection, glm::vec3(s0, 0, 0.f));
  if (render_via_viewport->GetIVal() == 0)
  {
    m_ScreenShader->Uniform(projection, "projection");
    m_ScreenShader->Uniform(uv_projection, "uv_projection");
    m_ScreenShader->Uniform(model, "model");
  }
  else
  {
    SetViewport(xpos, GetHeight() - h, xpos + w, GetHeight() - ypos - h);
  }

  SetState(State::DEPTH_TEST, false);
  m_ScreenShader->BindTextureUnit2D(texture_id, 0);
  m_ScreenQuad->draw();;
  SetState(State::CULL_FACE, true);
}

void GLRenderer::PrintLine(const char* szText, SDrawTextInfo& info)
{
  Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
}

bool GLRenderer::OnInputEvent(const SInputEvent& event)
{
#if 0
  switch (event.deviceType)
  {
    break;
  default:
    break;
  }
#endif
  return false;
}

void GLRenderer::Set2DMode(bool enable, int ortox, int ortoy)
{
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
  return new GLRenderer(pSystem);
}

