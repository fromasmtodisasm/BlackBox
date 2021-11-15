#pragma once

//////////////////////////////////////////////////////////////////////
class CRenderer;
extern CRenderer* gRenDev;

class CShader;

struct GlobalResources
{
	static ID3DShaderResourceView* FontAtlasRV;

	static ID3DShaderResourceView* WiteTextureRV;
	static ID3DShaderResourceView* GreyTextureRV;

	static ID3D11SamplerState* LinearSampler;

	static ID3D10EffectTechnique* BoxTechnique;
	static ID3D10EffectTechnique* MeshTechnique;

	static ID3DInputLayout* VERTEX_FORMAT_P3F_C4B_T2F_Layout;

	static _smart_ptr<CShader> SpriteShader;
	static _smart_ptr<CShader> TexturedQuadShader;

	//static _smart_ptr<CShader> GrayScale




	static ID3D11BlendState* FontBlendState;

};

namespace DeviceFormats
{
	//---------------------------------------------------------------------------------------------------------------------
	inline UINT GetStride(D3DFormat format)
	{
		switch (format)
		{
		default:
		case DXGI_FORMAT_UNKNOWN:
			return 0;
			break;

		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_SINT:
			return 4 * sizeof(float);
			break;

		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT:
		case DXGI_FORMAT_R32G32B32_UINT:
		case DXGI_FORMAT_R32G32B32_SINT:
			return 3 * sizeof(float);
			break;

		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_UNORM:
		case DXGI_FORMAT_R16G16B16A16_UINT:
		case DXGI_FORMAT_R16G16B16A16_SNORM:
		case DXGI_FORMAT_R16G16B16A16_SINT:
			return 4 * sizeof(short);
			break;

		case DXGI_FORMAT_R32G32_TYPELESS:
		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT:
		case DXGI_FORMAT_R32G32_SINT:
			return 2 * sizeof(float);
			break;

		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return 2 * sizeof(float);
			break;

		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R11G11B10_FLOAT:
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_SINT:
			return 4 * sizeof(char);
			break;

		case DXGI_FORMAT_R16G16_TYPELESS:
		case DXGI_FORMAT_R16G16_FLOAT:
		case DXGI_FORMAT_R16G16_UNORM:
		case DXGI_FORMAT_R16G16_UINT:
		case DXGI_FORMAT_R16G16_SNORM:
		case DXGI_FORMAT_R16G16_SINT:
			return 2 * sizeof(short);
			break;

		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_SINT:
			return 1 * sizeof(float);
			break;

	#if CRY_RENDERER_VULKAN
		case DXGI_FORMAT_D16_UNORM_S8_UINT:
		case DXGI_FORMAT_R16G8X8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM:
		case DXGI_FORMAT_R24X8_TYPELESS:
	#endif
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return 4 * sizeof(char);
			break;

		case DXGI_FORMAT_R8G8_TYPELESS:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_SINT:
			return 2 * sizeof(char);
			break;

		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_SINT:
			return 1 * sizeof(short);
			break;

		case DXGI_FORMAT_B5G6R5_UNORM:
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return 1 * sizeof(short);
			break;

		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
	#if CRY_RENDERER_VULKAN
		case DXGI_FORMAT_S8_UINT:
	#endif
		case DXGI_FORMAT_A8_UNORM:
			return 1 * sizeof(char);
			break;

		case DXGI_FORMAT_R1_UNORM:
			return 1 * sizeof(char);
			break;
		}

		CRY_ASSERT(0);
		return 0;
	}

}


////////////////////////////////////////////////////////////////////////////
// InputLayout API

struct SShaderBlob
{
	void* m_pShaderData;
	int   m_nDataSize;
};

struct SInputLayout
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_Declaration;			 // Configuration
	uint16                                m_firstSlot;
	std::vector<uint16>                   m_Strides;				 // Stride of each input slot, starting from m_firstSlot
	std::array<int8, 4>                   m_Offsets;				 // The offsets of "POSITION", "COLOR", "TEXCOORD" and "NORMAL"

	enum
	{
		eOffset_Position,
		eOffset_Color,
		eOffset_TexCoord,
		eOffset_Normal,
	};

	SInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> &&decs) : m_Declaration(std::move(decs))
	{
		// Calculate first slot index
		m_firstSlot = std::numeric_limits<uint16>::max();
		for (const auto &dec : m_Declaration)
			m_firstSlot = std::min(m_firstSlot, static_cast<uint16>(dec.InputSlot));

		// Calculate strides
		for (const auto &dec : m_Declaration)
		{
			const uint16 slot = dec.InputSlot - m_firstSlot;
			if (m_Strides.size() <= slot)
				m_Strides.resize(slot + 1, 0);

			m_Strides[slot] = std::max(m_Strides[slot], uint16(dec.AlignedByteOffset + DeviceFormats::GetStride(dec.Format)));
		}

		// Calculate offsets
		m_Offsets[eOffset_Position] = m_Offsets[eOffset_Color] = m_Offsets[eOffset_TexCoord] = m_Offsets[eOffset_Normal] = -1;
		for (int n = 0; n < m_Declaration.size(); ++n)
		{
			if (!m_Declaration[n].SemanticName)
				continue;

			if ((m_Offsets[eOffset_Position] == -1) && (!stricmp(m_Declaration[n].SemanticName, "POSITION")))
				m_Offsets[eOffset_Position] = m_Declaration[n].AlignedByteOffset;
			if ((m_Offsets[eOffset_Color] == -1) && (!stricmp(m_Declaration[n].SemanticName, "COLOR")))
				m_Offsets[eOffset_Color] = m_Declaration[n].AlignedByteOffset;
			if ((m_Offsets[eOffset_TexCoord] == -1) && (!stricmp(m_Declaration[n].SemanticName, "TEXCOORD")))
				m_Offsets[eOffset_TexCoord] = m_Declaration[n].AlignedByteOffset;
			if ((m_Offsets[eOffset_Normal] == -1) && (!stricmp(m_Declaration[n].SemanticName, "NORMAL") || !stricmp(m_Declaration[n].SemanticName, "TANGENT")))
				m_Offsets[eOffset_Normal] = m_Declaration[n].AlignedByteOffset;
		}
	}

	SInputLayout(const SInputLayout& src) = default;
	SInputLayout(SInputLayout&& src) = default;
	SInputLayout& operator=(const SInputLayout& src) = default;
	SInputLayout& operator=(SInputLayout&& src) = default;

	bool operator==(const std::vector<D3D11_INPUT_ELEMENT_DESC>& descs) const
	{
		size_t count = m_Declaration.size();
		if (count != descs.size())
		{
			return false;
		}

		for (size_t i = 0; i < count; ++i)
		{
			const D3D11_INPUT_ELEMENT_DESC& desc0 = m_Declaration[i];
			const D3D11_INPUT_ELEMENT_DESC& desc1 = descs[i];

			if (0 != stricmp(desc0.SemanticName, desc1.SemanticName) ||
				desc0.SemanticIndex != desc1.SemanticIndex ||
				desc0.Format != desc1.Format ||
				desc0.InputSlot != desc1.InputSlot ||
				desc0.AlignedByteOffset != desc1.AlignedByteOffset ||
				desc0.InputSlotClass != desc1.InputSlotClass ||
				desc0.InstanceDataStepRate != desc1.InstanceDataStepRate)
			{
				return false;
			}
		}
		return true;
	}
};


