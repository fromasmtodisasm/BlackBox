#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/BufferManager.hpp>

namespace
{
	struct INPUT_ELEMENT_DESC
	{
		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const void* pointer;

		INPUT_ELEMENT_DESC(
			GLuint index,
			GLint size,
			GLenum type,
			GLboolean normalized,
			GLsizei stride,
			const void* pointer)
			: index(index),
			  size(size),
			  type(type),
			  normalized(normalized),
			  stride(stride),
			  pointer(pointer)
		{
		}
	};

	struct ShaderInputInterface
	{
		const char* semantic;
		GLenum type;
	};
	enum D3D11_INPUT_CLASSIFICATION
	{
		D3D11_INPUT_PER_VERTEX_DATA,
		D3D11_INPUT_PER_INSTANCE_DATA
	};
	typedef struct D3D11_INPUT_ELEMENT_DESC
	{
		LPCSTR SemanticName;
		UINT SemanticIndex;
		GLenum Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
	} D3D11_INPUT_ELEMENT_DESC;

	ShaderInputInterface testInterface[] = {
		{"POSITION", GL_FLOAT_VEC4},
		{"TEXCOORD", GL_FLOAT_VEC2},
		{"COLOR", GL_FLOAT_VEC4}
	};

	struct SVF_P3F_T2F_T3F
	{
		glm::vec3 p;		
		glm::vec2 st0;		
		glm::vec3 st1;		
	};

	static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_T2F_T3F[] =
		{
			{"POSITION", 0, GL_FLOAT_VEC3, 0, offsetof(SVF_P3F_T2F_T3F, p), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, GL_FLOAT_VEC2, 0, offsetof(SVF_P3F_T2F_T3F, st0), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 1, GL_FLOAT_VEC4, 0, offsetof(SVF_P3F_T2F_T3F, st1), D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

	void BindAttributes(ShaderInputInterface* shader)
	{
			
	}

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

	GLenum ToGlPrimitive(RenderPrimitive rp)
	{
		switch (rp)
		{
		case RenderPrimitive::LINES:
			return GL_LINES;
		case RenderPrimitive::LINE_LOOP:
			return GL_LINE_LOOP;
		case RenderPrimitive::LINE_STRIP:
			return GL_LINE_STRIP;
		case RenderPrimitive::TRIANGLES:
			return GL_TRIANGLES;
		case RenderPrimitive::TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		}
		return -1;
	}
} // namespace

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
	auto buffer = new CVertexBuffer;
	stream.m_bDynamic	  = bDynamic;
	stream.m_VData		  = CreateVertexBuffer(vertexformat, vertexcount);

	gl::CreateVertexArrays(1, &buffer->m_Container);
	gl::BindVertexArray(buffer->m_Container);
	{
		gl::GenBuffer(&stream.m_VertBuf.m_nID);
		gl::BindBuffer(GL_ARRAY_BUFFER, stream.m_VertBuf.m_nID);
		gl::NamedBufferData(stream.m_VertBuf.m_nID, vertexcount * gVertexSize[vertexformat], nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		buffer->m_bDynamic		  = bDynamic;
		buffer->m_NumVerts		  = vertexcount;
		buffer->m_vertexformat	  = vertexformat;
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
	for (int i = 0; i < VSF_NUM; i++)
	{
		SAFE_DELETE(pVertexBuffer->m_VS[i].m_VData);
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

	auto gl_mode		  = ToGlPrimitive(static_cast<RenderPrimitive>(prmode));
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
	if (vertexcount <= dest->m_NumVerts)
	{
		gl::NamedBufferSubData(dest->m_VS[VSF_GENERAL].m_VertBuf.m_nID, nOffs + dest->m_VS[VSF_GENERAL].m_nBufOffset, vertexcount * gVertexSize[dest->m_vertexformat], src);
	}
	else
	{
		gl::NamedBufferData(dest->m_VS[VSF_GENERAL].m_VertBuf.m_nID, vertexcount * gVertexSize[dest->m_vertexformat], src, GL_DYNAMIC_DRAW);
	}
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
