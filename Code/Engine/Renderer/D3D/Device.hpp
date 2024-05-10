#pragma once
#include <Cry_Color4.h>
#include "StateManager.hpp"

#include <unordered_map>

class CD3DRenderer;
#if 0
extern CD3DRenderer gcpRendD3D;
#else
extern CD3DRenderer* gcpRendD3D;
#endif

class CD3D_FEATURE_LEVEL
{
public:
	CD3D_FEATURE_LEVEL(D3D_FEATURE_LEVEL Type)
	    : m_Type(Type)
	{
	}
	operator const char*()
	{
#define CONVERT(t) \
	case t:        \
		return #t;
		switch (m_Type)
		{
			CONVERT(D3D_FEATURE_LEVEL_9_1)
			CONVERT(D3D_FEATURE_LEVEL_9_2)
			CONVERT(D3D_FEATURE_LEVEL_9_3)
			CONVERT(D3D_FEATURE_LEVEL_10_0)
			CONVERT(D3D_FEATURE_LEVEL_10_1)
			CONVERT(D3D_FEATURE_LEVEL_11_0)
		default:
			return "Unknown";
		}
#undef CONVERT
	}
	                   operator D3D_FEATURE_LEVEL() { return m_Type; }
	                   operator D3D_FEATURE_LEVEL*() { return &m_Type; }
	D3D_FEATURE_LEVEL* operator&() { return &m_Type; }
	D3D_FEATURE_LEVEL  m_Type;
};

struct RasterizerDesc
{
	uint fillMode : 2;
	uint cullMode : 2;
	uint frontCounterClockwise : 1;
	uint DepthClipEnable : 1;
	uint ScissorEnable : 1;
	uint MultisampleEnable : 1;
	uint AntialiasedLineEnable : 1;
	uint DepthBias;
	float DepthBiasClamp;
	float SlopeScaledDepthBias;

	RasterizerDesc()
	{
		fillMode = D3D11_FILL_SOLID;
		cullMode = D3D11_CULL_BACK;
		frontCounterClockwise = false;
		DepthClipEnable = true;
		ScissorEnable = false;
		MultisampleEnable = false;
		AntialiasedLineEnable = false;
		DepthBias = 0;
		DepthBiasClamp = 0.0f;
		SlopeScaledDepthBias = 0.0f;
	}

	RasterizerDesc(const D3D11_RASTERIZER_DESC& desc)
	{
		fillMode = desc.FillMode;
		cullMode = desc.CullMode;
		frontCounterClockwise = desc.FrontCounterClockwise;
		DepthClipEnable = desc.DepthClipEnable;
		ScissorEnable = desc.ScissorEnable;
		MultisampleEnable = desc.MultisampleEnable;
		AntialiasedLineEnable = desc.AntialiasedLineEnable;
		DepthBias = desc.DepthBias;
		DepthBiasClamp = desc.DepthBiasClamp;
		SlopeScaledDepthBias = desc.SlopeScaledDepthBias;
	}

	D3D11_RASTERIZER_DESC GetDesc()
	{
		D3D11_RASTERIZER_DESC desc{};
		desc.FillMode = static_cast<D3D11_FILL_MODE>(fillMode);
		desc.CullMode = static_cast<D3D11_CULL_MODE>(cullMode);
		desc.FrontCounterClockwise = frontCounterClockwise;
		desc.DepthClipEnable = DepthClipEnable;
		desc.ScissorEnable = ScissorEnable;
		desc.MultisampleEnable = MultisampleEnable;
		desc.AntialiasedLineEnable = AntialiasedLineEnable;
		desc.DepthBias = DepthBias;
		desc.DepthBiasClamp = DepthBiasClamp;
		desc.SlopeScaledDepthBias = SlopeScaledDepthBias;
		return desc;
	}

	bool operator==(const RasterizerDesc& other) const
	{
		return memcmp(this, &other, sizeof(RasterizerDesc)) == 0;
	}

	bool operator!=(const RasterizerDesc& other) const
	{
		return !(*this == other);
	}

	size_t Hash1() const {
		int combinedBits = (fillMode << 7) |
			(cullMode << 5) |
			(frontCounterClockwise << 4) |
			(DepthClipEnable << 3) |
			(ScissorEnable << 2) |
			(MultisampleEnable << 1) |
			(AntialiasedLineEnable);

		size_t hash = 0;
		std::hash<int> hasher;
		hash ^= hasher(combinedBits) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

		std::hash<float> floatHasher;
		hash ^= floatHasher(DepthBiasClamp) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= floatHasher(SlopeScaledDepthBias) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		std::hash<int> intHasher;
		hash ^= intHasher(DepthBias) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

		return hash;
	}
	size_t HashSimple() const {
		int combinedBits = (fillMode << 7) |
			(cullMode << 5) |
			(frontCounterClockwise << 4) |
			(DepthClipEnable << 3) |
			(ScissorEnable << 2) |
			(MultisampleEnable << 1) |
			(AntialiasedLineEnable);

		size_t hash = 0;
		hash ^= std::hash<int>{}(combinedBits);
		hash ^= std::hash<float>{}(DepthBiasClamp);
		hash ^= std::hash<float>{}(SlopeScaledDepthBias);
		hash ^= std::hash<int>{}(DepthBias);

		return hash;
	}

};

struct DepsthStencilDesc
{
	uint64 DepthEnable : 1;
	uint64 DepthWriteMask : 2;
	uint64 DepthFunc : 3;
	uint64 StencilEnable : 1;
	uint64 StencilReadMask : 8;
	uint64 StencilWriteMask : 8;
	uint64 FrontFaceStencilFailOp : 4;
	uint64 FrontFaceStencilDepthFailOp : 4;
	uint64 FrontFaceStencilPassOp : 4;
	uint64 FrontFaceStencilFunc : 8;
	uint64 BackFaceStencilFailOp : 4;
	uint64 BackFaceStencilDepthFailOp : 4;
	uint64 BackFaceStencilPassOp : 4;
	uint64 BackFaceStencilFunc : 8;

	DepsthStencilDesc()
	{
		DepthEnable = true;
		DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthFunc = D3D11_COMPARISON_LESS;
		StencilEnable = false;
		StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		FrontFaceStencilFailOp = D3D11_STENCIL_OP_KEEP;
		FrontFaceStencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		FrontFaceStencilPassOp = D3D11_STENCIL_OP_KEEP;
		FrontFaceStencilFunc = D3D11_COMPARISON_ALWAYS;
		BackFaceStencilFailOp = D3D11_STENCIL_OP_KEEP;
		BackFaceStencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		BackFaceStencilPassOp = D3D11_STENCIL_OP_KEEP;
		BackFaceStencilFunc = D3D11_COMPARISON_ALWAYS;
	}

	DepsthStencilDesc(const D3D11_DEPTH_STENCIL_DESC& desc)
	{
		DepthEnable = desc.DepthEnable;
		DepthWriteMask = desc.DepthWriteMask;
		DepthFunc = desc.DepthFunc;
		StencilEnable = desc.StencilEnable;
		StencilReadMask = desc.StencilReadMask;
		StencilWriteMask = desc.StencilWriteMask;
		FrontFaceStencilFailOp = desc.FrontFace.StencilFailOp;
		FrontFaceStencilDepthFailOp = desc.FrontFace.StencilDepthFailOp;
		FrontFaceStencilPassOp = desc.FrontFace.StencilPassOp;
		FrontFaceStencilFunc = desc.FrontFace.StencilFunc;
		BackFaceStencilDepthFailOp = desc.BackFace.StencilDepthFailOp;
		BackFaceStencilFailOp = desc.BackFace.StencilFailOp;
		BackFaceStencilPassOp = desc.BackFace.StencilPassOp;
		BackFaceStencilFunc = desc.BackFace.StencilFunc;
	}

	D3D11_DEPTH_STENCIL_DESC GetDesc()
	{
		D3D11_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = DepthEnable;
		desc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(DepthWriteMask);
		desc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(DepthFunc);
		desc.StencilEnable = StencilEnable;
		desc.StencilReadMask = StencilReadMask;
		desc.StencilWriteMask = StencilWriteMask;
		desc.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(FrontFaceStencilFailOp);
		desc.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(FrontFaceStencilDepthFailOp);
		desc.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(FrontFaceStencilPassOp);
		desc.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(FrontFaceStencilFunc);
		desc.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(BackFaceStencilFailOp);
		desc.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(BackFaceStencilDepthFailOp);
		desc.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(BackFaceStencilPassOp);
		desc.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(BackFaceStencilFunc);
		return desc;
	}

	bool operator==(const DepsthStencilDesc& other) const
	{
		return memcmp(this, &other, sizeof(DepsthStencilDesc)) == 0;
	}

	bool operator!=(const DepsthStencilDesc& other) const
	{
		return !(*this == other);
	}


	int64 Get64BitValue() const {
		return *reinterpret_cast<const int64*>(this);
	}

	size_t Hash() const {
		std::hash<int64> hasher;
		return hasher(Get64BitValue());
	}
};

static_assert(sizeof(DepsthStencilDesc) <= sizeof(int64),  "DepsthStencilDesc size is too big");

struct BlendStateDesc
{
	uint AlphaToCoverageEnable : 1;
	uint IndependentBlendEnable : 1;
	uint RenderTargetWriteMask : 4;
	uint SrcBlend : 5;
	uint DestBlend : 5;
	uint BlendOp : 3;
	uint SrcBlendAlpha : 5;
	uint DestBlendAlpha : 5;
	uint BlendOpAlpha : 3;

	BlendStateDesc()
	{
		AlphaToCoverageEnable = false;
		IndependentBlendEnable = false;
		RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		SrcBlend = D3D11_BLEND_ONE;
		DestBlend = D3D11_BLEND_ZERO;
		BlendOp = D3D11_BLEND_OP_ADD;
		SrcBlendAlpha = D3D11_BLEND_ONE;
		DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendOpAlpha = D3D11_BLEND_OP_ADD;
	}

	BlendStateDesc(const D3D11_BLEND_DESC& desc)
	{
		AlphaToCoverageEnable = desc.AlphaToCoverageEnable;
		IndependentBlendEnable = desc.IndependentBlendEnable;
		RenderTargetWriteMask = desc.RenderTarget[0].RenderTargetWriteMask;
		SrcBlend = desc.RenderTarget[0].SrcBlend;
		DestBlend = desc.RenderTarget[0].DestBlend;
		BlendOp = desc.RenderTarget[0].BlendOp;
		SrcBlendAlpha = desc.RenderTarget[0].SrcBlendAlpha;
		DestBlendAlpha = desc.RenderTarget[0].DestBlendAlpha;
		BlendOpAlpha = desc.RenderTarget[0].BlendOpAlpha;
	}

	D3D11_BLEND_DESC GetDesc()
	{
		D3D11_BLEND_DESC desc{};
		desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
		desc.IndependentBlendEnable = IndependentBlendEnable;
		desc.RenderTarget[0].RenderTargetWriteMask = RenderTargetWriteMask;
		desc.RenderTarget[0].SrcBlend = static_cast<D3D11_BLEND>(SrcBlend);
		desc.RenderTarget[0].DestBlend = static_cast<D3D11_BLEND>(DestBlend);
		desc.RenderTarget[0].BlendOp = static_cast<D3D11_BLEND_OP>(BlendOp);
		desc.RenderTarget[0].SrcBlendAlpha = static_cast<D3D11_BLEND>(SrcBlendAlpha);
		desc.RenderTarget[0].DestBlendAlpha = static_cast<D3D11_BLEND>(DestBlendAlpha);
		desc.RenderTarget[0].BlendOpAlpha = static_cast<D3D11_BLEND_OP>(BlendOpAlpha);
		return desc;
	}

	bool operator==(const BlendStateDesc& other) const
	{
		return memcmp(this, &other, sizeof(BlendStateDesc)) == 0;
	}

	bool operator!=(const BlendStateDesc& other) const
	{
		return !(*this == other);
	}

	size_t Hash() const {
		static_assert(sizeof(BlendStateDesc) == sizeof(int), "BlendStateDesc size is too big");
        auto asInt = *reinterpret_cast<const int*>(this);
        return std::hash<int>{}(asInt);	
	}
};

static_assert(sizeof(BlendStateDesc) == sizeof(int), "BlendStateDesc size is too big");

struct SamplerStateDesc
{
	uint Filter : 9;
	uint AddressU : 3; //
	uint AddressV : 3; //
	uint AddressW : 3; //
	uint MaxAnisotropy : 4;
	uint ComparisonFunc : 3;
	uint Pad : 7;

	FLOAT MipLODBias;
	FLOAT MinLOD;
	FLOAT MaxLOD;
	UCol BorderColor;

	SamplerStateDesc()
	{
		Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		MipLODBias = 0;
		MaxAnisotropy = 0;
		ComparisonFunc = D3D11_COMPARISON_NEVER;
		BorderColor = Legacy::Vec4(1, 1, 1, 1);
		MinLOD = 0;
		MaxLOD = D3D11_FLOAT32_MAX;
		Pad = 0;
	}

	SamplerStateDesc(const D3D11_SAMPLER_DESC& desc)
	{
		Filter = desc.Filter;
		AddressU = desc.AddressU;
		AddressV = desc.AddressV;
		AddressW = desc.AddressW;
		MipLODBias = desc.MipLODBias;
		MaxAnisotropy = desc.MaxAnisotropy - 1;
		ComparisonFunc = desc.ComparisonFunc - D3D11_COMPARISON_NEVER;
		const auto& [r, g, b, a] = desc.BorderColor;
		BorderColor = Legacy::Vec4(r,g,b,a);
		MinLOD = desc.MinLOD;
		MaxLOD = desc.MaxLOD;
		Pad = 0;
	}

	D3D11_SAMPLER_DESC GetDesc()
	{
		D3D11_SAMPLER_DESC desc{};
		desc.Filter = static_cast<D3D11_FILTER>(Filter);
		desc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(AddressU);
		desc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(AddressV);
		desc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(AddressW);
		desc.MipLODBias = MipLODBias;
		desc.MaxAnisotropy = MaxAnisotropy + 1;
		desc.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(static_cast<D3D11_COMPARISON_FUNC>(ComparisonFunc) + D3D11_COMPARISON_NEVER);
		const auto c = GetBorderColor();
		memmove(&desc.BorderColor, &c, sizeof(c));
		desc.MinLOD = MinLOD;
		desc.MaxLOD = MaxLOD;
		return desc;
	}

	Legacy::Vec4 GetBorderColor() const
	{
		return Legacy::Vec4(BorderColor.r / 255.f, BorderColor.g / 255.f, BorderColor.b / 255.f, BorderColor.a / 255.f);
	}

	bool operator==(const SamplerStateDesc& other) const
	{
		return memcmp(this, &other, sizeof(SamplerStateDesc)) == 0;
	}

	bool operator!=(const SamplerStateDesc& other) const
	{
		return !(*this == other);
	}

	size_t Hash() const {
		size_t hash = 0;
		hash ^= std::hash<int>{}(Filter);
		hash ^= std::hash<int>{}(AddressU);
		hash ^= std::hash<int>{}(AddressV);
		hash ^= std::hash<int>{}(AddressW);
		hash ^= std::hash<int>{}(MaxAnisotropy);
		hash ^= std::hash<int>{}(ComparisonFunc);
		hash ^= std::hash<float>{}(MipLODBias);
		hash ^= std::hash<float>{}(MinLOD);
		hash ^= std::hash<float>{}(MaxLOD);
		hash ^= std::hash<uint32>{}(BorderColor.dcolor);
		return hash;
	}
};

static_assert(sizeof(SamplerStateDesc) == 20, "SamplerStateDesc size is too big");

namespace std
{
	template<>
	struct hash<RasterizerDesc>
	{
		size_t operator()(const RasterizerDesc& desc) const
		{
			return desc.HashSimple();
		}
	};

	template<>
	struct hash<DepsthStencilDesc>
	{
		size_t operator()(const DepsthStencilDesc& desc) const
		{
			return desc.Hash();
		}
	};

	template<>
	struct hash<BlendStateDesc>
	{
		size_t operator()(const BlendStateDesc& desc) const
		{
			return desc.Hash();
		}
	};

	template<>
	struct hash<SamplerStateDesc>
	{
		size_t operator()(const SamplerStateDesc& desc) const
		{
			return desc.Hash();
		}
	};
}

class CStateCache
{
public:
	CStateCache() = default;
	CStateCache(_smart_ptr<ID3DDevice> pDevice)
		: m_pDevice(pDevice)
		, m_StateManager(pDevice)
	{
	}

	_smart_ptr<ID3DRasterizerState> GetRasterizerState(const D3D11_RASTERIZER_DESC& desc)
	{
		RasterizerDesc key(desc);
		auto it = m_RasterizerStates.find(key);
		if (it != m_RasterizerStates.end())
		{
			return it->second;
		}
		else
		{
			ID3D11RasterizerState* pState = nullptr;
			auto result = m_pDevice->CreateRasterizerState(&desc, &pState);
			m_RasterizerStates[key] = pState;
			return pState;
		}
	}

	_smart_ptr<ID3DDepthStencilState> GetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc)
	{
		DepsthStencilDesc key(desc);
		auto it = m_DepthStencilStates.find(key);
		if (it != m_DepthStencilStates.end())
		{
			return it->second;
		}
		else
		{
			ID3DDepthStencilState* pState = nullptr;
			auto result = m_pDevice->CreateDepthStencilState(&desc, &pState);
			m_DepthStencilStates[key] = pState;
			return pState;
		}
	}

	_smart_ptr<ID3DBlendState> GetBlendState(const D3D11_BLEND_DESC& desc)
	{
		BlendStateDesc key(desc);
		auto it = m_BlendStates.find(key);
		if (it != m_BlendStates.end())
		{
			return it->second;
		}
		else
		{
			ID3DBlendState* pState = nullptr;
			auto result = m_pDevice->CreateBlendState(&desc, &pState);
			m_BlendStates[key] = pState;
			return pState;
		}
	}

	_smart_ptr<ID3D11SamplerState> GetSamplerState(const D3D11_SAMPLER_DESC& desc)
	{
		SamplerStateDesc key(desc);
		auto it = m_SamplerStates.find(key);
		if (it != m_SamplerStates.end())
		{
			return it->second;
		}
		else
		{
			ID3D11SamplerState* pState = nullptr;
			auto result = m_pDevice->CreateSamplerState(&desc, &pState);
			m_SamplerStates[key] = pState;
			return pState;
		}
	}

	void Clear()
	{
		m_RasterizerStates.clear();
		m_DepthStencilStates.clear();
		m_BlendStates.clear();
	}

	void SetRasterizerState(ID3DRasterizerState* pState)
	{
		m_StateManager.SetRasterizerState(pState);
	}

	void SetDepthStencilState(ID3DDepthStencilState* pState)
	{
		m_StateManager.SetDepthStencilState(pState);
	}

	void SetBlendState(ID3DBlendState* pState)
	{
		m_StateManager.SetBlendState(pState);
	}

	void SetSamplerState(ID3D11SamplerState* pState, UINT slot)
	{
		m_StateManager.SetSamplerState(pState, slot);
	}

	CStateManager m_StateManager;
private:
	_smart_ptr<ID3DDevice> m_pDevice;
	std::unordered_map<RasterizerDesc, _smart_ptr<ID3DRasterizerState>> m_RasterizerStates;
	std::unordered_map<DepsthStencilDesc, _smart_ptr<ID3DDepthStencilState>> m_DepthStencilStates;
	std::unordered_map<BlendStateDesc, _smart_ptr<ID3DBlendState>> m_BlendStates;
	std::unordered_map<SamplerStateDesc, _smart_ptr<ID3D11SamplerState> > m_SamplerStates;

	
};

class CDevice
{
public:
	using ContextPtr = _smart_ptr<ID3DDeviceContext>;
	using Ptr = _smart_ptr<ID3DDevice>;
public:
	CDevice(HWND Hwnd)
	    : m_Hwnd(Hwnd)
	{
	}

	bool Create(_smart_ptr<IDXGIAdapter> pAdapter);

	template<typename T>
	_smart_ptr<T> Get();

	template<>
	Ptr Get<ID3D11Device>()
	{
		return m_pd3dDevice;
	}
	template<>
	ContextPtr Get<ID3D11DeviceContext>()
	{
		return m_pImmediateContext;
	}

	std::tuple<bool, ID3D11Resource*, ID3DShaderResourceView*> CreateDDSTextureFromFile(
	    _In_z_ const wchar_t* szFileName,
	    _In_ size_t           maxsize = 0
#if 0
		,_Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr
#endif
	) const noexcept
	{
		ID3D11Resource*         pTexture{};
		ID3DShaderResourceView* pSRView = NULL;
		auto                    hr      = DirectX::CreateDDSTextureFromFile(
            m_pd3dDevice, szFileName, &pTexture, &pSRView, maxsize);
		if (SUCCEEDED(hr))
		{
			return std::make_tuple(true, pTexture, pSRView);
		}
		else
		{
			return std::make_tuple(false, pTexture, pSRView);
		}
	}

	HRESULT CreateDDSTextureFromMemory(
	    _In_reads_bytes_(ddsDataSize) const uint8_t* ddsData,
	    _In_ size_t                                  ddsDataSize,
	    _Outptr_opt_ ID3D11Resource** texture,
	    _Outptr_opt_ ID3D11ShaderResourceView** textureView,
	    _In_ size_t                             maxsize = 0 /*,
        _Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr*/
	    ) noexcept
	{
		auto HResult = DirectX::CreateDDSTextureFromMemory(
		    m_pd3dDevice,
		    ddsData,
		    ddsDataSize,
		    texture,
		    textureView);
		return HResult;
	}
	HRESULT CreateTextureFromJPGFile(const char* szFileName, 
	    const uint8_t* ddsData,
	     size_t                                  ddsDataSize,

		ID3D11Resource** ppTexture, ID3D11ShaderResourceView** ppSRView);
	ID3DTexture2D*                CreateEmptyTexture(vector2di size, color4f color, DXGI_FORMAT format, UINT bindFlags);
	//ID3DTexture2D* CreateTexture2D

	_smart_ptr<ID3DBlendState>    CreateBlendState(const D3D11_BLEND_DESC& desc);
	_smart_ptr<ID3DRasterizerState> CreateRasterizerState(const D3D11_RASTERIZER_DESC& desc);
	_smart_ptr<ID3DDepthStencilState> CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc);
	_smart_ptr<ID3D11SamplerState> CreateSamplerState(const D3D11_SAMPLER_DESC& desc);
	

	HWND                          m_Hwnd;
	D3D_DRIVER_TYPE               g_driverType        = D3D_DRIVER_TYPE_NULL;
	CD3D_FEATURE_LEVEL            m_FeatureLevel      = D3D_FEATURE_LEVEL_11_0;
	Ptr														m_pd3dDevice        = NULL;
	ContextPtr										m_pImmediateContext = NULL;


	CStateCache m_StateCache;
};
