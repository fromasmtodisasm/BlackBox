#pragma once

#include <span>

class CStateManager
{
public:
	CStateManager() = default;
	CStateManager(_smart_ptr<ID3DDevice> pDevice);

	void SetRasterizerState(ID3D11RasterizerState* pState);

	void SetDepthStencilState(ID3D11DepthStencilState* pState);

	void SetBlendState(ID3D11BlendState* pState);

	void SetSamplerState(ID3D11SamplerState* pState, int slot);

	void SetVertexShader(ID3D11VertexShader* pShader);

	void SetPixelShader(ID3D11PixelShader* pShader);

	void SetGeometryShader(ID3D11GeometryShader* pShader);

	void SetComputeShader(ID3D11ComputeShader* pShader);

	void SetHullShader(ID3D11HullShader* pShader);

	void SetDomainShader(ID3D11DomainShader* pShader);

	void SetInputLayout(ID3D11InputLayout* pLayout);

	//void SetResources(ID3D11ShaderResourceView* pResources[], int numResources);
	void SetResources(std::span<ID3D11ShaderResourceView*> pResources, int startSlot);

	void SetRenderTargets(std::span<ID3D11RenderTargetView*> pRenderTargets, ID3D11DepthStencilView* pDepthStencilView);

	void OnBeginFrame();

private:
	_smart_ptr<ID3DDeviceContext> m_pContext;
	ID3DRasterizerState* m_pRasterizerState{};
	ID3DDepthStencilState* m_pDepthStencilState{};
	ID3DBlendState* m_pBlendState{};
	ID3DSamplerState* m_pSamplerState{};

	// Shaders
	ID3D11VertexShader* m_pVertexShader{};
	ID3D11PixelShader* m_pPixelShader{};
	ID3D11GeometryShader* m_pGeometryShader{};
	ID3D11ComputeShader* m_pComputeShader{};
	ID3D11HullShader* m_pHullShader{};
	ID3D11DomainShader* m_pDomainShader{};
	ID3DInputLayout* m_pInputLayout{};

	std::array<ID3D11ShaderResourceView*,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT> m_pResources{};
	std::array<ID3D11RenderTargetView*,D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> m_pRenderTargets{};

};