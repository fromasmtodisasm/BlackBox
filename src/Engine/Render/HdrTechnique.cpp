#include <BlackBox/Render/HdrTechnique.hpp>
#include <BlackBox/Render/ShadowMapTechnique.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Scene.hpp>
#include <BlackBox/Render/SkyBox.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/ISystem.hpp>

#define NBLOOM
#define IMAGE 0
#define PREVIOS 0
#define CURRENT 1

HdrTechnique::HdrTechnique()
	:
	shadowMapping(nullptr),

  m_HdrBuffer(nullptr),
  m_Scene(nullptr),

	exposure(nullptr),
	enabled(nullptr),
	bloom(nullptr),
	bloomThreshold(nullptr),
	blurOn(nullptr),
	useBoxFilter(nullptr),
	defaultFilter(nullptr),
	bloom_exposure(nullptr),
	offset(nullptr),
	bloomTime(nullptr),
	upsampleTime(nullptr),
	downsampleTime(nullptr),
	averageBloomTime(nullptr),
	downsampleType(nullptr),
	blurOnly(nullptr),
	cam_height(nullptr),
	cam_width(nullptr),
	render(nullptr)
{
}

HdrTechnique::~HdrTechnique()
{
	DeleteFrameBuffers();
	SAFE_DELETE(shadowMapping);
}

bool HdrTechnique::Init(Scene* pScene, FrameBufferObject* renderTarget)
{
  if (inited)
    return true;
  render = GetISystem()->GetIRender();
  m_Scene = pScene;
	initConsoleVariables();
	initTest();
  createShader();
	CreateCommands();
  shadowMapping = new ShadowMapping();
	auto console = GetISystem()->GetIConsole();

	auto format = m_DispFormats.get();
	
	CreateFrameBuffers(format);

	// generate three query objects
	glGenQueries(3, timer_queries);

  inited = true;
  return shadowMapping->Init(pScene, m_HdrBuffer);
}

void HdrTechnique::CreateFrameBuffers(SDispFormat* format)
{
	glm::ivec2 resolution;// = glm::ivec2(render->GetWidth(), render->GetHeight());
	auto w = GetISystem()->GetIConsole()->GetCVar("r_backbuffer_w");
	auto h = GetISystem()->GetIConsole()->GetCVar("r_backbuffer_h");
	if (format != nullptr)
	{
		resolution = glm::ivec2(format->m_Width, format->m_Height);
	}
	else if (w == nullptr || h == nullptr)
	{
		resolution = glm::ivec2(render->GetWidth(), render->GetHeight());
		GetISystem()->Log("Use window resulution");
	}
	else
		resolution = glm::ivec2(w->GetIVal(), h->GetIVal());
	cam_width->Set(resolution.x);
	cam_height->Set(resolution.y);

	m_HdrBuffer = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, resolution.x, resolution.y, 2, false);
	debuger::frame_buffer_label(m_HdrBuffer->id, "hdrBuffer");
	m_HdrBuffer->clear(gl::Color(1, 1, 1, 1));

	auto mip_cnt = getMips(resolution);
	m_DownsampleBuffer.resize(mip_cnt);
	m_UpsampleBuffer.resize(mip_cnt);

	auto create_mip_chain = [&resolution, mip_cnt](std::vector<FrameBufferObject*> &chain) {
		for (int i = 0, width = resolution.x, height = resolution.y; i < mip_cnt; i++)
		{
			chain[i] = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, width, height, 1, false);
			width >>= 1;
			if (width <= 0)
				width = 1;
			height >>= 1;
			if (height <= 0)
				height = 1;
		}
	};

	create_mip_chain(m_DownsampleBuffer);
	create_mip_chain(m_UpsampleBuffer);

	auto label_buffer = [](std::vector<FrameBufferObject*>& fb, std::string base) {
		int i = 0;
		for (auto& pass : fb)
		{
			debuger::frame_buffer_label(pass->id, base + std::to_string(i));
			i++;
		}
	};

	label_buffer(m_DownsampleBuffer, "pass0_");
	label_buffer(m_UpsampleBuffer, "pass1_");
}

void HdrTechnique::DeleteFrameBuffers()
{
	SAFE_DELETE(m_HdrBuffer);
	if (m_DownsampleBuffer.size() > 0)
	{
		for (auto& buf : m_DownsampleBuffer)
		{
			SAFE_DELETE(buf);
		}
	}
	if (m_DownsampleBuffer.size() > 0)
	{
		for (auto& buf : m_DownsampleBuffer)
		{
			SAFE_DELETE(buf);
		}
	}
	m_DownsampleBuffer.erase(m_DownsampleBuffer.begin(), m_DownsampleBuffer.end());
	m_UpsampleBuffer.erase(m_UpsampleBuffer.begin(), m_UpsampleBuffer.end());
}

bool HdrTechnique::OnRenderPass(int pass)
{
	DEBUG_GROUP(__FUNCTION__);
  if (!shadowMapping->OnRenderPass(pass))
  {
		if (m_Scene->GetSkyBox() != nullptr)
			void(0);// m_Scene->GetSkyBox()->draw(m_Scene->getCurrentCamera());
		{
			Object *water;
			if ((water = m_Scene->getObject("water")) != nullptr)
			{
				water->m_transparent = true;
				static_cast<ShadowMapping*>(shadowMapping)->RenderTransparent(water);

			}
		}
    return HdrPass();
  }
  return true;
}

int HdrTechnique::GetFrame()
{
  return shadowMapping->GetFrame();
}

bool HdrTechnique::HdrPass()
{
	if (enabled->GetIVal())
	{
		m_Scene->setPostProcessor(this);
		PROFILER_PUSH_GPU_MARKER("BLOOM PASS", Utils::COLOR_DARK_GREEN);
		BloomPass();
		PROFILER_POP_GPU_MARKER();
	}
	else
		;// m_Scene->setPostProcessor(nullptr);

  return false;
}

void HdrTechnique::BloomPass()
{
#if 0
	GLuint64 time_0, time_1, time_2;
#endif

	{
		const char section[] = "DOWNSAMPLING";
		PROFILER_PUSH_CPU_MARKER(section, Utils::COLOR_BLACK);
			DEBUG_GROUP(section);
			downsampling();
		PROFILER_POP_CPU_MARKER();
	}
	{
		const char section[] = "UPSAMPLING";
		PROFILER_PUSH_CPU_MARKER(section, Utils::COLOR_RED);
			DEBUG_GROUP(section);
			upsampling();
		PROFILER_POP_CPU_MARKER();
	}


#if 0
	glGetQueryObjectui64v(timer_queries[0], GL_QUERY_RESULT, &time_0);
	glGetQueryObjectui64v(timer_queries[1], GL_QUERY_RESULT, &time_1);
	glGetQueryObjectui64v(timer_queries[2], GL_QUERY_RESULT, &time_2);

	upsampleTime->Set(float((time_1 - time_0) / 1e06));
	downsampleTime->Set(float((time_2 - time_1) / 1e06));
#endif
}

void HdrTechnique::createShader()
{
	ProgramDesc desc[] = {
		//0
		{
			"hdr_shader",
			"screenshader.vs",
			"hdrshader.frag"
		},
		//1
		{
			"downsampling",
			"screenshader.vs",
			"downsampling.frag"
		},
		//2
		{
			"downsampling_compute",
			"",
			"",
			"",
			"downsampling.comp"
		},
		//3
		{
			"upsampling",
			"screenshader.vs",
			"upsampling.frag",
		},
		//4
		{
			"upsampling_compute",
			"",
			"",
			"",
			"upsampling.comp"
		},
	};

	desc[1].vs.macro["STORE_TEXCOORDS"] = 1;

	MaterialManager::instance()->loadProgram(desc[0],false);
	MaterialManager::instance()->loadProgram(desc[1],false);
	MaterialManager::instance()->loadProgram(desc[2],false);
	MaterialManager::instance()->loadProgram(desc[3],false);
	MaterialManager::instance()->loadProgram(desc[4],false);

	m_ScreenShader = MaterialManager::instance()->getProgram(desc[0].name);
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(0,"scene");
	m_ScreenShader->setUniformValue(1,"bloomBlur");
	m_ScreenShader->unuse();

	m_DownsampleShader = MaterialManager::instance()->getProgram(desc[1].name);
	m_DownsampleShader->use();
	m_DownsampleShader->setUniformValue(0,"image");
	m_DownsampleShader->unuse();

	m_DownsampleComputeShader = MaterialManager::instance()->getProgram(desc[2].name);
	m_DownsampleComputeShader->use();
	m_DownsampleComputeShader->setUniformValue(0,"image");
	m_DownsampleComputeShader->unuse();

	m_UpsampleShader = MaterialManager::instance()->getProgram(desc[3].name);

	m_UpsampleShaderComputeShader = MaterialManager::instance()->getProgram(desc[4].name);

	const int MAX_CORNERS = 4;
	glGenBuffers(1, &quadCornersVBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, quadCornersVBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_CORNERS * sizeof(glm::vec2),	NULL, GL_DYNAMIC_DRAW);

}

void HdrTechnique::initConsoleVariables()
{
  exposure =				CREATE_CONSOLE_VAR("exp", 1.0f, 0, "exposure");
  enabled =					CREATE_CONSOLE_VAR("hdr", 1, 0, "Enable/disable HDR");
  bloom =						CREATE_CONSOLE_VAR("bloom", 1, 0, "Enable/disable HDR");
  bloomThreshold =	CREATE_CONSOLE_VAR("bt", 2.0f, 0, "Bloom threshold");
  blurOn =					CREATE_CONSOLE_VAR("blur", 1, 0, "Enable/disable blur for bloom");
  blurOnly =				CREATE_CONSOLE_VAR("blur_only", 0, 0, "Enable/disable blur for bloom");
  bloom_exposure =	CREATE_CONSOLE_VAR("bexp", 0.007f, 0, "Enable/disable blur for bloom");
  bloomTime =				CREATE_CONSOLE_VAR("bloomtime", 0.f, 0, "Time of bloom");
  upsampleTime =		CREATE_CONSOLE_VAR("uptime", 0.f, 0, "Time of bloom");
  downsampleTime =	CREATE_CONSOLE_VAR("dtime", 0.f, 0, "Time of bloom");
  downsampleType =	CREATE_CONSOLE_VAR("dtype", 0, 0, "Type of bloom downsample [0-standard/1-compute]");
  offset =					CREATE_CONSOLE_VAR("offset", -3.0f, 0, "Enable/disable blur for bloom");
  useBoxFilter =		CREATE_CONSOLE_VAR("bf", 0, 0, "Enable/disable BoxFilter in bloom");
  defaultFilter =		CREATE_CONSOLE_VAR("df", 1, 0, "Enable/disable default filtering in bloom");

	cam_width		= GetISystem()->GetIConsole()->GetCVar("r_cam_w");
	cam_height	= GetISystem()->GetIConsole()->GetCVar("r_cam_h");
}

void HdrTechnique::initTest()
{
	auto render = GetISystem()->GetIRender();
	auto numFormats = render->EnumDisplayFormats(nullptr);
	if (numFormats > 0)
	{
		m_DispFormats = std::shared_ptr<SDispFormat>(new SDispFormat[numFormats]);
		render->EnumDisplayFormats(m_DispFormats.get());
	}

	testid = std::min(GetISystem()->GetIConsole()->GetCVar("testid")->GetIVal(), numFormats);
}

int HdrTechnique::getMips(glm::vec2 resolution)
{
	//return static_cast<int>(std::log2(std::max(m_HdrBuffer->viewPort.z, m_HdrBuffer->viewPort.w)) + 1);
	return 6;
}

void HdrTechnique::CreateCommands()
{
	struct HdrCommands : public IConsoleCommand
	{
	public:
		HdrCommands(HdrTechnique* tech) : tech(tech) {}
		virtual bool execute(CommandDesc& cd) override
		{
			GetISystem()->GetIConsole()->PrintLine("set mode!");
			if (cd.argsCount() == 1)
			{
				tech->SetMode(_wtoi(cd.get(0).c_str()));
				return true;
			}
			return false;
		}
		HdrTechnique* tech;
	};

	IConsoleCommand *cmd = new HdrCommands(this);
	GetISystem()->GetIConsole()->AddCommand(
		"setmode", 
		cmd,
		"Set display mode"
		);
}

int HdrTechnique::SetRenderTarget(FrameBufferObject* renderTarget)
{
	return 0;
}

void HdrTechnique::SetMode(int n)
{
	DeleteFrameBuffers();
	CreateFrameBuffers(&m_DispFormats.get()[n]);
	shadowMapping->SetRenderTarget(m_HdrBuffer);
}

bool HdrTechnique::PreRenderPass()
{
  return false;
}

void HdrTechnique::PostRenderPass()
{
}

void HdrTechnique::downsampling()
{
	if (downsampleType->GetIVal() == 0)
		downsamplingStandard();
	else
		downsamplingCompute();
}

void HdrTechnique::downsamplingStandard()
{
	// 2. blur bright fragments with two-pass Gaussian Blur 
	// --------------------------------------------------
	bool horizontal = true, first_iteration = true;
	unsigned int amount = PASSES;
	m_DownsampleShader->use();
  m_DownsampleShader->setUniformValue(offset->GetFVal(), "offset");

	render->SetState(IRender::State::DEPTH_TEST, false);
	amount = getMips({ m_DownsampleBuffer[0]->viewPort.z, m_DownsampleBuffer[0]->viewPort.w });

		
	auto w = cam_width->GetFVal();
	auto h = cam_height->GetFVal();
	auto hdr_w = m_HdrBuffer->viewPort.z;
	auto hdr_h = m_HdrBuffer->viewPort.w;

	m_ScreenShader->setUniformValue(w/hdr_w, "vx");
	m_ScreenShader->setUniformValue(h/hdr_h, "vy");
	//m_ScreenShader->setUniformValue(glm::vec4(0,0,w, h) / glm::vec4(hdr_w,hdr_h,hdr_w,hdr_h), "view");
	for (unsigned int i = 0; i < amount - 1; i++)
	{
		auto rx = w / (1 << (i + 1));
		auto ry = h / (1 << (i + 1));
		auto& ds = m_DownsampleShader;
		ds->setUniformValue(rx, "rx");
		ds->setUniformValue(rx, "ry");
		m_DownsampleBuffer[i + 1]->bind({ 0,0, rx, ry });

		ds->bindTextureUnit2D(first_iteration ? m_HdrBuffer->texture[1] : m_DownsampleBuffer[i]->texture[0], IMAGE);
		m_ScreenQuad.draw();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	pingpong = !horizontal;
	m_DownsampleShader->unuse();

#if 0
	glBindBufferBase(GL_ARRAY_BUFFER, 2, quadCornersVBO);
	glm::vec2 *buf = static_cast<glm::vec2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));

	for (int i = 0; i < 4; i++)
	{
		std::cout << "Corner " << i << buf[i].x << ", " << buf[i].y;
	}
#endif

	GetISystem()->GetIRender()->SetState(IRender::State::DEPTH_TEST, true);
}

void HdrTechnique::downsamplingCompute()
{
	m_DownsampleComputeShader->use();
	{
		unsigned int image_unit = 2;
		m_DownsampleComputeShader->bindTexture2D(m_HdrBuffer->texture[1], image_unit, "inputImg1");
	}

	{
		unsigned int image_unit = 3;
		glBindImageTexture(image_unit, m_UpsampleBuffer[0]->texture[0], 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
		m_DownsampleComputeShader->setUniformValue(3, "inputImg2");
	}

	auto render = GetISystem()->GetIRender();
	m_DownsampleComputeShader->dispatch(render->GetWidth() / 6, render->GetHeight() / 6, 1, GL_SHADER_STORAGE_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
	return;
}

void HdrTechnique::upsampling()
{
	m_UpsampleShader->use();
	m_UpsampleShader->setUniformValue(blurOn->GetIVal(), "blurOn");
	m_UpsampleShader->setUniformValue(blurOnly->GetIVal(), "blurOnly");
	
	uint32_t amount;
	bool first_iteration = true;
	render->SetState(IRender::State::DEPTH_TEST, false);

		auto w = (float)cam_width->GetIVal();
		auto h = (float)cam_height->GetIVal();
		auto hdr_w = m_HdrBuffer->viewPort.z;
		auto hdr_h = m_HdrBuffer->viewPort.w;
	//m_ScreenShader->setUniformValue(glm::vec4(0,0,w, h) / glm::vec4(hdr_w,hdr_h,hdr_w,hdr_h), "viewPortf");
	m_ScreenShader->setUniformValue(w/hdr_w, "vx");
	m_ScreenShader->setUniformValue(h/hdr_h, "vy");

	amount = getMips({ m_DownsampleBuffer[0]->viewPort.z, m_DownsampleBuffer[0]->viewPort.w });
	for (unsigned int i = amount - 1; i > 0; i--)
	{
		auto rx = (w / hdr_w) * m_UpsampleBuffer[i-1]->viewPort.z;
		auto ry = (h / hdr_h) * m_UpsampleBuffer[i-1]->viewPort.w;
		auto& up = m_UpsampleShader;
		up->setUniformValue(rx, "rx");
		up->setUniformValue(rx, "ry");

		m_UpsampleBuffer[i - 1]->bind(glm::vec4(0,0, rx, ry));
		m_UpsampleShader->bindTexture2D(first_iteration ? m_DownsampleBuffer[amount - 1]->texture[0] : m_UpsampleBuffer[i]->texture[0],			PREVIOS, "previos");
		m_UpsampleShader->bindTexture2D(first_iteration ? m_DownsampleBuffer[amount - 2]->texture[0] : m_DownsampleBuffer[i - 1]->texture[0], CURRENT, "current");
		m_ScreenQuad.draw();
		if (first_iteration)
			first_iteration = false;
	}
	m_UpsampleShader->unuse();
}

void HdrTechnique::Do(unsigned int texture)
{
	DEBUG_GROUP(__FUNCTION__);
	glm::vec3 fog = glm::vec3(
		GetISystem()->GetIConsole()->GetCVar("fogR")->GetFVal(),
		GetISystem()->GetIConsole()->GetCVar("fogG")->GetFVal(),
		GetISystem()->GetIConsole()->GetCVar("fogB")->GetFVal());
	//FrameBufferObject::bindDefault(m_HdrBuffer->viewPort);
	FrameBufferObject::bindDefault({ 0,0, cam_width->GetIVal(), cam_height->GetIVal() });
	
	m_ScreenShader->use();
  m_ScreenShader->setUniformValue(exposure->GetFVal(), "exposure");
  m_ScreenShader->setUniformValue(bloom_exposure->GetFVal(), "bloom_exposure");
	render->SetState(IRender::State::DEPTH_TEST, false);

	m_ScreenShader->bindTexture2D(m_HdrBuffer->texture[0], 0, "scene");
	m_ScreenShader->bindTexture2D(m_UpsampleBuffer[0]->texture[0], 1, "bloomBlur");
	m_ScreenShader->setUniformValue(bloom->GetIVal(), "bloom");
	auto w = cam_width->GetIVal();
	auto h = cam_height->GetIVal();
	auto hdr_w = m_HdrBuffer->viewPort.z;
	auto hdr_h = m_HdrBuffer->viewPort.w;
	m_ScreenShader->setUniformValue(glm::vec4(0,0, w, h) / glm::vec4(hdr_w,hdr_h,hdr_w,hdr_h), "viewPortf");

	render->SetViewport(
		0, 0, 
		cam_width->GetIVal(), cam_height->GetIVal());
	m_ScreenQuad.draw();
}


#if 0
GLuint timer_queries[3];
// generate three query objects
glGenQueries(3, &timer_queries);
[...]
// query time stamps around all draw routines
glQueryCounter(timer_queries[0], GL_TIMESTAMP);
draw_world();
glQueryCounter(timer_queries[1], GL_TIMESTAMP);
draw_models();
glQueryCounter(timer_queries[2], GL_TIMESTAMP);
[...]
// later in the program retrieve the query results
GLuint64 time_0, time_1, time_2;
glGetQueryObjectui64v(timer_queries[0], GL_QUERY_RESULT, &time_0);
glGetQueryObjectui64v(timer_queries[1], GL_QUERY_RESULT, &time_1);
glGetQueryObjectui64v(timer_queries[2], GL_QUERY_RESULT, &time_2);
printf(" world draw time : %f ms \n", double(time_1 - time_0) / 1 e06);
printf(" models draw time: %f ms \n", double(time_2 - time_1) / 1 e06);
#endif
