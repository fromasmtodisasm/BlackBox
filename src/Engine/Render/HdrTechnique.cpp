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

#define CREATE_CONSOLE_VAR(name, value, flags, ...) GetISystem()->GetIConsole()->CreateVariable(name, value, flags, __VA_ARGS__)

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
	averageBloomTime(nullptr)
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
	glm::ivec2 resolution = glm::ivec2(render->GetWidth(), render->GetHeight());
	hdrBuffer = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, resolution.x, resolution.y, 2, false);

	auto mip_cnt = getMips(resolution);
	pass0.resize(mip_cnt);
	pass1.resize(mip_cnt);

	auto create_mip_chain = [&resolution, mip_cnt](std::vector<FrameBufferObject*> &chain) {
		for (int i = 0, width = 2 * resolution.x, height = 2 * resolution.y; i < mip_cnt; i++)
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
			//upsampling();
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
		{
			"hdr_shader",
			"screenshader.vs",
			"hdrshader.frag"
		},
		{
			"downsampling",
			"",
			"",
			"",
			"downsampling.comp"
		},
		{
			"upsampling",
			"screenshader.vs",
			"upsampling.frag",
		}
	};

	MaterialManager::instance()->loadProgram(desc[0],false);
	MaterialManager::instance()->loadProgram(desc[1],false);
	MaterialManager::instance()->loadProgram(desc[2],false);

	m_ScreenShader = MaterialManager::instance()->getProgram("hdr_shader");
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(0,"scene");
	m_ScreenShader->setUniformValue(1,"bloomBlur");
	m_ScreenShader->unuse();

	m_DownsampleShader = MaterialManager::instance()->getProgram("downsampling");
	m_DownsampleShader->use();
	m_DownsampleShader->setUniformValue(0,"image");
	m_DownsampleShader->unuse();

	m_UpsampleShader = MaterialManager::instance()->getProgram("upsampling");

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
  offset =					CREATE_CONSOLE_VAR("offset", -3.0f, 0, "Enable/disable blur for bloom");
  useBoxFilter =		CREATE_CONSOLE_VAR("bf", 0, 0, "Enable/disable BoxFilter in bloom");
  defaultFilter =		CREATE_CONSOLE_VAR("df", 1, 0, "Enable/disable default filtering in bloom");
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
	// 2. blur bright fragments with two-pass Gaussian Blur 
	// --------------------------------------------------

	m_DownsampleShader->use();
	{
		unsigned int image_unit = 2;
		glBindImageTexture(image_unit, hdrBuffer->texture[1], 0, false, 0, GL_READ_ONLY, GL_RGBA16F);
		m_DownsampleShader->setUniformValue(2, "inputImg1");
	}

	{
		unsigned int image_unit = 3;
		glBindImageTexture(image_unit, pass1[0]->texture[0], 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
		m_DownsampleShader->setUniformValue(3, "inputImg2");
	}

	auto render = GetISystem()->GetIRender();
	m_DownsampleShader->dispatch(render->GetWidth() / 6, render->GetHeight() / 6, 1, GL_SHADER_STORAGE_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
	return;
	bool horizontal = true, first_iteration = true;
	unsigned int amount = PASSES;
	m_DownsampleShader->use();
  m_DownsampleShader->setUniformValue(useBoxFilter->GetIVal(), "use_box_filter");
  m_DownsampleShader->setUniformValue(defaultFilter->GetIVal(), "default_filter");
  m_DownsampleShader->setUniformValue(offset->GetFVal(), "offset");
	glCheck(glDisable(GL_DEPTH_TEST));
	if (useBoxFilter->GetIVal())
		amount = 1;
	else
		amount = getMips({ pass0[0]->viewPort.z, pass0[0]->viewPort.w });
	for (unsigned int i = 0; i < amount - 1; i++)
	{
		pass0[i + 1]->bind();
		//pass0[i + 1]->bind(pass0[i + 1]->viewPort / 2.f);
		m_DownsampleShader->bindTextureUnit2D(first_iteration ? hdrBuffer->texture[1] : pass0[i]->texture[0], IMAGE);
		//renderQuad();
		m_ScreenQuad.draw();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	pingpong = !horizontal;
	GetISystem()->GetIRender()->SetState(IRender::State::DEPTH_TEST, true);
}

void HdrTechnique::upsampling()
{
	m_UpsampleShader->use();
	m_UpsampleShader->setUniformValue(blurOn->GetIVal(), "blurOn");
	
	uint32_t amount;
	bool first_iteration = true;
	glCheck(glDisable(GL_DEPTH_TEST));

	amount = getMips({ pass0[0]->viewPort.z, pass0[0]->viewPort.w });
	for (unsigned int i = amount - 1; i > 0; i--)
	{
		pass1[i - 1]->bind();
		//pass1[i - 1]->bind(pass1[i - 1]->viewPort / 2.f);
		m_UpsampleShader->bindTexture2D(first_iteration ? pass0[amount - 1]->texture[0] : pass1[i]->texture[0], PREVIOS, "previos");
		m_UpsampleShader->bindTexture2D(first_iteration ? pass0[amount - 1]->texture[0] : pass0[i - 1]->texture[0], CURRENT, "current");
		m_ScreenQuad.draw();
		if (first_iteration)
			first_iteration = false;
	}
}

void HdrTechnique::Do(unsigned int texture)
{
	glm::vec3 fog = glm::vec3(
		GetISystem()->GetIConsole()->GetCVar("fogR")->GetFVal(),
		GetISystem()->GetIConsole()->GetCVar("fogG")->GetFVal(),
		GetISystem()->GetIConsole()->GetCVar("fogB")->GetFVal());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(fog.r, fog.g, fog.b, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	m_ScreenShader->use();
  m_ScreenShader->setUniformValue(exposure->GetFVal(), "exposure");
  m_ScreenShader->setUniformValue(bloom_exposure->GetFVal(), "bloom_exposure");
	glDisable(GL_DEPTH_TEST);

	m_ScreenShader->bindTexture2D(hdrBuffer->texture[0], 0, "scene");
	m_ScreenShader->bindTexture2D(pass1[0]->texture[0], 1, "bloomBlur");
	m_ScreenShader->setUniformValue(bloom->GetIVal(), "bloom");

	glCheck(glViewport(0, 0, GetISystem()->GetIRender()->GetWidth(), GetISystem()->GetIRender()->GetHeight()));
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
