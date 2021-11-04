#include "D3D\Renderer.h"
#include "DeviceObjects.h"

// Shader API

inline uint8* CDeviceObjectFactory::Map(D3DBuffer* buffer, uint32 subresource, buffer_size_t offset, buffer_size_t size, D3D11_MAP mode)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource = {0};
	gcpRendD3D->GetDeviceContext()->Map(buffer, subresource, mode, 0, &mapped_resource);
	return reinterpret_cast<uint8*>(mapped_resource.pData);
}

inline void CDeviceObjectFactory::Unmap(D3DBuffer* buffer, uint32 subresource, buffer_size_t offset, buffer_size_t size, D3D11_MAP mode)
{
	gcpRendD3D->GetDeviceContext()->Unmap(buffer, subresource);
}

inline ID3D11VertexShader* CDeviceObjectFactory::CreateVertexShader(const void* pData, size_t bytes)
{
	ID3D11VertexShader* pResult;
	return SUCCEEDED(gcpRendD3D->GetDevice()->CreateVertexShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

inline ID3D11PixelShader* CDeviceObjectFactory::CreatePixelShader(const void* pData, size_t bytes)
{
	ID3D11PixelShader* pResult;
	return SUCCEEDED(gcpRendD3D->GetDevice()->CreatePixelShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

inline ID3D11GeometryShader* CDeviceObjectFactory::CreateGeometryShader(const void* pData, size_t bytes)
{
	ID3D11GeometryShader* pResult;
	return SUCCEEDED(gcpRendD3D->GetDevice()->CreateGeometryShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

inline ID3D11HullShader* CDeviceObjectFactory::CreateHullShader(const void* pData, size_t bytes)
{
	ID3D11HullShader* pResult;
	return SUCCEEDED(gcpRendD3D->GetDevice()->CreateHullShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

inline ID3D11DomainShader* CDeviceObjectFactory::CreateDomainShader(const void* pData, size_t bytes)
{
	ID3D11DomainShader* pResult;
	return SUCCEEDED(gcpRendD3D->GetDevice()->CreateDomainShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

inline ID3D11ComputeShader* CDeviceObjectFactory::CreateComputeShader(const void* pData, size_t bytes)
{
	ID3D11ComputeShader* pResult;
	return SUCCEEDED(gcpRendD3D->GetDevice()->CreateComputeShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}
