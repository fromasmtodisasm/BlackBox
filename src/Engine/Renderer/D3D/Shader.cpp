#include "Shader.hpp"
#include "Renderer.h"
#include <fstream>
#include <BlackBox/Core/Utils.hpp>
#include <filesystem>
#include <string_view>

#include <d3dcompiler.h>

namespace fs = std::filesystem;

CShader::~CShader()
{
	for (auto s : m_Shaders)
	{
		if (s)
		{
			auto sh = s->m_D3DShader;
			SAFE_RELEASE(sh);
		}
	}
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

#if 0
eVertexFormat CShader::GetVertexFormat(void)
{
	return eVertexFormat();
}
#endif

bool CShader::Reload()
{
	return false;
}

void CShader::Bind()
{
	auto d = GetDevice();

	if (auto s = m_Shaders[E_VERTEX]->m_D3DShader; s) { d->VSSetShader((PVertexShader)s); }
	//if (auto s = m_Shaders[E_GEOMETRY]->m_D3DShader; s) { d->VSSetShader((PVertexShader)s); }
	if (auto s = m_Shaders[E_FRAGMENT]->m_D3DShader; s) { d->PSSetShader((PPixelShader)s); }
}

void CShader::SaveBinaryShader(std::string_view name, int flags, uint64 nMaskGen)
{
}

CShader* CShader::LoadBinaryShader(std::string_view name, int flags, uint64 nMaskGen)
{
	return nullptr;
}

CHWShader* CShader::LoadFromEffect(PEffect pEffect, IShader::Type type, int nTechnique, int nPass)
{
	assert(nTechnique <= (pEffect->GetNumTechniques() - 1));
	auto tech = pEffect->GetTechnique(nTechnique);
	assert(nPass <= (tech->GetNumPasses() - 1));
	nTechnique = nPass = 0;
	auto pass = tech->GetPass(nPass);
	auto code = pass->Code;

	if (type == IShader::E_VERTEX)
	{
		for (auto& in : pass->InputLayout)
			code.push_back(in);
	}

	auto entry = pass->EntryPoints[type].data();
	//SaveHlslToDisk(code, type);
	return LoadFromMemory(code, type, entry);
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

void SaveHlslToDisk(const std::vector<std::string>& code, IShader::Type type)
{
	string stage(GetGLSLANGTargetName(type));
	std::ofstream output_file(string("bin/shadercache/shader_") + stage + ".hlsl");
	std::ostream_iterator<std::string_view> output_iterator(output_file, "\n");
	std::copy(code.begin(), code.end(), output_iterator);
	output_file.close();
}

CHWShader* CShader::Load(const std::string_view text, IShader::Type type, const char* pEntry, bool bFromMemory)
{
	const char* profile = type == Type::E_VERTEX ? "vs_4_0" : type == Type::E_GEOMETRY ? "gs_4_0" : "ps_4_0";
	ID3DBlob* pShaderBlob;
	ID3DBlob* pErrorBlob;
	const char* code = bFromMemory ? text.data() : nullptr;
	const char* file = bFromMemory ? nullptr : text.data();
	auto size		 = text.size();

	auto flags1 = D3DCOMPILE_DEBUG;
	auto hr = D3DCompile(
		code,
		size,
		file,
		nullptr,
		nullptr,
		pEntry,
		profile,
		flags1,
		0,
		&pShaderBlob,
		&pErrorBlob);
	auto pBlob = _smart_ptr(pShaderBlob);
	auto error	= _smart_ptr(pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob && pErrorBlob->GetBufferPointer())
		{
			auto log = std::string_view((const char*)pErrorBlob->GetBufferPointer());
			auto severity = VALIDATOR_WARNING;
			if (auto pos = log.find("warning"))
			{
				severity = VALIDATOR_WARNING;
			}
			else
			{
				severity = VALIDATOR_ERROR_DBGBRK;
			}
			CryWarning(VALIDATOR_MODULE_RENDERER, severity, "Error and warning from compilation:\n%s", log.data());
		}
		//char tmp[256];
		//sprintf(tmp, "The FX file <%s> cannot be located.  Please run this executable from the directory that contains the FX file.", file ? file : "MemoryStream");
		//MessageBox(NULL, tmp, "Error", MB_OK);
		return nullptr;
	}

	#if 0
	union
	{
		ID3D10VertexShader* pVertexShader;
		ID3D10GeometryShader* pGeometryShader;
		ID3D10PixelShader* pPixelShader;
	};
	#else
	ID3D10Resource* pShader;
	#endif

	//pVertexShader = nullptr;

	// Create the vertex shader
	if (type == E_VERTEX)
		hr = GetDevice()->CreateVertexShader((DWORD*)pBlob->GetBufferPointer(),
											 pBlob->GetBufferSize(), (ID3D10VertexShader**)&pShader);
	else if (type == E_GEOMETRY)
		hr = GetDevice()->CreateGeometryShader((DWORD*)pBlob->GetBufferPointer(),
											 pBlob->GetBufferSize(), (ID3D10GeometryShader**)&pShader);
	else if (type == E_FRAGMENT)
		hr = GetDevice()->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(),
											 pBlob->GetBufferSize(), (ID3D10PixelShader**)&pShader);

	return new CHWShader(pShader, pBlob, type);
}
