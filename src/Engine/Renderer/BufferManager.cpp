#include <BlackBox/Renderer/BufferManager.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>

namespace
{
	enum AttributeLocation : GLuint {
		position = 0,
		normal = 1,
		uv = 2,
		tangent = 3,
		btangent = 4,
		color = 5
	};
	struct VertexAttributePointer
	{
		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const void* pointer;

		VertexAttributePointer(
			GLuint index,
			GLint size,
			GLenum type,
			GLboolean normalized,
			GLsizei stride,
			const void* pointer
		)
			:
			index(index),
			size(size),
			type(type),
			normalized(normalized),
			stride(stride),
			pointer(pointer)
		{}

	};

#define OFFSET(m) reinterpret_cast<GLvoid*>(static_cast<INT_PTR>((gBufInfoTable[vertexFormat].m)))
	inline VertexAttributePointer GetPositionAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			position, 3, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat],	reinterpret_cast<GLvoid*>(0)
		);
	}
	inline VertexAttributePointer GetNormalAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			normal, 3, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat], OFFSET(OffsNormal)
		);
	}
	inline VertexAttributePointer GetColorAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			color, 4, GL_UNSIGNED_BYTE, GL_TRUE, gVertexSize[vertexFormat],	OFFSET(OffsColor)
		);
	}
	inline VertexAttributePointer GetUVAttributePointer(int vertexFormat)
	{
		return VertexAttributePointer(
			uv, 2, GL_FLOAT, GL_FALSE, gVertexSize[vertexFormat],	OFFSET(OffsTC)
		);
	}

	void SetAttribPointer(VertexAttributePointer vap)
	{
		gl::EnableVertexAttribArray(vap.index);
		gl::VertexAttribPointer(
				vap.index, vap.size, vap.type, vap.normalized, vap.stride, vap.pointer 
				);
	}
	void DisableAttributes(CVertexBuffer* vb)
	{
		SVertBufComps vbc;
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
		SVertBufComps vbc;
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

	GLenum toGlPrimitive(RenderPrimitive rp)
	{
		switch (rp)
		{
		case RenderPrimitive::LINES: return GL_LINES;
		case RenderPrimitive::LINE_LOOP: return GL_LINE_LOOP;
		case RenderPrimitive::LINE_STRIP: return GL_LINE_STRIP;
		case RenderPrimitive::TRIANGLES: return GL_TRIANGLES;
		case RenderPrimitive::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		default: return -1;
		}
	}

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
#if 0
  if (auto it = m_VertexBufferPool.find((eVertexFormat)vertexformat); it != m_VertexBufferPool.end())
  {
    auto &pool = it->second;
    if (pool.free > vertexcount* gVertexSize[vertexformat])
    {
      pool.vertexBuffer->m_VS[VSF_GENERAL].m_nBufOffset = pool.totalSize - pool.free;
      return pool.vertexBuffer;
    }
    assert(0 && "Out off buffer object memeory");
  }
  SVertexPoolEntry vpe;
  vpe.free = INIT_BUFFER_SIZE;
  vpe.totalSize = INIT_BUFFER_SIZE;
  vpe.vertexBuffer = buffer;
  m_VertexBufferPool.insert(std::make_pair((eVertexFormat)vertexformat, vpe));
#endif
	SVertexStream stream;
	CVertexBuffer *buffer = new CVertexBuffer;
	stream.m_bDynamic = bDynamic;
	stream.m_VData = CreateVertexBuffer(vertexformat, vertexcount);

	gl::CreateVertexArrays(1, &buffer->m_Container);
	gl::BindVertexArray(buffer->m_Container);
  gEnv->pLog->Log("Here");
    {
		gl::GenBuffer(&stream.m_VertBuf.m_nID);
		gl::BindBuffer(GL_ARRAY_BUFFER, stream.m_VertBuf.m_nID);
		gl::NamedBufferData(stream.m_VertBuf.m_nID, vertexcount * gVertexSize[vertexformat], nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		buffer->m_bDynamic = bDynamic;
		buffer->m_NumVerts = vertexcount;
		buffer->m_vertexformat = vertexformat;
		buffer->m_VS[VSF_GENERAL] = stream;
		//m_VertexBufferPool.push_back({ false, buffer });
		EnableAttributes(buffer);
    }
	gl::BindVertexArray(0);
	debuger::vertex_array_label(buffer->m_Container, szSource);

	return buffer;
}

void CBufferManager::Release(CVertexBuffer* pVertexBuffer)
{
    glDeleteBuffers(1, &pVertexBuffer->m_VS[0].m_VertBuf.m_nID);
    glDeleteVertexArrays(1, &pVertexBuffer->m_Container);
}

void CBufferManager::Create(SVertexStream* dest, const void* src, int indexcount)
{
	assert(dest != nullptr);
	assert(src != nullptr);

	SVertexStream *stream = new SVertexStream;
	stream->m_bDynamic = false;
	stream->m_nBufOffset = 0;
	stream->m_nItems = indexcount;

	gl::GenBuffer(&stream->m_VertBuf.m_nID);
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, stream->m_VertBuf.m_nID);
	gl::BufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(short), src, GL_STATIC_DRAW);
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  *dest = *stream;
}

void CBufferManager::Release(SVertexStream* pVertexStream)
{
	if (pVertexStream != nullptr)
	{
		glDeleteBuffers(1, &pVertexStream[0].m_VertBuf.m_nID);
	}
}

void CBufferManager::Draw(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start, int vert_stop, CMatInfo* mi)
{
	assert(src != nullptr);
	auto to_draw = vert_stop - vert_start;
	gl::BindVertexArray(src->m_Container);

  auto gl_mode = toGlPrimitive(static_cast<RenderPrimitive>(prmode));
  auto offset_in_buffer = src->m_VS[VSF_GENERAL].m_nBufOffset;
  if (indicies != nullptr)
  {
    assert(numindices != 0);
    auto s = sizeof(GLushort);
    gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies->m_VertBuf.m_nID);
    gl::DrawElements(gl_mode, numindices, GL_UNSIGNED_SHORT, reinterpret_cast<GLushort*>(offsindex * s + offset_in_buffer));
  }
  else
  {
    gl::DrawArrays(gl_mode, vert_start + offset_in_buffer, to_draw == 0 ? src->m_NumVerts : to_draw);
  }
	gl::BindVertexArray(0);
}

void CBufferManager::Update(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs, int Type)
{
	gl::NamedBufferSubData(dest->m_VS[VSF_GENERAL].m_VertBuf.m_nID, nOffs + dest->m_VS[VSF_GENERAL].m_nBufOffset, vertexcount * gVertexSize[dest->m_vertexformat], src);
}

void CBufferManager::Update(SVertexStream* dest, const void* src, int indexcount, bool bUnLock)
{
	gl::NamedBufferSubData(dest->m_VertBuf.m_nID, 0, indexcount * SIZEOF_INDEX, src);
}

IGraphicsDeviceConstantBuffer* CBufferManager::CreateConstantBuffer(int size)
{
  return nullptr;
}

void CGraphicsDeviceConstantBuffer::SetData(const uint8* data, size_t size)
{

}
