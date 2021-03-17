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
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	case RenderPrimitive::LINE_LOOP:
		gEnv->pSystem->FatalError("Unsupported topology");
		assert(0);
		return D3D_PRIMITIVE_TOPOLOGY(-1);
	case RenderPrimitive::LINE_STRIP:
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case RenderPrimitive::TRIANGLES:
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case RenderPrimitive::TRIANGLE_STRIP:
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	}
	return D3D_PRIMITIVE_TOPOLOGY(-1);
}

CConstantBuffer::CConstantBuffer(uint32 handle)
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

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.Usage		  = bDynamic ? D3D10_USAGE_DYNAMIC : D3D10_USAGE_DEFAULT;
	bufferDesc.ByteWidth	  = vertexcount * gVertexSize[vertexformat];
	bufferDesc.BindFlags	  = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = bDynamic ? D3D10_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags	  = 0;

	ID3D10Buffer**  p_VB	   = reinterpret_cast<ID3D10Buffer**>(&buffer->m_VS[0].m_VertBuf.m_pPtr);

	D3D10_SUBRESOURCE_DATA InitData;
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

	GetDevice()->IASetVertexBuffers(0, 1, p_VB, &stride, &offset);

	buffer->m_bDynamic		  = bDynamic;
	buffer->m_NumVerts		  = vertexcount;
	buffer->m_vertexformat	  = vertexformat;
	//EnableAttributes(buffer);
	//debuger::vertex_array_label(buffer->m_Container, szSource);

	return buffer;
}

void CBufferManager::Release(CVertexBuffer* pVertexBuffer)
{
	if (pVertexBuffer)
	{
		auto VB = reinterpret_cast<ID3D10Buffer*>(pVertexBuffer->m_VS[0].m_VertBuf.m_pPtr);
		SAFE_RELEASE(VB);
		//glDeleteVertexArrays(1, &pVertexBuffer->m_Container);
		for (int i = 0; i < VSF_NUM; i++)
		{
			SAFE_DELETE(pVertexBuffer->m_VS[i].m_VData);
		}
	}
}

void CBufferManager::Create(SVertexStream* dest, const void* src, int indexcount)
{
	assert(dest != nullptr);
	assert(src != nullptr);

	auto stream = new SVertexStream;
	stream->m_bDynamic	  = false;
	stream->m_nBufOffset  = 0;
	stream->m_nItems	  = indexcount;

	ID3D10Buffer* p_IB	   = (ID3D10Buffer*)(stream->m_VertBuf.m_pPtr);

	D3D10_BUFFER_DESC ibd;
	ibd.Usage		   = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = indexcount * sizeof(short); //sizeof(DWORD) * mNumFaces * 3;
	ibd.BindFlags	   = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags	   = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = src;
	if (GetDevice()->CreateBuffer(&ibd, &iinitData, &p_IB))
	{
	
	}

	*dest = *stream;
}

void CBufferManager::Release(SVertexStream* pVertexStream)
{
	if (pVertexStream != nullptr)
	{
		reinterpret_cast<ID3D10Buffer*>(pVertexStream->m_VertBuf.m_pPtr)->Release();
	}
}

void CBufferManager::Draw(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start, int vert_stop, CMatInfo* mi)
{
	assert(src != nullptr);
	auto to_draw = vert_stop - vert_start;
	auto offset_in_buffer = src->m_VS[VSF_GENERAL].m_nBufOffset;
	UINT stride = gVertexSize[src->m_vertexformat];
    UINT offset = 0;

	GetDevice()->IASetVertexBuffers(0, 1, reinterpret_cast<ID3D10Buffer* const*>(&src->m_VS[0].m_VertBuf.m_pPtr), &stride, &offset);
	GetDevice()->IASetPrimitiveTopology(ToDxPrimitive(static_cast<RenderPrimitive>(prmode)));

	if (indicies != nullptr)
	{
		assert(numindices != 0);
		auto s = sizeof(ushort);
		GetDevice()->DrawIndexed(numindices, offsindex * s + offset_in_buffer, vert_start + offset_in_buffer);
	}
	else
	{
		GetDevice()->Draw(to_draw == 0 ? src->m_NumVerts : to_draw, vert_start + offset_in_buffer);
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
		void* v = 0;
		auto hr = reinterpret_cast<ID3D10Buffer*>(dest->m_VS[0].m_VertBuf.m_pPtr)->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&v);
		if (FAILED(hr))
		{
			CryFatalError("Cannot map vertex buffer");
		}

		memcpy((char*)v + nOffs + dest->m_VS[VSF_GENERAL].m_nBufOffset, src, vertexcount * gVertexSize[dest->m_vertexformat]);
		reinterpret_cast<ID3D10Buffer*>(dest->m_VS[0].m_VertBuf.m_pPtr)->Unmap();
	}
	else
	{
		CryError("Origin buffer smaller");
	}
}

void CBufferManager::Update(SVertexStream* dest, const void* src, int indexcount, bool bUnLock)
{
	void* v = 0;
	auto hr = reinterpret_cast<ID3D10Buffer*>(dest->m_VertBuf.m_pPtr)->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&v);
	if (FAILED(hr))
	{
		CryFatalError("Cannot map vertex buffer");
	}

	memcpy(v, src, indexcount * SIZEOF_INDEX);
	reinterpret_cast<ID3D10Buffer*>(dest->m_VertBuf.m_pPtr)->Unmap();
}

IGraphicsDeviceConstantBuffer* CBufferManager::CreateConstantBuffer(int size)
{
	return nullptr;
}

#if 0
void CGraphicsDeviceConstantBuffer::SetData(const uint8* data, size_t size)
{
}
#endif