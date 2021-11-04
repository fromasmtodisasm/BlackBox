#include "Shader.hpp"
#include "Renderer.h"
#include <BlackBox/Core/Utils.hpp>
#include <filesystem>
#include <fstream>
#include <string_view>

#include <D3DX10Core.h>
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

void CShader::Release(bool bForce)
{
	m_NumRefs--;
	auto refs = m_NumRefs;
	if (m_NumRefs <= 0)
		delete this;
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

void CShader::Bind()
{
	if ((m_Flags2 & EF2_FAILED) != 0)
		return;
	auto d = GetDeviceContext();

	if (!WaitUntilLoaded())
		return;
	if (auto s = m_Shaders[E_VERTEX]->m_D3DShader; s) { d->VSSetShader((PVertexShader)s, nullptr, 0); }
	//if (auto s = m_Shaders[E_GEOMETRY]->m_D3DShader; s) { d->VSSetShader((PVertexShader)s); }
	if (auto s = m_Shaders[E_FRAGMENT]->m_D3DShader; s) { d->PSSetShader((PPixelShader)s, nullptr, 0); }

	d->IASetInputLayout(m_pInputLayout);
}

int CShader::GetFlags()
{
	return m_Flags;
}

int CShader::GetFlags2()
{
	return 0;
}

bool CShader::WaitUntilLoaded()
{
	if ((m_Flags2 & EF2_LOADED) == 0)
	{
		while ((m_Flags2 & (EF2_LOADED | EF2_FAILED)) == 0)
		{
			Sleep(1);
		}
	}
	return (m_Flags2 & EF2_FAILED) == 0;
}

void CShader::SaveBinaryShader(std::string_view name, int flags, uint64 nMaskGen)
{
}

CShader* CShader::LoadBinaryShader(std::string_view name, int flags, uint64 nMaskGen)
{
	return nullptr;
}

template<class T, size_t size>
struct PaddedStruct : public T
{
	std::array<uint8_t, size> Array;
};

void CShader::CreateInputLayout()
{
	HRESULT hr{};

	auto								  pVSBuf = (ID3DBlob*)m_Shaders[IShader::Type::E_VERTEX]->m_Bytecode;
	std::vector<D3D11_INPUT_ELEMENT_DESC> t_InputElementDescVec; // actually does not matter what store
	unsigned int						  t_ByteOffset = 0;

	for (int i = 0; i < m_Desc.InputParameters; ++i)
	{
		PaddedStruct<D3D11_SIGNATURE_PARAMETER_DESC, 100> SP_DESC;
		ZeroStruct(SP_DESC);
		m_pReflection->GetInputParameterDesc(i, &SP_DESC);

		// FIXME: temporary hack to workaround stack couruption around SP_DESC, because D3D11_INPUT_ELEMENT_DESC
		// struct has new field in last version of d3d11
		//
		// During call D3DReflect we get pointer to newer interface of relfection (d3dcompiler_47.dll)
		// but in dxsdk 2010 june version d3dcompiler_43.dll, but if linked with 43 version, d3dreflect
		// not found coresponding interface for IID_ID3D11ShaderReflection!!! it is worked, but...

		D3D11_INPUT_ELEMENT_DESC t_InputElementDesc;
		t_InputElementDesc.SemanticName			= SP_DESC.SemanticName;
		t_InputElementDesc.SemanticIndex		= SP_DESC.SemanticIndex;
		t_InputElementDesc.InputSlot			= 0;
		t_InputElementDesc.AlignedByteOffset	= t_ByteOffset;
		t_InputElementDesc.InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
		t_InputElementDesc.InstanceDataStepRate = 0;

		if (SP_DESC.Mask == 1)
		{
			if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::UInt);
				t_InputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::SInt);
				t_InputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::Float);
				t_InputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			t_ByteOffset += 4;
		}
		else if (SP_DESC.Mask <= 3)
		{
			if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::UVec2);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::SVec2);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::FVec2);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			t_ByteOffset += 8;
		}
		else if (SP_DESC.Mask <= 7)
		{
			if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::UVec3);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::SVec3);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::FVec3);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			t_ByteOffset += 12;
		}
		else if (SP_DESC.Mask <= 15)
		{
			bool isColor = false;
			uint offset	 = 16;

			if (!strcmp(SP_DESC.SemanticName, "COLOR"))
			{
				offset	= 4;
				isColor = true;
			}
			if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::UVec4);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::SVec4);
				t_InputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			}
			else if (SP_DESC.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				format.add(SP_DESC.SemanticName, DynVertexFormat::FVec4);
				t_InputElementDesc.Format = isColor ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			t_ByteOffset += offset;
		}

		t_InputElementDescVec.push_back(t_InputElementDesc);
	}
	hr = GetDevice()->CreateInputLayout(
		t_InputElementDescVec.data(),
		t_InputElementDescVec.size(),
		pVSBuf->GetBufferPointer(),
		pVSBuf->GetBufferSize(),
		&m_pInputLayout);
	if (FAILED(hr))
	{
		CryError("Error create input layout for font");
		//return false;
	}
	//SAFE_RELEASE(m_pReflection);
}

void CShader::ReflectShader()
{
	HRESULT hr{};

	auto pVSBuf = (ID3DBlob*)m_Shaders[IShader::Type::E_VERTEX]->m_Bytecode;
	//ID3D11ShaderReflection* pIShaderReflection1 = NULL;
	hr = D3DReflect((void*)pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_pReflection);
	if (m_pReflection)
	{
		m_pReflection->GetDesc(&m_Desc);
	}
}

CHWShader* CShader::LoadFromEffect(PEffect pEffect, IShader::Type type, int nTechnique, int nPass)
{
	auto				tech = pEffect->GetTechnique(nTechnique);
	auto				pass = tech->GetPass(nPass);
	std::vector<string> code{pEffect->GetCode()};

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
	string									stage(GetGLSLANGTargetName(type));
	std::ofstream							output_file(string("bin/shadercache/shader_") + stage + ".hlsl");
	std::ostream_iterator<std::string_view> output_iterator(output_file, "\n");
	std::copy(code.begin(), code.end(), output_iterator);
	output_file.close();
}

CHWShader* CShader::Load(const std::string_view text, IShader::Type type, const char* pEntry, bool bFromMemory)
{
	const char* profile = type == Type::E_VERTEX ? "vs_4_0" : type == Type::E_GEOMETRY ? "gs_4_0"
																					   : "ps_4_0";
	ID3DBlob*	pShaderBlob{};
	ID3DBlob*	pErrorBlob{};
	const char* code = bFromMemory ? text.data() : nullptr;
	const char* file = bFromMemory ? nullptr : text.data();
	auto		size = text.size();

	auto flags1 = 0;
	//D3D10_SHADER_DEBUG;
	auto hr = D3DCompile(
		code,
		size,
		file,
		nullptr,
		nullptr,
		pEntry,
		profile,
		flags1,
		0, //flags2
		&pShaderBlob,
		&pErrorBlob);
	auto pBlob = _smart_ptr(pShaderBlob);
	auto error = _smart_ptr(pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob && pErrorBlob->GetBufferPointer())
		{
			auto log	  = std::string_view((const char*)pErrorBlob->GetBufferPointer());
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
	ID3D11Resource* pShader;
#endif

	//pVertexShader = nullptr;

	// Create the vertex shader
	if (type == E_VERTEX)
		hr = GetDevice()->CreateVertexShader((DWORD*)pBlob->GetBufferPointer(),
											 pBlob->GetBufferSize(), nullptr, (ID3D11VertexShader**)&pShader);
	else if (type == E_GEOMETRY)
		hr = GetDevice()->CreateGeometryShader((DWORD*)pBlob->GetBufferPointer(),
											   pBlob->GetBufferSize(), nullptr, (ID3D11GeometryShader**)&pShader);
	else if (type == E_FRAGMENT)
		hr = GetDevice()->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(),
											pBlob->GetBufferSize(), nullptr, (ID3D11PixelShader**)&pShader);

	return new CHWShader(pShader, pBlob, type);
}
