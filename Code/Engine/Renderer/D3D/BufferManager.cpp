#include "../BufferManager.hpp"

#include "Renderer.h"

#if 0
namespace
{

#define OFFSET(m) reinterpret_cast<GLvoid*>(static_cast<INT_PTR>((gBufInfoTable[vertexFormat].m)))
	INPUT_ELEMENT_DESC GetPositionAttributePointer(int vertexFormat)
	{
		return INPUT_ELEMENT_DESC(
			position, 3, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat], reinterpret_cast<GLvoid*>(0));
	}
	INPUT_ELEMENT_DESC GetNormalAttributePointer(int vertexFormat)
	{
		return INPUT_ELEMENT_DESC(
			normal, 3, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat], OFFSET(OffsNormal));
	}
	INPUT_ELEMENT_DESC GetColorAttributePointer(int vertexFormat)
	{
		return INPUT_ELEMENT_DESC(
			color, 4, GL_UNSIGNED_BYTE, GL_TRUE, gVertexSize[vertexFormat], OFFSET(OffsColor));
	}
	INPUT_ELEMENT_DESC GetUVAttributePointer(int vertexFormat)
	{
		return INPUT_ELEMENT_DESC(
			uv, 2, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat], OFFSET(OffsTC));
	}

	void SetAttribPointer(INPUT_ELEMENT_DESC vap)
	{
		gl::EnableVertexAttribArray(vap.index);
		gl::VertexAttribPointer(
			vap.index, vap.size, vap.type, vap.normalized, vap.stride, vap.pointer);
	}
	void DisableAttributes(CVertexBuffer* vb)
	{
		SVertBufComps vbc{};
		GetVertBufComps(&vbc, vb->m_vertexformat);

		gl::DisableVertexAttribArray(position);
		if (vbc.m_bHasNormals)
		{
			gl::DisableVertexAttribArray(normal);
		}
		if (vbc.m_bHasTC)
		{
			gl::DisableVertexAttribArray(uv);
		}
		if (vbc.m_bHasColors)
		{
			gl::DisableVertexAttribArray(color);
		}
	}
	void EnableAttributes(CVertexBuffer* vb)
	{
		SVertBufComps vbc{};
		auto& vf = vb->m_vertexformat;
		GetVertBufComps(&vbc, vf);

		SetAttribPointer(GetPositionAttributePointer(vf));
		if (vbc.m_bHasNormals)
		{
			SetAttribPointer(GetNormalAttributePointer(vf));
		}
		if (vbc.m_bHasTC)
		{
			SetAttribPointer(GetUVAttributePointer(vf));
		}
		if (vbc.m_bHasColors)
		{
			SetAttribPointer(GetColorAttributePointer(vf));
		}
	}

} // namespace
#endif

D3D_PRIMITIVE_TOPOLOGY  ToDxPrimitive(RenderPrimitive rp)
{
	switch (rp)
	{
	case RenderPrimitive::LINES:
		return D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case RenderPrimitive::LINE_LOOP:
		gEnv->pSystem->FatalError("Unsupported topology");
		assert(0);
		return D3D_PRIMITIVE_TOPOLOGY(-1);
	case RenderPrimitive::LINE_STRIP:
		return D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case RenderPrimitive::TRIANGLES:
		return D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case RenderPrimitive::TRIANGLE_STRIP:
		return D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	}
	return D3D_PRIMITIVE_TOPOLOGY(-1);
}

//////////////////////////////////////////////////////////////////////////////////////
CConstantBuffer::CConstantBuffer(uint32 handle)
	: m_buffer()
	, m_base_ptr()
	, m_handle(handle)
	, m_offset(0)
	, m_size(0)
	, m_nRefCount(1u)
	, m_nUpdCount(0u)
	, m_clearFlags(0)
{
}


CConstantBuffer::~CConstantBuffer()
{
}

CBufferManager::CBufferManager()
{
}

CBufferManager::~CBufferManager()
{
}

bool CBufferManager::Preallocate()
{
	return false;
}

CVertexBuffer* CBufferManager::Create(int vertexcount, int vertexformat, const char* szSource, bool bDynamic)
{
	bDynamic = true;
	assert(vertexformat >= VERTEX_FORMAT_P3F && vertexformat < VERTEX_FORMAT_NUMS);
	auto buffer = new CVertexBuffer;
	SVertexStream& stream = buffer->m_VS[VSF_GENERAL];
	buffer->m_VS[VSF_GENERAL] = stream;
	stream.m_bDynamic	  = bDynamic;
	stream.m_VData		  = CreateVertexBuffer(vertexformat, vertexcount);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage		  = bDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth	  = vertexcount * gVertexSize[vertexformat];
	bufferDesc.BindFlags	  = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = bDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags	  = 0;

	ID3D11Buffer**  p_VB	   = reinterpret_cast<ID3D11Buffer**>(&buffer->m_VS[0].m_VertBuf.m_pPtr);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem		  = stream.m_VData;
	InitData.SysMemPitch	  = 0;
	InitData.SysMemSlicePitch = 0;
	auto hr					  = GetDevice()->CreateBuffer(&bufferDesc, &InitData, p_VB);
	if (FAILED(hr))
	{
		CryFatalError("Cannot create vertex buffer");	
		delete buffer;
		return nullptr;
	}


	UINT stride = gVertexSize[vertexformat];
    UINT offset = 0;

	GetDeviceContext()->IASetVertexBuffers(0, 1, p_VB, &stride, &offset);

	buffer->m_bDynamic		  = bDynamic;
	buffer->m_NumVerts		  = vertexcount;
	buffer->m_vertexformat	  = vertexformat;
	//EnableAttributes(buffer);
	//debuger::vertex_array_label(buffer->m_Container, szSource);

	return buffer;
}

void CBufferManager::Release(CVertexBuffer* pVertexBuffer)
{
	assert(pVertexBuffer);
	if (pVertexBuffer)
	{
		auto ptr = reinterpret_cast<ID3D11Buffer*>(pVertexBuffer->m_VS[0].m_VertBuf.m_pPtr);
		SAFE_RELEASE(ptr);
		for (int i = 0; i < VSF_NUM; i++)
		{
			SAFE_DELETE(pVertexBuffer->m_VS[i].m_VData);
		}
	}
	SAFE_DELETE(pVertexBuffer);
}

void CBufferManager::Create(SVertexStream* dest, const void* src, int indexcount)
{
	assert(dest != nullptr);
	assert(src != nullptr);

	auto stream = new SVertexStream;
	stream->m_bDynamic	  = false;
	stream->m_nBufOffset  = 0;
	stream->m_nItems	  = indexcount;

	ID3D11Buffer** p_IB	   = (ID3D11Buffer**)(&stream->m_VertBuf.m_pPtr);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage		   = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = indexcount * sizeof(short); //sizeof(DWORD) * mNumFaces * 3;
	ibd.BindFlags	   = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags	   = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = src;
	if (GetDevice()->CreateBuffer(&ibd, &iinitData, p_IB))
	{
	
	}

	*dest = *stream;
}

void CBufferManager::Release(SVertexStream* pVertexStream)
{
	assert(pVertexStream);
	reinterpret_cast<ID3D11Buffer*>(pVertexStream->m_VertBuf.m_pPtr)->Release();
	delete pVertexStream;
}

void CBufferManager::Draw(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start, int vert_stop, CMatInfo* mi)
{
	assert(src != nullptr);
	auto to_draw = vert_stop - vert_start;
	auto offset_in_buffer = src->m_VS[VSF_GENERAL].m_nBufOffset;
	UINT stride = gVertexSize[src->m_vertexformat];
    UINT offset = 0;

	GetDeviceContext()->IASetVertexBuffers(0, 1, reinterpret_cast<ID3D11Buffer* const*>(&src->m_VS[0].m_VertBuf.m_pPtr), &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(ToDxPrimitive(static_cast<RenderPrimitive>(prmode)));

	if (indicies != nullptr)
	{
		assert(numindices != 0);
		auto s = sizeof(ushort);
		GetDeviceContext()->IASetIndexBuffer((ID3D11Buffer*)indicies->m_VertBuf.m_pPtr, DXGI_FORMAT_R16_UINT, 0);
		GetDeviceContext()->DrawIndexed(numindices, offsindex * s + offset_in_buffer, vert_start + offset_in_buffer);
	}
	else
	{
		GetDeviceContext()->Draw(to_draw == 0 ? src->m_NumVerts : to_draw, vert_start + offset_in_buffer);
	}
}

void CBufferManager::Update(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs, int Type)
{
	#if 0
	if (vertexcount <= dest->m_NumVerts)
	{
		gl::NamedBufferSubData(dest->m_VS[VSF_GENERAL].m_VertBuf.m_nID, , , src);
	}
	else
	{
		gl::NamedBufferData(dest->m_VS[VSF_GENERAL].m_VertBuf.m_nID, vertexcount * gVertexSize[dest->m_vertexformat], src, GL_DYNAMIC_DRAW);
	}
	#endif

	if (vertexcount <= dest->m_NumVerts)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		auto hr = GetDeviceContext()->Map(reinterpret_cast<ID3D11Buffer*>(dest->m_VS[0].m_VertBuf.m_pPtr), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			CryFatalError("Cannot map vertex buffer");
			return;
		}

		memcpy((char*)mappedResource.pData + nOffs + dest->m_VS[VSF_GENERAL].m_nBufOffset, src, vertexcount * gVertexSize[dest->m_vertexformat]);
		GetDeviceContext()->Unmap(reinterpret_cast<ID3D11Buffer*>(dest->m_VS[0].m_VertBuf.m_pPtr),0);
	}
	else
	{
		CryError("Origin buffer smaller");
	}
}

void CBufferManager::Update(SVertexStream* dest, const void* src, int indexcount, bool bUnLock)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto hr = GetDeviceContext()->Map(reinterpret_cast<ID3D11Buffer*>(dest->m_VertBuf.m_pPtr), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		CryFatalError("Cannot map vertex buffer");
	}

	memcpy(mappedResource.pData, src, indexcount * SIZEOF_INDEX);
	GetDeviceContext()->Unmap(reinterpret_cast<ID3D11Buffer*>(dest->m_VertBuf.m_pPtr),0);
}

IGraphicsDeviceConstantBuffer* CBufferManager::CreateConstantBuffer(int size)
{
	return nullptr;
}

void CBufferManager::MemoryUsage(ICrySizer* pSizer)
{
	pSizer->Add(this);
}

#if 0
void CGraphicsDeviceConstantBuffer::SetData(const uint8* data, size_t size)
{
}
#endif
