#pragma once

#if !defined(D3DBuffer)
	#define D3DBuffer void
#endif
#if !defined(D3DShaderResource)
	#define D3DShaderResource void
#endif
#if !defined(D3DSamplerState)
	#define D3DSamplerState void
#endif

#include <BlackBox/Renderer/IRender.hpp>
#include "DeviceManager/DeviceResources.hpp"
#include <BlackBox\Core\Math\Cry_Math.h>

#include "DeviceManager\DeviceObjects.h"

enum AttributeLocation : uint {
	position = 0,
	normal = 1,
	uv = 2,
	tangent = 3,
	btangent = 4,
	color = 5
};

struct SVertexPoolEntry
{
	std::vector<CVertexBuffer*> vertexBuffers;
  int totalSize = 0;
  int free = 0;
};

typedef uintptr_t buffer_handle_t;
typedef uint32    item_handle_t;

////////////////////////////////////////////////////////////////////////////////////////
// Constant buffer wrapper class
class CConstantBuffer
{
public:
	CDeviceBuffer* m_buffer;
#if CONSTANT_BUFFER_ENABLE_DIRECT_ACCESS
	void*          m_allocator;
#endif
	void*          m_base_ptr;
	item_handle_t  m_handle;
	buffer_size_t  m_offset;
	buffer_size_t  m_size;
	int32          m_nRefCount;
	int8           m_nUpdCount;
	union
	{
		struct
		{
			uint8 m_used              : 1;
			uint8 m_lock              : 1;
			uint8 m_dynamic           : 1;
		};

		uint8 m_clearFlags;
	};

	CConstantBuffer(uint32 handle);
	~CConstantBuffer();

	inline void AddRef()
	{
		//CryInterlockedIncrement(&m_nRefCount);
		m_nRefCount++;
	}

	inline void Release()
	{
		/*if (CryInterlockedDecrement(&m_nRefCount) == 0)
			ReturnToPool();*/

		if (--m_nRefCount == 0)
			delete this;
	}
	
	inline void SetDebugName(const char* name) const
	{
		if (!m_buffer) return;
		// If we have CB direct access we only get a range-fragment instead of our own object which we can name
#if !CONSTANT_BUFFER_ENABLE_DIRECT_ACCESS
		//return m_buffer->SetDebugName(name);
		//return nullptr;
#endif
	}

	inline D3DBuffer* GetD3D() const
	{
		return m_buffer->GetBuffer();
	}

	inline D3DBuffer* GetD3D(buffer_size_t* offset, buffer_size_t* size) const
	{
		*offset = m_offset;
		*size = m_size;
		return m_buffer->GetBuffer();
	}

	inline uint64 GetCode() const
	{
#if CONSTANT_BUFFER_ENABLE_DIRECT_ACCESS
		uint64 code = reinterpret_cast<uintptr_t>(m_buffer) ^ SwapEndianValue((uint64)m_offset, true);
		return code;
#else
		return reinterpret_cast<uint64>(m_buffer);
#endif
	}

	void* BeginWrite()
	{
		//CConditonalDevManLock lock(&gcpRendD3D->m_DevBufMan, m_lock);
		if (!m_used)
		{
			const SBufferLayout Layout =
				{
					DXGI_FORMAT_UNKNOWN, m_size, 1,
					(m_dynamic ? CDeviceObjectFactory::USAGE_CPU_WRITE : 0) |
						CDeviceObjectFactory::BIND_CONSTANT_BUFFER};

			if (!(m_buffer = CDeviceBuffer::Create(Layout, nullptr)))
			{
				CryLogAlways("CConstantBuffer::BeginWrite: could not allocate buffer of size %" PRISIZE_T, m_size);
				return nullptr;
			}

			m_used = 1;
		}
		if (m_dynamic)
		{
			return CDeviceObjectFactory::Map(m_buffer->GetBuffer(), 0, m_offset, 0, D3D11_MAP_WRITE_DISCARD_CB);
		}
		else
		{
			return m_base_ptr = new char[m_size];
		}
		return NULL;
	}

	void  EndWrite(bool requires_flush = false)
	{
#if CONSTANT_BUFFER_ENABLE_DIRECT_ACCESS
		if (m_used)
		{
			// Transfer sub-set of GPU resource to CPU, also allows graphics debugger and multi-gpu broadcaster to do the right thing
			CDeviceObjectFactory::MarkWriteRange(m_buffer->GetBuffer(), m_offset, m_size, D3D11_MAP_WRITE);

#	if CONSTANT_BUFFER_ENABLE_ALLOCATOR_MAPPING
			auto pAllocator = static_cast<CPartitionAllocator*>(m_allocator);
			pAllocator->unmap();
#	endif
		}
#else
		//if (m_dynamic)
		{
			CDeviceObjectFactory::Unmap(m_buffer->GetBuffer(), 0, m_offset, m_size, D3D11_MAP_WRITE_DISCARD_CB);
		}
		//else
		//{
		//	const SResourceMemoryAlignment& layout = {sizeof(char), m_size, 0, 0};
		//	GetDeviceObjectFactory().GetCoreCommandList().GetCopyInterface()->Copy(m_base_ptr, this, layout);
		//	delete[] alias_cast<char*>(m_base_ptr);
		//	m_base_ptr = nullptr;
		//}
#endif
		m_nUpdCount++;
	}



	bool  UpdateBuffer(const void* src, buffer_size_t size, buffer_size_t offset = 0, int numDataBlocks = 1); // See CDeviceManager::UploadContents for details on numDataBlocks

	bool  IsNullBuffer() const { return m_size == 0; }

private:
	void  ReturnToPool();
};
typedef _smart_ptr<CConstantBuffer> CConstantBufferPtr;

#if 0
// Wrapper on the Constant buffer, allowing using it from outside of renderer and RenderThread
class CGraphicsDeviceConstantBuffer : public IGraphicsDeviceConstantBuffer
{
public:
	CGraphicsDeviceConstantBuffer() : m_size(0), m_bDirty(false) {}
	virtual void       SetData(const uint8* data, size_t size) override;
	// Should only be called from the thread that can create and update constant buffers
	CConstantBufferPtr GetConstantBuffer();
	CConstantBufferPtr GetNullConstantBuffer();
	buffer_size_t      GetSize() const { return m_size; }

protected:
	//virtual void DeleteThis() const override { delete this; }

private:
	buffer_size_t                                      m_size;
	stl::aligned_vector<uint8, CRY_PLATFORM_ALIGNMENT> m_data;
	CConstantBufferPtr                                 m_pConstantBuffer;
	bool                                               m_bDirty;

	static CryCriticalSection                          s_accessLock;
};
#endif

class CBufferManager
{
  static const int SIZEOF_INDEX = sizeof(short);
  const int INIT_BUFFER_SIZE = (1024 * 1024) * 4; // 4 billion of vertices 
public:
  CBufferManager();
  ~CBufferManager();
  bool Preallocate();

  	////////////////////////////////////////////////////////////////////////////////////////
  // ConstantBuffer creation
  inline CConstantBufferPtr CreateConstantBuffer(buffer_size_t size, bool dynamic = true, bool needslock = false)
  {
	  CConstantBufferPtr result;
	  result = CreateConstantBufferRaw(size, dynamic, needslock);
	  return result;
  }
  CConstantBuffer* CreateConstantBufferRaw(buffer_size_t size, bool dynamic, bool ts)
  {
	  size = (std::max(size, buffer_size_t(1u)) + (255)) & ~(255);
	  //CConditonalDevManLock lock(this, ts);
#if DEVBUFFERMAN_CCONSTANTBUFFER_POOL
	  CConstantBuffer* buffer = &s_PoolManager.m_constant_buffers[ts][s_PoolManager.m_constant_buffers[ts].Allocate()];
#else
	  CConstantBuffer* buffer = new CConstantBuffer(0xCACACACA);
#endif
	  buffer->m_size	= size;
	  buffer->m_dynamic = dynamic;
	  buffer->m_lock	= ts;
	  return buffer;
  }



  static CVertexBuffer* Create(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false);
  static void Release(CVertexBuffer* pVertexBuffer);
  static void Create(SVertexStream* dest, const void* src, int indexcount);
  void Release(SVertexStream* pVertexStream);
  void Draw(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start = 0, int vert_sto = 0, CMatInfo* mi = NULL);
  void Update(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs/* = 0*/, int Type/* = 0*/);
  void Update(SVertexStream* dest, const void* src, int indexcount, bool bUnLock/* = true*/);
  IGraphicsDeviceConstantBuffer* CreateConstantBuffer(int size);
  
  std::map<eVertexFormat, SVertexPoolEntry> m_VertexBufferPool;

  void MemoryUsage(ICrySizer* pSizer);
};
