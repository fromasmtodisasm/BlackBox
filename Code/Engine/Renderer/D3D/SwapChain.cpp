#include "SwapChain.hpp"
#include "Device.hpp"

CSwapChain::CSwapChain(std::shared_ptr<CDevice> Device, _smart_ptr<IDXGISwapChain> pSwapChain)
    : m_pd3dDevice(Device->Get<ID3DDevice>())
    , m_pImmediateDeviceContext(Device->Get<ID3DDeviceContext>())
    , m_pSwapChain(pSwapChain)
{
}

std::vector<_smart_ptr<IDXGIAdapter>> CSwapChain::EnumerateAdapters(IDXGIFactory* pFactory)
{
	_smart_ptr<IDXGIAdapter>              pAdapter = NULL;
	std::vector<_smart_ptr<IDXGIAdapter>> vAdapters;

	for (UINT i = 0;
	     pFactory->EnumAdapters(i, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND;
	     ++i)
	{
		vAdapters.push_back(pAdapter);
	}
	return vAdapters;
}

_smart_ptr<IDXGIAdapter> CSwapChain::SelectAdapter(std::vector<_smart_ptr<IDXGIAdapter>> vAdapters)
{
	for (auto& adapter : vAdapters)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

#define LogInfo(p, exp, f) CryLog("    " #p ": " f, desc.p*(exp))
		CryLog("GPU Info");
		LogInfo(VendorId, 1, "0x%X");
		LogInfo(VendorId, 1, "0x%X");
		LogInfo(DeviceId, 1, "0x%X");
		LogInfo(SubSysId, 1, "0x%X");
		LogInfo(Revision, 1, "%d");
		float exp = (1.f / (1024.f * 1024.f));
		LogInfo(DedicatedVideoMemory, exp, "%f Mb");
		LogInfo(DedicatedSystemMemory, exp, "%f Mb");
		LogInfo(SharedSystemMemory, exp, "%f Mb");
		//LUID AdapterLuid;
#undef LogInfo
	}

	return vAdapters[0];
}

bool CSwapChain::OnResize(int newWidth, int newHeight, ID3D11RenderTargetView** m_pMainRenderTargetView)
{
	m_pSwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
	// Create a render target view
	ID3DTexture2D* pBuffer;
	auto           hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3DTexture2D), (LPVOID*)&pBuffer);
	if (FAILED(hr))
		return false;

	hr            = m_pd3dDevice->CreateRenderTargetView(pBuffer, NULL, m_pMainRenderTargetView);
	auto num_refs = pBuffer->Release();
	if (FAILED(hr))
		return false;
	return true;
}
