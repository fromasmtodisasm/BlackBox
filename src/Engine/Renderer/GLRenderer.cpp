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
		for (auto& s : m_Shaders)
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

	m_pSystem->GetISystemEventDispatcher()->RegisterListener(this, "GLRenderer");
}

GLRenderer::~GLRenderer()
{
	SAFE_DELETE(m_RenderAuxGeom);
	SAFE_DELETE(m_BufferManager);
	SAFE_DELETE(m_VertexBuffer);

	for (auto t : m_RenderTargets)
	{
		SAFE_RELEASE(t);
	}

	SAFE_DELETE(m_MainMSAAFrameBuffer);
	SAFE_DELETE(m_MainReslovedFrameBuffer);

	SAFE_DELETE(gShMan);
}

IWindow* GLRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	InitCVars();
	//m_Camera.InitCVars();
	IWindow* result = m_Window = window;
	bInFullScreen = fullscreen;
	if (window == nullptr)
		return nullptr;
	//=======================
	InitConsoleCommands();
	//=======================
	if (isDebug && GET_CVAR("r_Debug")->GetIVal() == 1)
		glContextType = AttributeType::DEBUG;
	else
		glContextType = AttributeType::CORE;
	if (!m_Window->init(x, y, width, height, cbpp, zbpp, sbits, fullscreen))
		return nullptr;
	gEnv->pLog->Log("window inited");
	if (!OpenGLLoader())
		return nullptr;
	context = SDL_GL_GetCurrentContext();
	m_HWND	= (HWND)window->getHandle();
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
		return nullptr;

	printHardware();
	m_BufferManager = new CBufferManager();
	CreateQuad();
	gEnv->pLog->Log("here");
	gShMan = new ShaderMan;
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

	CreateRenderTarget();

	auto dm				  = reinterpret_cast<SDL_DisplayMode*>(window->GetDesktopMode());
	m_MainMSAAFrameBuffer = FrameBufferObject::create(dm->w, dm->h, m_RenderTargets.back(), false);

	{
		char buffer[32];
		snprintf(buffer, 32, "rt_%zd", m_RenderTargets.size());
		auto dm = reinterpret_cast<SDL_DisplayMode*>(m_Window->GetDesktopMode());
		m_RenderTargets.push_back(Texture::create(dm->w, dm->h, TextureType::LDR_RENDER_TARGET, false, buffer, false, nullptr, true));
	}

	m_MainReslovedFrameBuffer = FrameBufferObject::create(dm->w, dm->h, m_RenderTargets.back(), false);

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
	Vec4d vp(0, 0, GetWidth(), GetHeight());
	if (m_pRenerCallback)
		m_pRenerCallback->CallBack(IRenderCallback::eOnRender);
	//GetViewport(&vp[0], &vp[1], &vp[2], &vp[3]);

	m_MainMSAAFrameBuffer->bind({vp[0], vp[1], vp[2], vp[3]});
	//m_MainMSAAFrameBuffer->clear({m_clearColor, 1});
	gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::Update(void)
{
	//if (bIsActive)
	{
		{
			//std::cout << " aux " << std::endl;
			Flush();
		}
		m_MainMSAAFrameBuffer->bindDefault({0, 0, GetWidth(), GetHeight()});
		if (m_pRenerCallback)
			m_pRenerCallback->CallBack(IRenderCallback::eBeforeSwapBuffers);
		m_MainMSAAFrameBuffer->DrawTo(
			m_MainReslovedFrameBuffer,
			m_MainReslovedFrameBuffer->viewPort);
		m_MainReslovedFrameBuffer->DrawToBackbuffer(
	#if 0
			[&]() -> Vec4 {
				Vec4d r;
				GetViewport(&r.x, &r.y, &r.z, &r.w);
				return r;
			}()
	#endif
			Vec4(0, 0, GetWidth(), GetHeight()));
	}
	m_FrameID++;
}

void GLRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
	Vec4 vp;
	gl::GetFloatv(GL_VIEWPORT, &vp[0]);
	*x		= vp.x;
	*y		= vp.y;
	*width	= vp.z;
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
	GLint width	 = dims[2];
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
	if (glContextType == AttributeType::DEBUG || GET_CVAR("r_Debug")->GetIVal() == 1)
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
	SetState(State::MULTISAMPLE, true);
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
	STATEMAP(State::MULTISAMPLE, GL_MULTISAMPLE);

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
	Vec4d vp;							// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, &vp[0]); // Retrieves The Viewport Values (X, Y, Width, Height)
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
				  << "\tVendor: [" << m_Hardware.vendor << "]\n"
														   "\tRender: ["
				  << m_Hardware.render << "]\n"
										  "\tVersion: ["
				  << m_Hardware.version << "]\n"
										   "\tShader Language Version: ["
				  << m_Hardware.glsl_version << "]\n";
	m_pSystem->Log(hardware_info.str().data());
	std::cout << gl::GetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	m_pSystem->Log("OOOK");
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
	return TextureManager::instance()->getTexture(nameTex, eTT);
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
	if (
		auto it = std::find_if(m_RenderTargets.begin(), m_RenderTargets.end(), [nHandle](ITexture* texture) { return texture->getId() == nHandle; });
		it != m_RenderTargets.end())
	{
		m_MainMSAAFrameBuffer->bind();
		m_MainMSAAFrameBuffer->attach(*it);
		m_MainMSAAFrameBuffer->bindDefault(Vec4(0, 0, GetWidth(), GetHeight()));
	}
}

void GLRenderer::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_CHANGE_FOCUS:
		break;
	case ESYSTEM_EVENT_MOVE:
		break;
	case ESYSTEM_EVENT_RESIZE:
		if (!transit_to_FS)
			m_Window->changeSize(wparam, lparam);
		transit_to_FS = false;
		break;
	case ESYSTEM_EVENT_ACTIVATE:
		bIsActive = bool(wparam);
		break;
	case ESYSTEM_EVENT_LEVEL_LOAD_START:
		break;
	case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:
		break;
	case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:
		break;
	case ESYSTEM_EVENT_LANGUAGE_CHANGE:
		break;
	case ESYSTEM_EVENT_TOGGLE_FULLSCREEN:
		bInFullScreen = bool(wparam);
		m_Window->EnterFullscreen(bInFullScreen);
		break;
	case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:
		bIsActive = bool(wparam);
		break;
	default:
		break;
	}
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
	DrawBuffer(m_VertexBuffer, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLE_STRIP));
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
	DrawImage(0, 0, GetWidth(), GetHeight(), texture_id, 0, 0, 1, 1, 1, 1, 1, 1);
}

bool GLRenderer::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
	if (!strcmp(pVar->GetName(), "r_Width"))
	{
		gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_RESIZE, std::strtof(sNewValue, nullptr), GetHeight());
	}
	else if (!strcmp(pVar->GetName(), "r_Height"))
	{
		gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_RESIZE, GetWidth(), std::strtof(sNewValue, nullptr));
	}
	else if (!strcmp(pVar->GetName(), "r_Fullscreen"))
	{
		gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_TOGGLE_FULLSCREEN, std::strtol(sNewValue, nullptr, 10), 0);
	}
	else if (!strcmp(pVar->GetName(), "r_debug"))
	{
		OpenglDebuger::SetIgnore(!(bool)std::stoi(sNewValue));
	}
	return false;
}

void GLRenderer::OnAfterVarChange(ICVar* pVar)
{
}

void GLRenderer::OnVarUnregister(ICVar* pVar)
{
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
	RSS(this, BLEND, true);
	RSS(this, CULL_FACE, false);
	RSS(this, DEPTH_TEST, false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_ScreenShader->Use();
	m_ScreenShader->Uniform(Vec4(r, g, b, a), "color");

	glm::mat4 model(1.0);
	auto uv_projection	 = glm::mat4(1.0);
	glm::mat4 projection = glm::ortho(0.f, width, height, 0.f);

	model = glm::translate(model, glm::vec3(xpos, ypos, 0.f));
	model = glm::scale(model, {w, h, 1.f});

	uv_projection = glm::scale(glm::mat4(1.0), glm::vec3(1.f, 1.f, 1.0f));
	uv_projection = glm::translate(uv_projection, glm::vec3(s0, 0, 0.f));

	{
		m_ScreenShader->Uniform(projection, "projection");
		m_ScreenShader->Uniform(uv_projection, "uv_projection");
		m_ScreenShader->Uniform(model, "model");
		m_ScreenShader->Uniform(Vec4(GetWidth(), GetHeight(), 1.f / GetWidth(), -1.f / GetHeight()), "screen");
	}
	m_ScreenShader->BindTextureUnit2D(texture_id, 0);
	DrawFullscreenQuad();
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

void GLRenderer::ShareWith(GLRenderer* renderer)
{
	if (renderer != nullptr)
	{
	}
}

void GLRenderer::CreateQuad()
{
	SVF_P3F_T2F verts[] = {
		{{0, 1, 0}, {0,1}},
		{{0, 0, 0}, {0,0}},
		{{1, 1, 0}, {1,1}},
		{{1, 0, 0}, {1,0}}};
	gEnv->pLog->Log("here");
	m_VertexBuffer = gEnv->pRenderer->CreateBuffer(4, VERTEX_FORMAT_P3F_T2F, "screen_quad", false);
	UpdateBuffer(m_VertexBuffer, verts, 4, false);
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
	glReadPixels(x, GetHeight() - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &out);
	return out;
}

void GLRenderer::Sh_Reload()
{
	gShMan->ReloadAll();
}

int GLRenderer::CreateRenderTarget()
{
	char buffer[32];
	snprintf(buffer, 32, "rt_%zd", m_RenderTargets.size());
	auto dm = reinterpret_cast<SDL_DisplayMode*>(m_Window->GetDesktopMode());
	m_RenderTargets.push_back(Texture::create(dm->w, dm->h, TextureType::LDR_RENDER_TARGET, false, buffer, false, nullptr, true));
	return m_RenderTargets.back()->getId();
}

void GLRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
	m_pRenerCallback = pCallback;
}

void GLRenderer::Flush()
{
	DEBUG_GROUP("AUX");
	m_AuxGeomShader->Use();
	m_AuxGeomShader->Uniform(m_Camera.getProjectionMatrix(), "projection");
	m_AuxGeomShader->Uniform(m_Camera.GetViewMatrix(), "view");
	m_RenderAuxGeom->Flush();
	m_AuxGeomShader->Unuse();
}

void GLRenderer::ShareResources(IRenderer* renderer)
{
	ShareWith(dynamic_cast<GLRenderer*>(renderer));
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new GLRenderer(pSystem);
}
