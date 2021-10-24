#pragma once
#define IRENDER_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <BlackBox/System/File/ICryPak.hpp>

#ifdef DX_RENDERER
#	include <BlackBox/Core/Platform/Windows.hpp>
#	define __IFont_INTERFACE_DEFINED__

#	include <d3d11.h>
//#	include <d3dx11.h>
#	include <d3dx10.h>
#	include "D3D/DDSTextureLoader.h"

#ifdef DX_11
#	define ID3DDevice ID3D11Device
#	define ID3DRenderTargetView ID3D11RenderTargetView
#	define ID3DTexture2D ID3D11Texture2D
#	define ID3DDepthStencilView ID3D11DepthStencilView
#	define ID3DRasterizerState ID3D11RasterizerState
#	define ID3DDepthStencilState ID3D11DepthStencilState
#else
#	define ID3DDevice ID3D10Device
#	define ID3DBuffer ID3D10Buffer
#	define ID3DRenderTargetView ID3D10RenderTargetView
#	define ID3DTexture2D ID3D10Texture2D
#	define ID3DDepthStencilView ID3D10DepthStencilView
#	define ID3DRasterizerState ID3D10RasterizerState
#	define ID3DDepthStencilState ID3D10DepthStencilState
#endif

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

#include "RendererCommon.hpp"
