#pragma once

class CDevice;

class CSwapChain
{
public:
	CSwapChain(
	    std::shared_ptr<CDevice> Device, _smart_ptr<IDXGISwapChain> pSwapChain);
	static std::vector<_smart_ptr<IDXGIAdapter>> EnumerateAdapters(IDXGIFactory* pFactory);
	static _smart_ptr<IDXGIAdapter>              SelectAdapter(std::vector<_smart_ptr<IDXGIAdapter>> vAdapters);
	static std::unique_ptr<CSwapChain>           Create(std::shared_ptr<CDevice> pDevice, HWND Hwnd);

	_smart_ptr<IDXGISwapChain>                   Get() { return m_pSwapChain; }
	HRESULT                                      Present(
	                                         /* [in] */ UINT SyncInterval,
        /* [in] */ UINT Flags)
	                                     {
		return m_pSwapChain->Present(SyncInterval, Flags);
	}

	bool                          OnResize(int newWidth, int newHeight, ID3D11RenderTargetView** m_pMainRenderTargetView);

	_smart_ptr<ID3DDevice>        m_pd3dDevice;
	_smart_ptr<ID3DDeviceContext> m_pImmediateDeviceContext;
	_smart_ptr<IDXGISwapChain>    m_pSwapChain;
};
