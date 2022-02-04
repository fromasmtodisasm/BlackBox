#pragma once
#define IRENDER_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/Core/smartptr.hpp>

#include <BlackBox/System/File/ICryPak.hpp>

#define DX_11
#define BB_RENDERER_DIRECT3D 110
#ifdef DX_RENDERER
#	include <BlackBox/Core/Platform/Windows.hpp>
#	define __IFont_INTERFACE_DEFINED__

#	include <d3d11.h>
#	include <d3d11shader.h>
//#	include <d3dx11.h>
//#	include <d3dx10.h>
//#	include <D3dx11effect.h>
#	include "D3D/DDSTextureLoader11.h"
#	include <d3d9.h> // debug markers
const D3DCOLOR D3DC_Blue = D3DCOLOR_RGBA(0, 0, 1, 1); 

#ifdef DX_11
#	define ID3DDevice ID3D11Device
#	define ID3DDeviceContext ID3D11DeviceContext
#	define ID3DBuffer ID3D11Buffer
#	define ID3DResource ID3D11Resource
#	define ID3DRenderTargetView ID3D11RenderTargetView
#	define ID3DTexture2D ID3D11Texture2D
#	define ID3DDepthStencilView ID3D11DepthStencilView
#	define ID3DRasterizerState ID3D11RasterizerState
#	define ID3DDepthStencilState ID3D11DepthStencilState
#	define ID3DShaderResourceView ID3D11ShaderResourceView
#	define ID3DInputLayout ID3D11InputLayout
#	define ID3DSamplerState ID3D11SamplerState

#	define D3DDevice ID3DDevice
#	define D3DBuffer ID3DBuffer
#   define D3DFormat DXGI_FORMAT

	#define D3DReflection                     D3DReflect
	#define IID_D3DShaderReflection           IID_ID3D11ShaderReflection
	#define D3DShaderReflection               ID3D11ShaderReflection
	#define D3DShaderReflectionConstantBuffer ID3D11ShaderReflectionConstantBuffer
	#define D3DShaderReflectionVariable       ID3D11ShaderReflectionVariable
	#define D3DShaderReflectionType           ID3D11ShaderReflectionType
	#define D3D_SHADER_DESC                   D3D11_SHADER_DESC
	#define D3D_SHADER_TYPE_DESC              D3D11_SHADER_TYPE_DESC
	#define D3D_SHADER_BUFFER_DESC            D3D11_SHADER_BUFFER_DESC
	#define D3D_SHADER_VARIABLE_DESC          D3D11_SHADER_VARIABLE_DESC
	#define D3D_SHADER_INPUT_BIND_DESC        D3D11_SHADER_INPUT_BIND_DESC
	#define D3D_SIGNATURE_PARAMETER_DESC      D3D11_SIGNATURE_PARAMETER_DESC

using D3D_VIEWPORT = D3D11_VIEWPORT;
using D3D_RASTERIZER_DESC = D3D11_RASTERIZER_DESC;
#else
#	define ID3DDevice ID3D10Device
#	define ID3DBuffer ID3D10Buffer
#	define ID3DRenderTargetView ID3D10RenderTargetView
#	define ID3DTexture2D ID3D10Texture2D
#	define ID3DDepthStencilView ID3D10DepthStencilView
#	define ID3DRasterizerState ID3D10RasterizerState
#	define ID3DDepthStencilState ID3D10DepthStencilState
#endif

#	define D3D11_MAP_WRITE_DISCARD_VB (D3D11_MAP_WRITE_DISCARD)
#	define D3D11_MAP_WRITE_DISCARD_IB (D3D11_MAP_WRITE_DISCARD)
#	define D3D11_MAP_WRITE_DISCARD_CB (D3D11_MAP_WRITE_DISCARD)
#	define D3D11_MAP_WRITE_DISCARD_SR (D3D11_MAP_WRITE_DISCARD)
#	define D3D11_MAP_WRITE_DISCARD_UA (D3D11_MAP_WRITE_DISCARD)

typedef ID3D11SamplerState CDeviceSamplerState;
typedef ID3D11InputLayout	CDeviceInputLayout;


using D3DXMATRIX_  = glm::mat4;
using D3DXVECTOR3_ = glm::vec3;
using D3DXVECTOR4_ = glm::vec4;

#	define D3DXMATRIX D3DXMATRIX_
#	define D3DXVECTOR3 D3DXVECTOR3_
#	define D3DXVECTOR4 D3DXVECTOR4_

#	define D3DX_PI glm::pi<double>()

inline void D3DXMatrixIdentity_(D3DXMATRIX* mat)
{
	*mat = glm::mat4(1.f);
}

inline void D3DXMatrixRotationY_(D3DXMATRIX* mat, float y)
{
	*mat = glm::rotate(D3DXMATRIX(1.f), y, D3DXVECTOR3(0.f, 1.f, 0.f));
}

inline D3DXMATRIX* D3DXMatrixLookAtLH_(D3DXMATRIX* mat, D3DXVECTOR3* Eye, D3DXVECTOR3* At, D3DXVECTOR3* Up)
{
	*mat = glm::lookAtLH(*Eye, *At, *Up);
	return mat;
}

// Build a perspective projection matrix. (left-handed)
inline D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH_(D3DXMATRIX* pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf)
{
	*pOut = glm::perspectiveLH_ZO(fovy, Aspect, zn, zf);
	return pOut;
}

inline D3DXMATRIX* D3DXMatrixTranspose(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM)
{
	*pOut = glm::transpose(*pM);
	return pOut;
}

#	define D3DXMatrixIdentity D3DXMatrixIdentity_
#	define D3DXMatrixRotationY D3DXMatrixRotationY_
#	define D3DXMatrixLookAtLH D3DXMatrixLookAtLH_
#	define D3DXMatrixPerspectiveFovLH D3DXMatrixPerspectiveFovLH_

#elif VK_RENDERER
#	include <vulkan/vulkan.h>
#endif

#include <array>
#include <sstream>

#include "Common\CommonRender.h"
#include "Common\CryNameR.h"

namespace Memory
{
	constexpr auto RoundUp(int numToRound, int multiple)
	{
		if (multiple == 0)
			return numToRound;

		auto remainder = numToRound % multiple;
		if (remainder == 0)
			return numToRound;

		return numToRound + multiple - remainder;
	}
	template<class T, size_t align>
	struct AlignedSize
	{
		static constexpr auto value = RoundUp(sizeof(T), align);
	};

	template<class T>
	using AlignedSizeCB = AlignedSize<T, sizeof(Legacy::Vec4)>;
}

struct ScopedMarker
{
	ScopedMarker(const wchar_t* m)
	{
		D3DPERF_BeginEvent(D3DC_Blue, m);
	}
	~ScopedMarker()
	{
		D3DPERF_EndEvent();
	}
};

#define D3DMarker(m) ScopedMarker _m(L#m);

inline void SetDebugName(ID3D11DeviceChild* pNativeResource, const char* name)
{
#if !defined(RELEASE) && BB_PLATFORM_WINDOWS
	if (!pNativeResource)
		return;

	pNativeResource->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
	pNativeResource->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name) + 1, name);
#endif
}

#include <wrl.h>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#define H(exp, fmt, ...) \
[&] { \
	auto hr = exp;        \
	if (FAILED(hr)){     \
		CryError("[Render] " fmt, __VA_ARGS__);\
	} \
	return hr; \
}()



// Types
struct CTexture
{
	uint16			 m_nWidth;
	uint16			 m_nHeight;
	uint16			 m_nDepth;
	ETEX_Format		 m_eDstFormat;
	ID3D11View*		 m_pView;
	ID3D11Texture2D* m_pResource;
};


