#include "Device.hpp"
#include "Renderer.h"

bool CDevice::Create(_smart_ptr<IDXGIAdapter> pAdapter)
{
	HRESULT hr				  = S_OK;
	UINT	createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_HARDWARE),
			D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_WARP),
			D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_REFERENCE),
		};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	CD3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL l = featureLevels[0];

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
			return true;
		}
		#if 0
		//hr = D3D11CreateDeviceAndSwapChain(
		//	NULL,
		//	g_driverType,
		//	NULL,
		//	createDeviceFlags,
		//	featureLevels[0],
		//	numFeatureLevels,
		//	D3D11_SDK_VERSION,
		//	&sd,
		//	pSwapChain.GetAddressOf(),
		//	m_pd3dDevice.GetAddressOf(),
		//	&m_FeatureLevel,
		//	m_pImmediateContext.GetAddressOf());
		#endif
	}
	return false;
}

ID3DTexture2D* CDevice::CreateEmptyTexture(vector2di size, color4f color, DXGI_FORMAT format, UINT bindFlags)
{
	// Create the render target texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width			  = size.x;
	desc.Height			  = size.y;
	desc.MipLevels		  = 1;
	desc.ArraySize		  = 1;
	desc.Format			  = format;
	desc.SampleDesc.Count = 1;
	desc.Usage			  = D3D11_USAGE_DEFAULT;
	desc.BindFlags		  = bindFlags;

	ID3DTexture2D* pTexture = NULL;
	m_pd3dDevice->CreateTexture2D(&desc, NULL, &pTexture);

	// Create the shader-resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format					 = desc.Format;
	srDesc.ViewDimension			 = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels		 = 1;

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
