#include "pch.hpp"

#include "StateManager.hpp"

CStateManager::CStateManager(_smart_ptr<ID3DDevice> pDevice)
{
	pDevice->GetImmediateContext(m_pContext.GetAddressOf());
}

void CStateManager::SetRasterizerState(ID3D11RasterizerState* pState)
{
	if (m_pRasterizerState != pState)
	{
		m_pContext->RSSetState(pState);
		m_pRasterizerState = pState;
	}
}

void CStateManager::SetDepthStencilState(ID3D11DepthStencilState* pState)
{
	if (m_pDepthStencilState != pState)
	{
		m_pContext->OMSetDepthStencilState(pState, 0);
		m_pDepthStencilState = pState;
	}
}

void CStateManager::SetBlendState(ID3D11BlendState* pState)
{
	if (m_pBlendState != pState)
	{
		m_pContext->OMSetBlendState(pState, 0, 0xffffffff);
		m_pBlendState = pState;
	}
}

void CStateManager::SetSamplerState(ID3D11SamplerState* pState, int slot)
{
	if (m_pSamplerState != pState)
	{
		m_pContext->PSSetSamplers(slot, 1, &pState);
		m_pSamplerState = pState;
	}
}

void CStateManager::SetVertexShader(ID3D11VertexShader* pShader)
{
    if (m_pVertexShader != pShader)
    {
        m_pContext->VSSetShader(pShader, nullptr, 0);
        m_pVertexShader = pShader;
    }
}

void CStateManager::SetPixelShader(ID3D11PixelShader* pShader)
{
    if (m_pPixelShader != pShader)
    {
        m_pContext->PSSetShader(pShader, nullptr, 0);
        m_pPixelShader = pShader;
    }
}

void CStateManager::SetGeometryShader(ID3D11GeometryShader* pShader)
{
    if (m_pGeometryShader != pShader)
    {
        m_pContext->GSSetShader(pShader, nullptr, 0);
        m_pGeometryShader = pShader;
    }
}

void CStateManager::SetComputeShader(ID3D11ComputeShader* pShader)
{
    if (m_pComputeShader != pShader)
    {
        m_pContext->CSSetShader(pShader, nullptr, 0);
        m_pComputeShader = pShader;
    }
}

void CStateManager::SetHullShader(ID3D11HullShader* pShader)
{
    if (m_pHullShader != pShader)
    {
        m_pContext->HSSetShader(pShader, nullptr, 0);
        m_pHullShader = pShader;
    }
}

void CStateManager::SetInputLayout(ID3D11InputLayout* pLayout)
{
    if (m_pInputLayout != pLayout)
    {
        m_pContext->IASetInputLayout(pLayout);
        m_pInputLayout = pLayout;
    }
}

void CStateManager::SetResources(std::span<ID3D11ShaderResourceView*> pResources, int startSlot) {
    if (startSlot < 0 || startSlot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) {
        // Обработка ошибки: неверный слот
        return;
    }
    if (pResources.size() > (D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - startSlot)) {
        // Обработка ошибки: слишком много ресурсов для заданного слота
        return;
    }

    int first = -1; // Индекс начала измененной последовательности
    for (int i = 0; i < pResources.size(); ++i) {
        if (m_pResources[startSlot + i] != pResources[i]) {
            m_pResources[startSlot + i] = pResources[i];
            if (first == -1) first = i; // Запоминаем начало измененной последовательности
        } else if (first != -1) {
            // Устанавливаем ресурсы с 'first' до 'i - 1'
            m_pContext->PSSetShaderResources(startSlot + first, i - first, &pResources[first]);
            first = -1; // Сброс начала измененной последовательности
        }
    }
    // Если последняя часть была изменена, устанавливаем ее
    if (first != -1) {
        m_pContext->PSSetShaderResources(startSlot + first, pResources.size() - first, &pResources[first]);
    }
}



void CStateManager::SetRenderTargets(std::span<ID3D11RenderTargetView*> pRenderTargets, ID3D11DepthStencilView* pDepthStencilView)
{
    #if _DEBUG
    if (pRenderTargets.size() > D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
    {
        // Обработка ошибки: слишком много рендер-таргетов
        CryError("Too many render targets");
        return;
    }
    #endif

    // Фактические рендер-таргеты
    std::array<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> targets = { nullptr };

    // Устанавливаем рендер-таргеты
    for (int i = 0; i < pRenderTargets.size(); ++i)
    {
        if (m_pRenderTargets[i] != pRenderTargets[i])
        {
            m_pRenderTargets[i] = pRenderTargets[i];
            targets[i] = pRenderTargets[i];
            continue;
        }
    }

    // Устанавливаем рендер-таргеты
    m_pContext->OMSetRenderTargets(targets.size(), targets.data(), pDepthStencilView);


}

void CStateManager::OnBeginFrame()
{
    // Сбрасываем состояние
    m_pResources.fill(nullptr);
    m_pRenderTargets.fill(nullptr);
}


