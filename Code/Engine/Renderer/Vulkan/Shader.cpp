#include "Shader.hpp"
#include "Renderer.h"
#include <BlackBox/Core/Utils.hpp>
#include <fstream>

#include <shaderc/shaderc.hpp>

static string SaveHlslToDisk(std::string_view name, const std::vector<std::string>& code, IShader::Type type);

CShader::~CShader()
{
#if 0
	for (auto s : m_Shaders)
	{
		if (s)
		{
			auto sh = s->m_D3DShader;
			SAFE_RELEASE(sh);
		}
	}
#endif
}

int CShader::GetID()
{
	return 0;
}

void CShader::AddRef()
{
	m_NumRefs++;
}

int CShader::Release()
{
	m_NumRefs--;
	auto refs = m_NumRefs;
	if (m_NumRefs <= 0)
		delete this;
	return refs;
}

IShader::Type CShader::GetType()
{
	return IShader::Type();
}

const char* CShader::GetName()
{
	return nullptr;
}

eVertexFormat CShader::GetVertexFormat(void)
{
	return eVertexFormat();
}

bool CShader::Reload()
{
	return false;
}

void CShader::Bind()
{
	auto d = GetDevice();

#if 0
	if (auto s = m_Shaders[E_VERTEX]->m_D3DShader; s) { d->VSSetShader((PVertexShader)s); }
	//if (auto s = m_Shaders[E_GEOMETRY]->m_D3DShader; s) { d->VSSetShader((PVertexShader)s); }
	if (auto s = m_Shaders[E_FRAGMENT]->m_D3DShader; s) { d->PSSetShader((PPixelShader)s); }
#endif
}

void CShader::SaveBinaryShader(std::string_view name, int flags, uint64 nMaskGen)
{
}

CShader* CShader::LoadBinaryShader(std::string_view name, int flags, uint64 nMaskGen)
{
	return nullptr;
}

CHWShader* CShader::LoadFromEffect(PEffect pEffect, IShader::Type type)
{
	auto tech = pEffect->GetTechnique(0);
	auto pass = tech->GetPass(0);
	auto code = pass->Code;

	if (type == IShader::E_VERTEX)
	{
		for (auto& in : pass->InputLayout)
			code.push_back(in);
	}

	auto entry = pass->EntryPoints[type].data();
	//return LoadFromMemory(code, type, entry);
	auto effectName = pEffect->GetName();
	SaveHlslToDisk(effectName, code, type);
	return LoadFromFile(effectName, type, entry);
}

CHWShader* CShader::LoadFromMemory(const std::vector<std::string>& text, IShader::Type type, const char* pEntry)
{
	std::string code;
	for (const auto& piece : text)
		code += piece;
	return CShader::Load(code, type, pEntry, true);
}

CHWShader* CShader::LoadFromFile(const std::string_view file, IShader::Type type, const char* pEntry)
{
	return CShader::Load(file, type, pEntry, false);
}

const char* GetGLSLANGTargetName(IShader::Type target)
{
	switch (target)
	{
	case IShader::E_VERTEX:
		return "vert";

	case IShader::E_GEOMETRY:
		return "geom";

	case IShader::E_FRAGMENT:
		return "frag";

	default:
		return "1111";
	}
}

void DumpWarning(const char* str)
{
	CryLogAlways("[Vk Compiler] Warning: %s", str);
	fwrite(str, sizeof(char), strlen(str), stderr);
}

void DumpError(const char* str)
{
	CRY_ASSERT(false, "[Vk Compiler] ERROR: %s", str);
	CryLogAlways("[Vk Compiler] ERROR: %s", str);
	fwrite(str, sizeof(char), strlen(str), stderr);
}

bool ShellExecute(const std::string& file, const std::string& parameters, const std::string& workingDir = "")
{
#ifdef WIN32
	HANDLE childStdInRead;
	HANDLE childStdInWrite;
	HANDLE childStdOutRead;
	HANDLE childStdOutWrite;

	SECURITY_ATTRIBUTES securityAttributes;
	securityAttributes.nLength				= sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle		= true;
	securityAttributes.lpSecurityDescriptor = nullptr;

	CreatePipe(&childStdInRead, &childStdInWrite, &securityAttributes, 0);
	SetHandleInformation(childStdInWrite, HANDLE_FLAG_INHERIT, 0);

	CreatePipe(&childStdOutRead, &childStdOutWrite, &securityAttributes, 0);
	SetHandleInformation(childStdOutRead, HANDLE_FLAG_INHERIT, 0);

	PROCESS_INFORMATION processInfo = {0};
	STARTUPINFO startUpInfo			= {0};

	startUpInfo.cb		   = sizeof(STARTUPINFO);
	startUpInfo.hStdInput  = childStdInRead;
	startUpInfo.hStdOutput = childStdOutWrite;
	startUpInfo.hStdError  = childStdOutWrite;
	startUpInfo.dwFlags |= STARTF_USESTDHANDLES;

	char parametersTemp[4096];
	sprintf_s(parametersTemp, 4096, "%s %s", file.c_str(), parameters.c_str());

	std::string output;

	CreateProcess(nullptr, parametersTemp, nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, workingDir.size() == 0 ? nullptr : workingDir.c_str(), &startUpInfo, &processInfo);
	while (WaitForSingleObject(processInfo.hProcess, 100) == WAIT_TIMEOUT)
	{
		unsigned long size = GetFileSize(childStdOutRead, nullptr);

		if (size != 0)
		{
			std::string outputTemp;
			outputTemp.resize(size);

			unsigned long readBytes;
			ReadFile(childStdOutRead, &outputTemp[0], size, &readBytes, nullptr);

			output += outputTemp;
		}
	}

	unsigned long size = GetFileSize(childStdOutRead, nullptr);

	if (size != 0)
	{
		std::string outputTemp;
		outputTemp.resize(size);

		unsigned long readBytes;
		ReadFile(childStdOutRead, &outputTemp[0], size, &readBytes, nullptr);

		output += outputTemp;
	}

	unsigned long exitCode;
	GetExitCodeProcess(processInfo.hProcess, &exitCode);

	if (exitCode != 0)
	{
		// convert 'ERROR' to 'error' as remote compiler
		// doesn't report upper case version
		size_t curPos = output.find("ERROR:", 0);
		while (curPos != std::string::npos)
		{
			output.replace(curPos, 5, "error");
			curPos = output.find("ERROR:", curPos + 1);
		}

		DumpError(output.c_str());
	}
	else
	{
		if (output.size() > 0)
			DumpWarning(output.c_str());
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	CloseHandle(childStdInRead);
	CloseHandle(childStdInWrite);
	CloseHandle(childStdOutRead);
	CloseHandle(childStdOutWrite);

	return exitCode == 0;
#else
	CryFatalError("Cannot run the process! ShellExecute is implemented ONLY for Windows Platform.");
	return false;
#endif
}

string SaveHlslToDisk(std::string_view name, const std::vector<std::string>& code, IShader::Type type)
{
	string file_name = {name.data() + string("_") + string{GetGLSLANGTargetName(type)} + ".hlsl"};
	std::ofstream output_file{string{"bin/shadercache/"} + file_name};
	std::ostream_iterator<std::string_view> output_iterator(output_file, "\n");
	std::copy(code.begin(), code.end(), output_iterator);
	output_file.close();
	return file_name;
}

#define OUTPUT_SPIRV_FORMAT ".spv"
const char* CompileToSpirv_Glslangvalidator(const char* name, const char* pEntrypoint, IShader::Type type, ShaderLangId langId, const string& stage, const char* ext, char* params);

const char* CompileToSpirv_Glslc(const char* name, const char* pEntrypoint, IShader::Type type, ShaderLangId langId, const string& stage, const char* ext, char* params);

bool CompileToSpirv(const char* name, const char* pEntrypoint, IShader::Type type, ShaderLangId langId)
{
	//else if (vkShaderCompiler == STR_VK_SHADER_COMPILER_GLSLANG)
	string stage(GetGLSLANGTargetName(type));

	//const bool needsInvertingY = strncmp(pTarget, "vs", 2) == 0 || strncmp(pTarget, "ds", 2) == 0 || strncmp(pTarget, "gs", 2) == 0;

	char params[1001];
	const char* ext = langId == ShaderLangId::Hlsl ? ".hlsl" : ".glsl";

	//CompileToSpirv_Glslangvalidator(name, pEntrypoint, type, langId, stage.data(), ext, params);
	;

	auto file	= CompileToSpirv_Glslc(name, pEntrypoint, type, langId, stage.data(), ext, params);
	auto result = ShellExecute(file, params, gEnv->pSystem->GetRootFolder());

	CryLog("compiler string: %s", params);

	return result;
}

const char* CompileToSpirv_Glslangvalidator(const char* name, const char* pEntrypoint, IShader::Type type, ShaderLangId langId, const string& stage, const char* ext, char* params)
{
	std::string targetEnv = "vulkan1.2";
	const char* extra	  = nullptr;
	extra				  = langId == ShaderLangId::Hlsl ? "-D" : "";

	sprintf(params, "%s%s_%s%s -o %s%s_%s%s -G --target-env %s -S %s -e %s %s",
			"bin/shadercache/", name, stage.data(), ext,
			"bin/shadercache/", name, stage.data(), OUTPUT_SPIRV_FORMAT,
			targetEnv.c_str(),
			stage.data(),
			pEntrypoint,
			extra);
	return "glslangValidator.exe";
}
const char* CompileToSpirv_Glslc(const char* name, const char* pEntrypoint, IShader::Type type, ShaderLangId langId, const string& stage, const char* ext, char* params)
{
	sprintf(params, "-fshader-stage=%s  -fentry-point=%s --target-env=vulkan %s%s_%s%s -o %s%s_%s%s",
			stage.data(),
			pEntrypoint,
			"bin/shadercache/", name, stage.data(), ext,
			"bin/shadercache/", name, stage.data(), OUTPUT_SPIRV_FORMAT
			);
	return "glslc";
}

CHWShader* LoadSpirvFromFile(const char* name, const char* entry, IShader::Type type)
{
	string stage(GetGLSLANGTargetName(type));
	char file[1001];
	sprintf(file, "%s%s_%s%s",
			"bin/shadercache/", name, stage.data(), OUTPUT_SPIRV_FORMAT);
	// Load binary from file
	std::ifstream inputStream(file, std::ios::binary);
	if (!inputStream.is_open())
	{
		CryError("Could not load SPIRV file: [%s]", file);
		return nullptr;
	}
	std::istreambuf_iterator<char> startIt(inputStream), endIt;

	spirv_bin buffer(startIt, endIt); // Load file
	inputStream.close();

	VkShaderModule hShader;
	{
		VkShaderModuleCreateInfo ci{};
		ci.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ci.codeSize = buffer.size();
		ci.flags	= 0;
		ci.pCode	= buffer.data();
		vkCreateShaderModule(GetDevice(), &ci, nullptr, &hShader);
	}

	return new CHWShader(hShader, std::move(buffer), type);
}

CHWShader* CShader::Load(const std::string_view text, IShader::Type type, const char* pEntry, bool bFromMemory)
{
	if (bFromMemory)
		return {};
	CompileToSpirv(text.data(), pEntry, type, ShaderLangId::Hlsl);
	return LoadSpirvFromFile(text.data(), pEntry, type);
}
