#include "pch.hpp"
#include "Device.hpp"
#include "Renderer.h"
#include "FreeImage.h"


bool CDevice::Create(_smart_ptr<IDXGIAdapter> pAdapter)
{
	HRESULT         hr                = S_OK;
	UINT            createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
	D3D_DRIVER_TYPE driverTypes[] =
	    {
	        D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_HARDWARE),
	        D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_WARP),
	        D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_REFERENCE),
	    };
	UINT               numDriverTypes = ARRAYSIZE(driverTypes);

	CD3D_FEATURE_LEVEL featureLevels[] =
	    {
	        D3D_FEATURE_LEVEL_11_0,
	        D3D_FEATURE_LEVEL_10_1,
	        D3D_FEATURE_LEVEL_10_0,
	    };
	UINT              numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL l                = featureLevels[0];

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];

		if (SUCCEEDED(H(D3D11CreateDevice(
		                    pAdapter,
		                    driverTypes[driverTypeIndex],
		                    NULL,
		                    createDeviceFlags,
		                    &featureLevels[0],
		                    numFeatureLevels,
		                    D3D11_SDK_VERSION,
		                    m_pd3dDevice.GetAddressOf(),
		                    m_FeatureLevel,
		                    m_pImmediateContext.GetAddressOf()),
		                "Failed create D3D device")))
		{
			m_StateCache = CStateCache(m_pd3dDevice);
			return true;
		}
	}
	return false;
}

HRESULT CDevice::CreateTextureFromJPGFile(const char* szFileName, 
	    const uint8_t* ddsData,
	     size_t                                  ddsDataSize,

ID3D11Resource** ppTexture, ID3D11ShaderResourceView** ppSRView)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	FIMEMORY* fiMemory = FreeImage_OpenMemory((BYTE*)ddsData, ddsDataSize);

	fif = FreeImage_GetFileType(szFileName, 0);

	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(szFileName);

	if (fif == FIF_UNKNOWN)
		return E_FAIL;

	dib = FreeImage_LoadFromMemory(fif, fiMemory);

	if (!dib)
		return E_FAIL;

	// Преобразовать изображение в 32-битный формат
	FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);

	// Освободить исходное изображение
	FreeImage_Unload(dib);

	if (!dib32)
    return E_FAIL;

	// Переставить каналы B и R
	for (unsigned y = 0; y < FreeImage_GetHeight(dib32); y++)
	{
		BYTE* bits = FreeImage_GetScanLine(dib32, y);
		for (unsigned x = 0; x < FreeImage_GetWidth(dib32); x++)
		{
			BYTE tmp = bits[FI_RGBA_RED];
			bits[FI_RGBA_RED] = bits[FI_RGBA_BLUE];
			bits[FI_RGBA_BLUE] = tmp;
			bits += 4;
		}
	}

	dib = dib32;

	BYTE* pData = FreeImage_GetBits(dib);

	int iWidth = FreeImage_GetWidth(dib);
	int iHeight = FreeImage_GetHeight(dib);

	if ((iWidth == 0) || (iHeight == 0))
	{
		FreeImage_Unload(dib);
		return E_FAIL;
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = iWidth;
	desc.Height = iHeight;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA subresource;
	subresource.pSysMem = pData;
	subresource.SysMemPitch = FreeImage_GetPitch(dib);
	subresource.SysMemSlicePitch = 0;

	HRESULT hr = m_pd3dDevice->CreateTexture2D(&desc, &subresource, (ID3D11Texture2D**)ppTexture);

	if (SUCCEEDED(hr))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = m_pd3dDevice->CreateShaderResourceView(*ppTexture, &SRVDesc, ppSRView);
	}

	FreeImage_CloseMemory(fiMemory);
	FreeImage_Unload(dib);

	return hr;
}

ID3DTexture2D* CDevice::CreateEmptyTexture(vector2di size, color4f color, DXGI_FORMAT format, UINT bindFlags)
{
	// Create the render target texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width              = size.x;
	desc.Height             = size.y;
	desc.MipLevels          = 1;
	desc.ArraySize          = 1;
	desc.Format             = format;
	desc.SampleDesc.Count   = 1;
	desc.Usage              = D3D11_USAGE_DEFAULT;
	desc.BindFlags          = bindFlags;

	ID3DTexture2D* pTexture = NULL;
	m_pd3dDevice->CreateTexture2D(&desc, NULL, &pTexture);

	// Create the shader-resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format                          = desc.Format;
	srDesc.ViewDimension                   = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip       = 0;
	srDesc.Texture2D.MipLevels             = 1;

	ID3DShaderResourceView* pShaderResView = NULL;
	m_pd3dDevice->CreateShaderResourceView(pTexture, &srDesc, &pShaderResView);

#if 0
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	::GetDeviceContext()->Map(pTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

	UCHAR* pTexels = (UCHAR*)mappedTex.pData;
	for (UINT row = 0; row < desc.Height; row++)
	{
		UINT rowStart = row * mappedTex.RowPitch;
		for (UINT col = 0; col < desc.Width; col++)
		{
			UINT colStart					 = col * 4;
			pTexels[rowStart + colStart + 0] = 255; // Red
			pTexels[rowStart + colStart + 1] = 128; // Green
			pTexels[rowStart + colStart + 2] = 64;	// Blue
			pTexels[rowStart + colStart + 3] = 32;	// Alpha
		}
	}

	::GetDeviceContext()->Unmap(pTexture, D3D11CalcSubresource(0, 0, 1));
#endif

	return 0;
}

_smart_ptr<ID3DBlendState> CDevice::CreateBlendState(const D3D11_BLEND_DESC& desc)
{
	return m_StateCache.GetBlendState(desc);
}

_smart_ptr<ID3DRasterizerState> CDevice::CreateRasterizerState(const D3D11_RASTERIZER_DESC& desc)
{
	return m_StateCache.GetRasterizerState(desc);
}

_smart_ptr<ID3DDepthStencilState> CDevice::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc)
{
	return m_StateCache.GetDepthStencilState(desc);
}

_smart_ptr<ID3D11SamplerState> CDevice::CreateSamplerState(const D3D11_SAMPLER_DESC& desc)
{
	return m_StateCache.GetSamplerState(desc);
}
