#pragma once
#include <Cry_Color4.h>

class CD3DRenderer;
#if 1
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

class CDevice
{
public:
	CDevice(HWND Hwnd)
	    : m_Hwnd(Hwnd)
	{
	}

	bool Create(_smart_ptr<IDXGIAdapter> pAdapter);

	template<typename T>
	_smart_ptr<T> Get();

	template<>
	_smart_ptr<ID3D11Device> Get<ID3D11Device>()
	{
		return m_pd3dDevice;
	}
	template<>
	_smart_ptr<ID3DDeviceContext> Get<ID3D11DeviceContext>()
	{
		return m_pImmediateContext;
	}

	std::tuple<bool, ID3D11Resource*, ID3DShaderResourceView*> CreateDDSTextureFromFile(
	    _In_z_ const wchar_t* szFileName,
	    _In_ size_t           maxsize = 0
#if 0
		,_Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr
#endif
	    ) noexcept
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
	ID3DTexture2D*                CreateEmptyTexture(vector2di size, color4f color, DXGI_FORMAT format, UINT bindFlags);

	HWND                          m_Hwnd;
	D3D_DRIVER_TYPE               g_driverType        = D3D_DRIVER_TYPE_NULL;
	CD3D_FEATURE_LEVEL            m_FeatureLevel      = D3D_FEATURE_LEVEL_11_0;
	_smart_ptr<ID3DDevice>        m_pd3dDevice        = NULL;
	_smart_ptr<ID3DDeviceContext> m_pImmediateContext = NULL;
};
