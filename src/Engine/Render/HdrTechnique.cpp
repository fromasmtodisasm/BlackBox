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

  hdrBuffer(nullptr),
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
	downsampleType(nullptr)
{
}

HdrTechnique::~HdrTechnique()
{
	DeleteFrameBuffers();
	SAFE_RELEASE(shadowMapping);
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
  return shadowMapping->Init(pScene, hdrBuffer);
}

void HdrTechnique::CreateFrameBuffers(SDispFormat* format)
{
	glm::ivec2 resolution;// = glm::ivec2(render->GetWidth(), render->GetHeight());
	auto w = GetISystem()->GetIConsole()->GetCVar("r_backbuffer_w");
	auto h = GetISystem()->GetIConsole()->GetCVar("r_backbuffer_h");
	if (w == nullptr || h == nullptr)
	{
		resolution = glm::ivec2(render->GetWidth(), render->GetHeight());
		GetISystem()->Log("Use window resulution");
	}
	else
		resolution = glm::ivec2(w->GetIVal(), h->GetIVal());
	hdrBuffer = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, resolution.x, resolution.y, 2, false);
	debuger::frame_buffer_label(hdrBuffer->id, "hdrBuffer");

	auto mip_cnt = getMips(resolution);
	pass0.resize(mip_cnt);
	pass1.resize(mip_cnt);

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

	create_mip_chain(pass0);
	create_mip_chain(pass1);

	auto label_buffer = [](std::vector<FrameBufferObject*>& fb, std::string base) {
		int i = 0;
		for (auto& pass : fb)
		{
			debuger::frame_buffer_label(pass->id, base + std::to_string(i));
			i++;
		}
	};

	label_buffer(pass0, "pass0_");
	label_buffer(pass1, "pass1_");
}

void HdrTechnique::DeleteFrameBuffers()
{
	SAFE_RELEASE(hdrBuffer);
	if (pass0.size() > 0)
	{
		for (auto& buf : pass0)
		{
			SAFE_RELEASE(buf);
		}
	}
	if (pass0.size() > 0)
	{
		for (auto& buf : pass0)
		{
			SAFE_RELEASE(buf);
		}
	}
	pass0.erase(pass0.begin(), pass0.end());
	pass1.erase(pass1.begin(), pass1.end());
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

}

void HdrTechnique::initConsoleVariables()
{
  exposure =				CREATE_CONSOLE_VAR("exp", 1.0f, 0, "exposure");
  enabled =					CREATE_CONSOLE_VAR("hdr", 1, 0, "Enable/disable HDR");
  bloom =						CREATE_CONSOLE_VAR("bloom", 1, 0, "Enable/disable HDR");
  bloomThreshold =	CREATE_CONSOLE_VAR("bt", 2.0f, 0, "Bloom threshold");
  blurOn =					CREATE_CONSOLE_VAR("blur", 1, 0, "Enable/disable blur for bloom");
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
	//return std::log2(std::max(pass0[0]->viewPort.z, pass0[0]->viewPort.w)) + 1;
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
	shadowMapping->SetRenderTarget(hdrBuffer);
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
  m_DownsampleShader->setUniformValue(useBoxFilter->GetIVal(), "use_box_filter");
  m_DownsampleShader->setUniformValue(defaultFilter->GetIVal(), "default_filter");
  m_DownsampleShader->setUniformValue(offset->GetFVal(), "offset");
  m_DownsampleShader->setUniformValue(glm::vec4(0,0, cam_width->GetIVal()/hdrBuffer->viewPort.z, cam_height->GetIVal()/hdrBuffer->viewPort.w), "viewPort");

	glCheck(glDisable(GL_DEPTH_TEST));
	if (useBoxFilter->GetIVal())
		amount = 1;
	else
		amount = getMips({ pass0[0]->viewPort.z, pass0[0]->viewPort.w });

		
		auto w = cam_width->GetIVal();
		auto h = cam_height->GetIVal();
		auto hdr_w = hdrBuffer->viewPort.z;
		auto hdr_h = hdrBuffer->viewPort.w;
	m_ScreenShader->setUniformValue(glm::vec2(w, h) / glm::vec2(hdr_w,hdr_h), "viewPort");
	for (unsigned int i = 0; i < amount - 1; i++)
	{
		pass0[i + 1]->bind({ 0,0, (w / hdr_w) * pass0[i + 1]->viewPort.z, (h / hdr_h) * pass0[i + 1]->viewPort.w });
		//pass0[i + 1]->bind(pass0[i + 1]->viewPort / 2.f);
		m_DownsampleShader->bindTextureUnit2D(first_iteration ? hdrBuffer->texture[1] : pass0[i]->texture[0], IMAGE);
		//renderQuad();
		m_ScreenQuad.draw();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	pingpong = !horizontal;
	m_DownsampleShader->unuse();
	GetISystem()->GetIRender()->SetState(IRender::State::DEPTH_TEST, true);
}

void HdrTechnique::downsamplingCompute()
{
	m_DownsampleComputeShader->use();
	{
		unsigned int image_unit = 2;
		m_DownsampleComputeShader->bindTexture2D(hdrBuffer->texture[1], image_unit, "inputImg1");
	}

	{
		unsigned int image_unit = 3;
		glBindImageTexture(image_unit, pass1[0]->texture[0], 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
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
	
	uint32_t amount;
	bool first_iteration = true;
	glCheck(glDisable(GL_DEPTH_TEST));

		auto w = cam_width->GetIVal();
		auto h = cam_height->GetIVal();
		auto hdr_w = hdrBuffer->viewPort.z;
		auto hdr_h = hdrBuffer->viewPort.w;
	m_ScreenShader->setUniformValue(glm::vec2(w, h) / glm::vec2(hdr_w,hdr_h), "viewPort");

	amount = getMips({ pass0[0]->viewPort.z, pass0[0]->viewPort.w });
	for (unsigned int i = amount - 1; i > 0; i--)
	{

		pass1[i - 1]->bind(glm::vec4(0,0, (w / hdr_w) * pass1[i-1]->viewPort.z, (h / hdr_h) * pass1[i-1]->viewPort.w));
		//pass1[i - 1]->bind(pass1[i - 1]->viewPort / 2.f);
		m_UpsampleShader->bindTexture2D(first_iteration ? pass0[amount - 1]->texture[0] : pass1[i]->texture[0],			PREVIOS, "previos");
		m_UpsampleShader->bindTexture2D(first_iteration ? pass0[amount - 2]->texture[0] : pass0[i - 1]->texture[0], CURRENT, "current");
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
	//FrameBufferObject::bindDefault(hdrBuffer->viewPort);
	FrameBufferObject::bindDefault({ 0,0, cam_width->GetIVal(), cam_height->GetIVal() });
	
	m_ScreenShader->use();
  m_ScreenShader->setUniformValue(exposure->GetFVal(), "exposure");
  m_ScreenShader->setUniformValue(bloom_exposure->GetFVal(), "bloom_exposure");
	render->SetState(IRender::State::DEPTH_TEST, false);

	m_ScreenShader->bindTexture2D(hdrBuffer->texture[0], 0, "scene");
	m_ScreenShader->bindTexture2D(pass1[0]->texture[0], 1, "bloomBlur");
	m_ScreenShader->setUniformValue(bloom->GetIVal(), "bloom");
	auto w = cam_width->GetIVal();
	auto h = cam_height->GetIVal();
	auto hdr_w = hdrBuffer->viewPort.z;
	auto hdr_h = hdrBuffer->viewPort.w;
	m_ScreenShader->setUniformValue(glm::ivec4(0,0, w, h), "viewPorti");
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
