#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>

#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/BaseShader.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include "Renderer.hpp"
#include <BlackBox/Renderer/ShaderManager.hpp>
#include <BlackBox/Renderer/TechniqueManager.hpp>
#include <BlackBox/Renderer/TextureManager.hpp>
#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IWindow.hpp>

#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/VertexBuffer.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#include <BlackBox/World/World.hpp>

#include "../Shaders/FxParser.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <SDL2/SDL.h>

#include <BlackBox/Core/Utils.hpp>
#include <sstream>

#pragma warning(push)
#pragma warning(disable : 4244)

extern FxParser* g_FxParser;
extern FxParser s_FxParser;

GLRenderer::GLRenderer(ISystem* engine) : CRenderer(engine)
{
	m_pSystem->GetISystemEventDispatcher()->RegisterListener(this, "GLRenderer");
}

GLRenderer::~GLRenderer()
{
	for (auto* t : m_RenderTargets)
	{
		SAFE_RELEASE(t);
	}

	SAFE_DELETE(m_MainMSAAFrameBuffer);
	SAFE_DELETE(m_MainReslovedFrameBuffer);

	SAFE_DELETE(gShMan);
	gEnv->pConsole->RemoveConsoleVarSink(this);
}

IWindow* GLRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	return CRenderer::Init(x,y,width, height, cbpp, zbpp, sbits, fullscreen, window);
}

bool GLRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void GLRenderer::BeginFrame(void)
{
	Vec4d vp(0, 0, GetWidth(), GetHeight());
	if (m_pRenderCallback)
		m_pRenderCallback->CallBack(IRenderCallback::eOnRender);
	//GetViewport(&vp[0], &vp[1], &vp[2], &vp[3]);

	m_MainMSAAFrameBuffer->bind({vp[0], vp[1], vp[2], vp[3]});
	//m_MainMSAAFrameBuffer->clear({m_clearColor, 1});
	float depth = 1.f;
	glClearBufferfv(GL_COLOR, 0, &m_clearColor[0]);
	glClearBufferfv(GL_DEPTH, 0, &depth);
	//gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::Update(void)
{
	//if (bIsActive)
	{
		{
			//std::cout << " aux " << std::endl;
			Flush();
		}
		FrameBufferObject::bindDefault({0, 0, GetWidth(), GetHeight()});
		if (m_pRenderCallback)
			m_pRenderCallback->CallBack(IRenderCallback::eBeforeSwapBuffers);
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
	// Disable screenshot code path for pure release builds on consoles
	// Capture currently presented back-buffer to be able to captures frames without capture-lag
#if !defined(_RELEASE) || CRY_PLATFORM_WINDOWS || defined(ENABLE_LW_PROFILERS)
	if (CV_r_GetScreenShot)
	{
		ScreenShot(nullptr);
		CV_r_GetScreenShot = 0;
	}
#endif

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

bool GLRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void GLRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

int GLRenderer::SetPolygonMode(int mode)
{
	return 0;
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
	stbi_write_png(filename != 0 ? filename : "screenshot.png", width, height, 3, pixels, width * 3);

	//stbi_image_free(img);
	gl::BindFramebuffer(drawFboId);
}

void GLRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

bool GLRenderer::InitOverride()
{
	CBaseShaderProgram::use_cache = GetISystem()->GetIConsole()->GetCVar("sh_use_cache");
	FillSates();
	if (glContextType == (int)AttributeType::Debug || gEnv->pConsole->GetCVar("r_Debug")->GetIVal() == 1)
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
	
	return true;
}

void GLRenderer::FillSates()
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

void GLRenderer::PrintHardware()
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
}

void GLRenderer::AquireVB()
{
	//CreateBuffer(INIT_BUFFER_SIZE, VERTEX_FORMAT_P3F_N_T2F, "VertexBufferPool", false);
}

bool GLRenderer::VBF_InPool(int format)
{
	return false;
}

IFont* GLRenderer::GetIFont()
{
	m_Fonts.push_back(CreateIFont());
	return m_Fonts.back();
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
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
	return SDL_GL_MakeCurrent((SDL_Window*)hWnd, context) == 0;
}

void GLRenderer::MakeMainContextActive()
{
}

WIN_HWND GLRenderer::GetCurrentContextHWND()
{
	return m_hWnd;
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
	m_clearColor = Vec4(vColor, 1.f);
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

void GLRenderer::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

void GLRenderer::DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)
{
	float
		width  = GetWidth(),
		height = GetHeight();
	//gl::BindFramebuffer(0);
	RSS(this, BLEND, true);
	RSS(this, CULL_FACE, false);
	RSS(this, DEPTH_TEST, false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 model(1.0);
	auto uv_projection = glm::mat4(1.0);

	model = glm::translate(model, glm::vec3(xpos, ypos, 0.f));
	model = glm::scale(model, {w, h, 1.f});

	uv_projection = glm::scale(glm::mat4(1.0), glm::vec3(s1 - s0, t1 - t0, 1.0f));
	uv_projection = glm::translate(uv_projection, glm::vec3(s0, t0, 0.f));

	{
		auto sb			 = CRenderer::screenBuffer.get();
		sb->Color		 = Vec4(r, g, b, a);
		sb->UvProjection = uv_projection;
		sb->Model		 = model;
		sb->Alpha		 = a;
		sb->Update();
	}
	m_ScreenShader->Use();
	glProgramUniformHandleui64ARB(m_ScreenShader->Get(), glGetUniformLocation(m_ScreenShader->Get(), "screenTexture"), texture_id);

	DrawFullscreenQuad();
	m_ScreenShader->Unuse();
}

bool GLRenderer::InitResourceManagers()
{
	//====================================================
	CryLog("Begin loading resources");
	if (!gEnv->IsDedicated())
	{
		if (!ShaderManager::init())
			return false;
		CryLog("End loading resources");
	}
	return true;
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
	m_RenderTargets.push_back(Texture::create(
		Image(dm->w, dm->h, 3, std::vector<uint8_t>(), false),
		TextureType::LDR_RENDER_TARGET, buffer, false, true));
	return m_RenderTargets.back()->getId();
}

void GLRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
	m_pRenderCallback = pCallback;
}

void GLRenderer::Flush()
{
	DEBUG_GROUP("AUX");

	auto* pvb			 = perViewBuffer.get();
	pvb->Projection		 = m_Camera.getProjectionMatrix();
	pvb->View			 = m_Camera.GetViewMatrix();
	pvb->OrthoProjection = glm::ortho(0.f, float(GetWidth()), float(GetHeight()), 0.f);
	pvb->ViewProjection	 = pvb->Projection * pvb->View;
	pvb->Eye			 = gEnv->pSystem->GetViewCamera().GetPos();

	pvb->Update();
	m_RenderAuxGeom->Flush();
}

void GLRenderer::ShareResources(IRenderer* renderer)
{
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new GLRenderer(pSystem);
}
