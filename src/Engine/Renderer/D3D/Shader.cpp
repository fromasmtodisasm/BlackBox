#include "Shader.hpp"
#include "Renderer.h"
#include <fstream>
#include <BlackBox/Core/Utils.hpp>

#include <d3dcompiler.h>

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
}

int CShader::Release()
{
	return 0;
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
	
	for (int type = Type::E_VERTEX; type < 3; type++)
	{
		if (true)
		{
			switch (type)
			{
			case E_VERTEX:
				{
					VertexShader* v{};
					if (SUCCEEDED(m_Shaders[type]->m_D3DShader->QueryInterface(IID_ID3D10VertexShader, (void**)&v)))
					{
						ID3D10Device* d;
						v->GetDevice(&d);
						d->VSSetShader(v);
					}
					
				}

				break;
			case E_GEOMETRY:
				
#if 0
				VertexShader* v;
				if (SUCCEEDED(s->m_D3DShader->QueryInterface(__uuidof(ID3D10VertexShader), (void**)&v)))
				{
					ID3D10Device* d;
					v->GetDevice(&d);
					d->VSSetShader(v);
				}
#endif

				break;
			case E_FRAGMENT:
				{
					PixelShader* v;
					if (SUCCEEDED(m_Shaders[type]->m_D3DShader->QueryInterface(__uuidof(ID3D10PixelShader), (void**)&v)))
					{
						ID3D10Device* d;
						v->GetDevice(&d);
						d->PSSetShader(v);
					}
					
				}

				break;
			default:;
				assert(0);
			}
		}
	}
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
	std::ofstream output_file(string("bin/shadercache/shader_") + stage +  ".hlsl");
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
	auto shader = _smart_ptr(pShaderBlob);
	auto error	= _smart_ptr(pErrorBlob);
	if (pErrorBlob && pErrorBlob->GetBufferPointer())
	{
		CryError("Error and warning from compilation:\n%s", wstr_to_str(wstring((const wchar_t*)pErrorBlob->GetBufferPointer())));
	}
	if (FAILED(hr))
	{
		MessageBox(NULL,
				   "The FX file cannot be located.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return nullptr;
	}

	union
	{
		ID3D10VertexShader* pVertexShader;
		ID3D10GeometryShader* pGeometryShader;
		ID3D10PixelShader* pPixelShader;
	};

	pVertexShader = nullptr;

	// Create the vertex shader
	if (type == E_VERTEX)
		hr = GetDevice()->CreateVertexShader((DWORD*)shader->GetBufferPointer(),
											 shader->GetBufferSize(), &pVertexShader);
	else if (type == E_GEOMETRY)
		hr = GetDevice()->CreateGeometryShader((DWORD*)shader->GetBufferPointer(),
											 shader->GetBufferSize(), &pGeometryShader);
	else if (type == E_FRAGMENT)
		hr = GetDevice()->CreatePixelShader((DWORD*)shader->GetBufferPointer(),
											 shader->GetBufferSize(), &pPixelShader);

	return new CHWShader(pVertexShader, shader, type);
}
