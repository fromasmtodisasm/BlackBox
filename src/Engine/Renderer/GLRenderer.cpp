#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>

#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/BaseShader.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/Renderer/Render.hpp>
#include <BlackBox/Renderer/ShaderManager.hpp>
#include <BlackBox/Renderer/TechniqueManager.hpp>
#include <BlackBox/Renderer/TextureManager.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IWindow.hpp>

#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/VertexBuffer.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#include <BlackBox/World/World.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <SDL2/SDL.h>

#include <sstream>

#pragma warning(push)
#pragma warning(disable : 4244)

class ShaderMan
{
  public:
	IShaderProgram* Sh_Load(const char* vertex, const char* fragment)
	{
		using ShaderInfo = IShaderProgram::ShaderInfo;
		auto vs			 = CShader::load(ShaderDesc(vertex, IShader::E_VERTEX));
		auto fs			 = CShader::load(ShaderDesc(fragment, IShader::E_FRAGMENT));
		auto p			 = new CShaderProgram(ShaderInfo(vs, std::string(vertex)), ShaderInfo(fs, std::string(fragment)));
		p->Create((std::string(vertex) + std::string(fragment)).data());
		m_Shaders.push_back(p);
		return p;
	}
	void ReloadAll()
	{
		for (auto &s : m_Shaders)
		{
			s->Reload();
		}
	}

  std::vector<_smart_ptr<CShaderProgram>> m_Shaders;
};

ShaderMan* gShMan = nullptr;

GLRenderer::GLRenderer(ISystem* engine)
	: m_pSystem(engine), m_viewPort(0, 0, 0, 0)
{
}

GLRenderer::~GLRenderer()
{
}

IWindow* GLRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	InitCVars();
	//m_Camera.InitCVars();
	IWindow* result = m_Window = window;
	if (window == nullptr)
		return nullptr;
	//=======================
	InitConsoleCommands();
	//=======================
	if (isDebug && r_debug->GetIVal() == 1)
		glContextType = AttributeType::DEBUG;
	else
		glContextType = AttributeType::CORE;
	if (!m_Window->init(x, y, width, height, cbpp, zbpp, sbits, fullscreen))
		return nullptr;
	if (!OpenGLLoader())
		return nullptr;
	context = SDL_GL_GetCurrentContext();
	m_HWND  = (HWND)window->getHandle();
	// now you can make GL calls.
	gl::ClearColor({m_clearColor, 1});
	gl::Clear(GL_COLOR_BUFFER_BIT);
	m_Window->swap();
	//=======================
	m_pSystem->GetIConsole()->AddConsoleVarSink(this);
	m_pSystem->GetIInput()->AddEventListener(this);
	//=======================
	glInit();
	if (!InitResourceManagers())
		return false;

	printHardware();
	m_BufferManager = new CBufferManager();
	CreateQuad();
	gShMan			= new ShaderMan;
	//=======================
	//pd.vs.macro["STORE_TEXCOORDS"] = "1";
	if (!(m_ScreenShader = gEnv->pRenderer->Sh_Load("screenshader.vs", "screenshader.frag")))
	{
		m_pSystem->Log("Error of loading screen shader");
		return nullptr;
	}
	m_ScreenShader->Use();
	m_ScreenShader->Uniform(0, "screenTexture");
	m_ScreenShader->Unuse();

	if (!(m_AuxGeomShader = gEnv->pRenderer->Sh_Load("auxgeom.vs", "auxgeom.frag")))
	{
		m_pSystem->Log("Error of loading auxgeom shader");
	}
	m_RenderAuxGeom = new CRenderAuxGeom();

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
	{
		//std::cout << " aux " << std::endl;
		DEBUG_GROUP("AUX");
		m_AuxGeomShader->Use();
		m_AuxGeomShader->Uniform(m_Camera.getProjectionMatrix(), "projection");
		m_AuxGeomShader->Uniform(m_Camera.GetViewMatrix(), "view");
		m_RenderAuxGeom->Flush();
		m_AuxGeomShader->Unuse();
	}
	m_FrameID++;
}

void GLRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
	Vec4 vp;
	gl::GetFloatv(GL_VIEWPORT, &vp[0]);
	*x		= vp.x;
	*y		= vp.y;
	*width  = vp.z;
	*height = vp.w;
}

void GLRenderer::SetViewport(int x, int y, int width, int height)
{
	gl::ViewPort(glm::vec4(x, y, width, height));
}

void GLRenderer::SetScissor(int x, int y, int width, int height)
{
	gl::Scissor(x, y, width, height);
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
	GLint drawFboId = 0; // , readFboId = 0;
	gl::GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);

	gl::BindFramebuffer(0);
	GLint dims[4] = {0};
	gl::GetIntegerv(GL_VIEWPORT, dims);
	GLint width  = dims[2];
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
	SetState(State::PROGRAM_POINT_SIZE, true);
	glLineWidth(1);
	SetState(State::DEPTH_TEST, true);
	SetState(State::BLEND, true);
	SetState(State::CULL_FACE, true);
	SetCullMode(CullMode::BACK);

	m_Hardware.vendor		= gl::GetString(GL_VENDOR);
	m_Hardware.render		= gl::GetString(GL_RENDERER);
	m_Hardware.version		= gl::GetString(GL_VERSION);
	m_Hardware.glsl_version = gl::GetString(GL_SHADING_LANGUAGE_VERSION);
}

void GLRenderer::fillSates()
{
#define STATEMAP(k, v) stateMap.insert(std::make_pair(k, v))
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

CVertexBuffer* GLRenderer::CreateBuffer(int vertexcount, int vertexformat, const char* szSource, bool bDynamic /* = false*/)
{
	return m_BufferManager->Create(vertexcount, vertexformat, szSource, bDynamic);
}

void GLRenderer::ReleaseBuffer(CVertexBuffer* bufptr)
{
}

void GLRenderer::DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start /* = 0*/, int vert_stop /* = 0*/, CMatInfo* mi /* = NULL*/)
{
	m_BufferManager->Draw(src, indicies, numindices, offsindex, prmode, vert_start, vert_stop, mi);
}

void GLRenderer::UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs /* = 0*/, int Type /* = 0*/)
{
	m_BufferManager->Update(dest, src, vertexcount, bUnLock, nOffs, Type);
}

void GLRenderer::CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount)
{
	m_BufferManager->Create(dest, src, indexcount);
}

void GLRenderer::UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock /* = true*/)
{
	m_BufferManager->Update(dest, src, indexcount, bUnLock);
}

void GLRenderer::ReleaseIndexBuffer(SVertexStream* dest)
{
}

void GLRenderer::ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz)
{
	auto s = glm::project(Vec3(ptx, pty, ptz), m_Camera.GetViewMatrix(), m_Camera.getProjectionMatrix(), Vec4(0, 0, GetWidth(), GetHeight()));

	*sx = s.x;
	*sy = s.y;
	*sz = s.z;
}

int GLRenderer::UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4])
{
	return 0;
}

int GLRenderer::UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz)
{
	Vec4d vp;                  // Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, &vp[0]);           // Retrieves The Viewport Values (X, Y, Width, Height)
	auto p = glm::unProject(
		glm::vec3(sx, GetHeight() - sy, sz), m_Camera.GetViewMatrix(), m_Camera.getProjectionMatrix(), glm::vec4(0, 0, GetWidth(), GetHeight()));
	*px = p.x;
	*py = p.y;
	*pz = p.z;

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
	mat = &m_Camera.getProjectionMatrix()[0][0];
}

Vec3 GLRenderer::GetUnProject(const Vec3& WindowCoords, const CCamera& cam)
{
	auto& c = WindowCoords;
	return glm::unProject(
		glm::vec3(c.x, GetHeight() - c.y, 0), cam.GetViewMatrix(), cam.getProjectionMatrix(), glm::vec4(0, 0, GetWidth(), GetHeight()));
}

int GLRenderer::GetFrameID(bool bIncludeRecursiveCalls /* = true*/)
{
	return m_FrameID;
}

void GLRenderer::printHardware()
{
	std::stringstream hardware_info;
	hardware_info << "Hardware render info\n"
				  << "Vendor: [" << m_Hardware.vendor << "]\n"
														 "Render: ["
				  << m_Hardware.render << "]\n"
										  "Version: ["
				  << m_Hardware.version << "]\n"
										   "Shader Language Version: ["
				  << m_Hardware.glsl_version << "]\n";
	m_pSystem->Log(hardware_info.str().c_str());
}

void GLRenderer::AquireVB()
{
	//CreateBuffer(INIT_BUFFER_SIZE, VERTEX_FORMAT_P3F_N_T2F, "VertexBufferPool", false);
}

bool GLRenderer::VBF_InPool(int format)
{
	return false;
}

ITexture* GLRenderer::LoadTexture(const char* nameTex, uint flags, byte eTT)
{
	TextureManager::instance()->getTexture(nameTex, eTT);
	return nullptr;
}

IFont* GLRenderer::GetIFont()
{
	return CreateIFont();
}

void GLRenderer::PushProfileMarker(char* label)
{
	OpenglDebuger::PushGroup(0, std::strlen(label), label);
}

void GLRenderer::PopProfileMarker(char* label)
{
	OpenglDebuger::EndGroup();
}

IRenderAuxGeom* GLRenderer::GetIRenderAuxGeom()
{
	return m_RenderAuxGeom;
}

void GLRenderer::SetRenderTarget(int nHandle)
{
	if (m_CurrentTarget != nHandle)
		gl::BindFramebuffer(nHandle);
}

IShaderProgram* GLRenderer::Sh_Load(const char* name, int flags)
{
	using ShaderInfo = IShaderProgram::ShaderInfo;
	auto vs			 = CShader::load(ShaderDesc("test.vs", IShader::E_VERTEX));
	auto fs			 = CShader::load(ShaderDesc("test.frag", IShader::E_FRAGMENT));
	auto p			 = new CShaderProgram(ShaderInfo(vs, std::string("test.vs")), ShaderInfo(fs, std::string("test.vs")));
	p->Create("TestProgram");
	m_Shaders.push_back(p);
	return p;
}

IShaderProgram* GLRenderer::Sh_Load(const char* vertex, const char* fragment)
{
	return gShMan->Sh_Load(vertex, fragment);
}

void GLRenderer::DrawFullscreenQuad()
{
	DrawBuffer(m_VertexBuffer, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
}

void GLRenderer::SetCullMode(CullMode mode /* = CullMode::BACK*/)
{
	if (mode == CullMode::FRONT_AND_BACK)
		gl::CullFace(GL_FRONT_AND_BACK);
	else
		gl::CullFace(GL_FRONT + static_cast<uint>(mode));
}

bool GLRenderer::DeleteContext(WIN_HWND hWnd)
{
	return false;
}

bool GLRenderer::CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX /* = 1*/, int SSY /* = 1*/)
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
		width  = GetWidth(),
		height = GetHeight();
	gl::BindFramebuffer(0);
	SetViewport(0, 0, width, height);
	m_ScreenShader->Use();
	auto proj	  = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	auto transform = glm::scale(proj, glm::vec3(width, height, 1));
	m_ScreenShader->Uniform(transform, "transform");
	SetState(State::DEPTH_TEST, false);
	m_ScreenShader->BindTextureUnit2D(texture_id, 0);
	DrawFullscreenQuad();
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
	float
		width  = GetWidth(),
		height = GetHeight();
	//gl::BindFramebuffer(0);
	SetState(State::BLEND, true);
	SetState(IRenderer::State::CULL_FACE, false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_ScreenShader->Use();
	m_ScreenShader->Uniform(a, "alpha");

	glm::mat4 model(1.0);
	auto uv_projection   = glm::mat4(1.0);
	glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f);

	//model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.f));
	model = glm::scale(model, {w, h, 1.f});

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
	DrawFullscreenQuad();
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

bool GLRenderer::InitResourceManagers()
{
	//====================================================
	gEnv->pConsole->PrintLine("Begin loading resources");
	if (!gEnv->IsDedicated())
	{
		if (!ShaderManager::init())
			return false;
		gEnv->pConsole->PrintLine("End loading resources");
	}
	return true;
}

void GLRenderer::CreateQuad()
{
	SVF_P3F_T2F verts[] = {
		{{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
		{{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},

		{{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
		{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}};
	m_VertexBuffer = gEnv->pRenderer->CreateBuffer(6, VertFormatForComponents(false, false, false, true), "screen_quad", false);
	UpdateBuffer(m_VertexBuffer, verts, 6, false);
}

IGraphicsDeviceConstantBuffer* GLRenderer::CreateConstantBuffer(int size)
{
	return nullptr;
}

ITechniqueManager* GLRenderer::GetITechniqueManager()
{
	return TechniqueManager::instance();
}

float GLRenderer::GetDepthValue(int x, int y)
{
	float out;
	glReadPixels( x, GetHeight()-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &out);
	return out;
}

void GLRenderer::Sh_Reload()
{
	gShMan->ReloadAll();
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new GLRenderer(pSystem);
}
