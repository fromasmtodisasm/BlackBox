#include "pch.hpp"
#include "Shader.hpp"
#include "Renderer.h"
#include <BlackBox/Core/Utils.hpp>
#include <filesystem>
#include <fstream>
#include <string_view>

#include <d3dcompiler.h>

#include <d3d9.h>


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

CShader& CShader::operator=(const CShader& src)
{
	//uint32 i;

	//mfFree();

	int   Offs = (int)(INT_PTR) & (((CShader*)0)->m_NumRefs);
	byte* d    = (byte*)this;
	byte* s    = (byte*)&src;
	memcpy(&d[Offs], &s[Offs], sizeof(CShader) - Offs);

	m_NameShader = src.m_NameShader;
	m_NameFile   = src.m_NameFile;
	//m_NameShaderICRC = src.m_NameShaderICRC;

#if 0
	if (src.m_HWTechniques.Num())
	{
		m_HWTechniques.Create(src.m_HWTechniques.Num());
		for (i = 0; i < src.m_HWTechniques.Num(); i++)
		{
			m_HWTechniques[i]			= new SShaderTechnique(this);
			*m_HWTechniques[i]			= *src.m_HWTechniques[i];
			m_HWTechniques[i]->m_shader = this; // copy operator will override m_shader
		}
	}
#endif

	return *this;
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
	if (m_NumRefs <= 0 || bForce)
		delete this;
}

IShader::Type CShader::GetType()
{
	return IShader::Type();
}

const char* CShader::GetName()
{
	return m_NameShader.c_str();
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
	return m_Flags2;
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
	string   path = PathUtil::Make("%engineroot%/bin/shadercache", (string(name) + ".fxb").c_str());
	CCryFile file(path.c_str(), "wb");
	auto     Save = [&](CHWShader* pShader)->bool {
		if (auto code = pShader->m_ByteCode; code->GetBufferPointer())
		{
            file.Write((void*)&pShader->m_Type, sizeof pShader->m_Type);
			auto size = code->GetBufferSize();
            file.Write((void*)&size, sizeof size);
            file.Write(code->GetBufferPointer(), code->GetBufferSize());
			return true;
		}
		return false;
    };
	if (m_Shaders[Type::E_VERTEX])
	{
		if (!Save(m_Shaders[Type::E_VERTEX]))
		{
			return;
		}
	}
	if (m_Shaders[Type::E_FRAGMENT])
	{
		if (!Save(m_Shaders[Type::E_VERTEX]))
		{
			return;
		}
	}
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
	HRESULT                               hr{};

	auto                                  pVSBuf = (ID3DBlob*)m_Shaders[IShader::Type::E_VERTEX]->m_ByteCode;
	std::vector<D3D11_INPUT_ELEMENT_DESC> t_InputElementDescVec; // actually does not matter what store
	unsigned int                          t_ByteOffset = 0;

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
		t_InputElementDesc.SemanticName         = SP_DESC.SemanticName;
		t_InputElementDesc.SemanticIndex        = SP_DESC.SemanticIndex;
		t_InputElementDesc.InputSlot            = 0;
		t_InputElementDesc.AlignedByteOffset    = t_ByteOffset;
		t_InputElementDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
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
			uint offset  = 16;

			if (!strcmp(SP_DESC.SemanticName, "COLOR"))
			{
				offset  = 4;
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

	auto    pVSBuf = (ID3DBlob*)m_Shaders[IShader::Type::E_VERTEX]->m_ByteCode;
	//ID3D11ShaderReflection* pIShaderReflection1 = NULL;
	hr             = D3DReflect((void*)pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_pReflection);
	if (m_pReflection)
	{
		m_pReflection->GetDesc(&m_Desc);
	}
}

#pragma optimize("", off)
bool CShader::LoadFromEffect(CShader* pSH, FxEffect* pEffect, int nTechnique, int nPass)
{
	auto                tech = pEffect->GetTechnique(nTechnique);
	auto                pass = tech->GetPass(nPass);
	std::vector<string> code{pEffect->GetCode()};

	const auto& 		passStates = pass->RenderStates;

	IShader::Type       Types[] = {
        IShader::Type::E_VERTEX,
        IShader::Type::E_GEOMETRY,
        IShader::Type::E_FRAGMENT,
    };
	for (auto st : Types)
	{
		auto entry = pass->EntryPoints[st];
		if (entry.empty())
			continue;
		if (auto res = LoadFromMemory(code, st, entry.data()); res.first)
		{
			auto shader        = new CHWShader(st, entry);
			shader->m_ByteCode = res.first;
			if (shader->Upload(shader->m_ByteCode, pSH))
			{
				pSH->m_Shaders[st] = shader;
			}
			else
			{
				#if 0
				res.second->Release();
				#endif
				delete shader;
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc = D3D11_DEPTH_STENCIL_DESC {
		.DepthEnable = true,
		.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		.DepthFunc = D3D11_COMPARISON_LESS,
		.StencilEnable = false,
		.StencilReadMask = 0xFF,
		.StencilWriteMask = 0xFF,
		.FrontFace = D3D11_DEPTH_STENCILOP_DESC {
			.StencilFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilDepthFailOp = D3D11_STENCIL_OP_INCR,
			.StencilPassOp = D3D11_STENCIL_OP_KEEP,
			.StencilFunc = D3D11_COMPARISON_ALWAYS,
		},
		.BackFace = D3D11_DEPTH_STENCILOP_DESC {
			.StencilFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilDepthFailOp = D3D11_STENCIL_OP_DECR,
			.StencilPassOp = D3D11_STENCIL_OP_KEEP,
			.StencilFunc = D3D11_COMPARISON_ALWAYS,
		},
	};
	D3D11_RASTERIZER_DESC     rsDesc = D3D11_RASTERIZER_DESC {
		.FillMode = D3D11_FILL_SOLID,
		.CullMode = D3D11_CULL_BACK,
		.FrontCounterClockwise = false,
		.DepthBias = 0,
		.DepthBiasClamp = 0.0f,
		.SlopeScaledDepthBias = 0.0f,
		.DepthClipEnable = true,
		.ScissorEnable = false,
		.MultisampleEnable = false,
		.AntialiasedLineEnable = false,
	};
	D3D11_BLEND_DESC          bsDesc = D3D11_BLEND_DESC {
		.AlphaToCoverageEnable = false,
		.IndependentBlendEnable = false,
		.RenderTarget = {
			{
				.BlendEnable = false,
				.SrcBlend = D3D11_BLEND_ONE,
				.DestBlend = D3D11_BLEND_ZERO,
				.BlendOp = D3D11_BLEND_OP_ADD,
				.SrcBlendAlpha = D3D11_BLEND_ONE,
				.DestBlendAlpha = D3D11_BLEND_ZERO,
				.BlendOpAlpha = D3D11_BLEND_OP_ADD,
				.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL,
			},
		},
	};
	D3D11_SAMPLER_DESC		ssDesc = { 0 };


	for (auto& s : passStates)
	{
		switch (s.Type)
		{
		case fx::ERenderState::ZENABLE:
		{
			dsDesc.DepthEnable = s.b;
			break;
		}
		case fx::ERenderState::ZWRITEENABLE:
		{
			dsDesc.DepthWriteMask = s.b ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			break;
		}
		case fx::ERenderState::ZFUNC:
		{
			dsDesc.DepthFunc = (D3D11_COMPARISON_FUNC)s.i;
			break;
		}
		case fx::ERenderState::ALPHABLENDENABLE:
		{
			bsDesc.RenderTarget[0].BlendEnable = s.b;
			break;
		}
		case fx::ERenderState::SRCBLEND:
		{
			bsDesc.RenderTarget[0].SrcBlend = (D3D11_BLEND)s.i;
			break;
		}
		case fx::ERenderState::DESTBLEND:
		{
			bsDesc.RenderTarget[0].DestBlend = (D3D11_BLEND)s.i;
			break;
		}
		case fx::ERenderState::BLENDOP:
		{
			bsDesc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)s.i;
			break;
		}

		case fx::ERenderState::ALPHABLEND_SRC:
		{
			bsDesc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)s.i;
			break;
		}
		case fx::ERenderState::ALPHABLEND_DST:
		{
			bsDesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)s.i;
			break;
		}
		case fx::ERenderState::ALPHABLEND_EQUATION:
		{
			bsDesc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)s.i;
			break;
		}
		default:
			break;
		}
	}

	//if (dsDesc.DepthEnable)
	{
		if (auto pDevice = GetDevice(); pDevice)
		{
			pDevice->CreateDepthStencilState(&dsDesc, &pSH->m_pDepthStencilState);
			//pDevice->CreateBlendState(&bsDesc, &pSH->m_pBlendState);

			if (bsDesc.IndependentBlendEnable)
			{
				for (auto& target: bsDesc.RenderTarget)
				{
					if (target.BlendEnable)
					{
						pDevice->CreateBlendState(&bsDesc, &pSH->m_pBlendState);
					}
				}
			}
			else if (bsDesc.RenderTarget[0].BlendEnable)
			{
					pDevice->CreateBlendState(&bsDesc, &pSH->m_pBlendState);
			}
		}
	}

	//SaveHlslToDisk(code, type);
	return true;
}

std::pair<ID3DBlob*, ID3DBlob*> CShader::LoadFromMemory(const std::vector<std::string>& text, IShader::Type type, const char* pEntry)
{
	std::string code;
	for (const auto& piece : text)
		code += piece;
	return CShader::Load(code, type, pEntry, true);
}

#if 0
CHWShader* CShader::LoadFromFile(const std::string_view file, IShader::Type type, const char* pEntry)
{
	return CShader::Load(file, type, pEntry, false);
}
#endif

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
	string                                  stage(GetGLSLANGTargetName(type));
	std::ofstream                           output_file(string("bin/shadercache/shader_") + stage + ".hlsl");
	std::ostream_iterator<std::string_view> output_iterator(output_file, "\n");
	std::copy(code.begin(), code.end(), output_iterator);
	output_file.close();
}

std::pair<ID3DBlob*, ID3DBlob*> CShader::Load(const std::string_view text, IShader::Type type, const char* pEntry, bool bFromMemory)
{
	const char* profile = type == Type::E_VERTEX ? "vs_5_0" : type == Type::E_GEOMETRY ? "gs_5_0"
	                                                                                   : "ps_5_0";
	ID3DBlob*   pShaderBlob{};
	ID3DBlob*   pErrorBlob{};
	const char* code   = bFromMemory ? text.data() : nullptr;
	const char* file   = bFromMemory ? nullptr : text.data();
	auto        size   = text.size();

	//auto        nFlags = 0;	//D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
	auto        nFlags = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
	auto        hr     = D3DCompile(
	               code,
	               size,
	               file,
	               nullptr,
	               nullptr,
	               pEntry,
	               profile,
	               nFlags,
	               0, //flags2
	               &pShaderBlob,
	               &pErrorBlob);
	if (FAILED(hr))
	{
#if 1
		auto pBlob = _smart_ptr(pShaderBlob);
		auto error = _smart_ptr(pErrorBlob);
		if (pErrorBlob && pErrorBlob->GetBufferPointer())
		{
			auto log      = std::string_view((const char*)pErrorBlob->GetBufferPointer());
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
			CryError("%*.*s", text.length(), text.length(), text.data());
		}
#endif
		return std::make_pair(nullptr, pErrorBlob);
	}
	else
	{
#if 0
		void* pShaderReflBuf;
		UINT* pData = (UINT*)pShaderBlob->GetBufferPointer();
		UINT  nSize = (uint32)pShaderBlob->GetBufferSize();
		hr			= D3DReflection(pData, nSize, IID_D3DShaderReflection, &pShaderReflBuf);
		if (SUCCEEDED(hr))
		{
			D3DShaderReflection* pShaderReflection = (D3DShaderReflection*)pShaderReflBuf;
			*ppConstantTable					   = (void*)pShaderReflection;
		}
		else
		{
			assert(0);
		}
#endif
	}

	return std::make_pair(pShaderBlob, pErrorBlob);
	//return new CHWShader(pShader, pBlob, type);
}

bool CHWShader::Upload(ID3DBlob* pBlob, CShader* pSH)
{
	HRESULT   hr;
	auto      pBuf  = (DWORD*)pBlob->GetBufferPointer();
	auto      nSize = pBlob->GetBufferSize();

	IUnknown* handle{};

	switch (m_Type)
	{
	case IShader::E_VERTEX:
		hr = (handle = GetDeviceObjectFactory().CreateVertexShader(pBuf, nSize)) ? S_OK : E_FAIL;
		break;
	case IShader::E_GEOMETRY:
		hr = (handle = GetDeviceObjectFactory().CreateGeometryShader(pBuf, nSize)) ? S_OK : E_FAIL;
		break;
	case IShader::E_FRAGMENT:
		hr = (handle = GetDeviceObjectFactory().CreatePixelShader(pBuf, nSize)) ? S_OK : E_FAIL;
		break;
	case IShader::E_NUM:
		break;
	default:
		break;
	}

	if (SUCCEEDED(hr))
	{
		m_D3DShader = (ID3D11Resource*)handle;
		// Assign name to Shader for enhanced debugging
#if !defined(RELEASE) && (BB_PLATFORM_WINDOWS)
		char name[1024];
		sprintf(name, "%s_%s", pSH->GetName(), m_EntryFunc.c_str());

	#if BB_PLATFORM_WINDOWS
		#if BB_RENDERER_DIRECT3D
		auto pObject = (ID3D11DeviceChild*)m_D3DShader;
		::SetDebugName(pObject, name);
		#elif BB_RENDERER_VULKAN
		auto pObject = reinterpret_cast<NCryVulkan::CShader*>(pInst->m_Handle.m_pShader->GetHandle());
		SetDebugName(pObject, name);
		#endif
	#endif
#endif
	}
	return (hr == S_OK);
}
