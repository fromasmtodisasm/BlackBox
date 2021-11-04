#pragma once

#define D3DBaseBuffer ID3D10Resource
#define D3DResource ID3D10Resource

typedef uintptr_t DeviceBufferHandle;
typedef uint32    buffer_size_t;

class CDeviceTexture;
typedef CDeviceTexture* LPDEVICETEXTURE;

class CDeviceBuffer;
typedef CDeviceBuffer* LPDEVICEBUFFER;

////////////////////////////////////////////////////////////////////////////

struct SBufferLayout
{
	DXGI_FORMAT   m_eFormat;

	buffer_size_t m_elementCount;
	uint16        m_elementSize;

	uint32        m_eFlags;           // e.g. CDeviceObjectFactory::BIND_VERTEX_BUFFER
};

class CDeviceResource
{
public:
	D3DResource* GetNativeResource() const { return m_pD3DResource; }

  private:
	ID3D10Resource* m_pD3DResource;
};

class CDeviceBuffer : public CDeviceResource
{
	friend class CDeviceObjectFactory;
	// for native hand-made buffers
	size_t m_nBaseAllocatedSize;

	bool   m_bNoDelete;

#if (CRY_RENDERER_DIRECT3D >= 110) && (CRY_RENDERER_DIRECT3D < 120) && defined(USE_NV_API)
	void* m_handleMGPU;
#endif
#if (CRY_RENDERER_DIRECT3D >= 110) && (CRY_RENDERER_DIRECT3D < 120) && DEVRES_USE_PINNING
	SGPUMemHdl               m_gpuHdl;
#endif
#if (CRY_RENDERER_DIRECT3D >= 110) && (CRY_RENDERER_DIRECT3D < 120) && CRY_PLATFORM_DURANGO
	const SDeviceBufferDesc* m_pLayout;
#endif

public:
	static CDeviceBuffer*    Create(const SBufferLayout& pLayout, const void* pData);
	static CDeviceBuffer*    Associate(const SBufferLayout& pLayout, D3DResource* pBuf);

	SBufferLayout            GetLayout() const;
	// This calculates the hardware alignments of a texture resource, respecting block-compression and tiling mode (typeStride will round up if a fraction)
#if 0
	SResourceMemoryAlignment GetAlignment() const;
	SResourceDimension       GetDimension() const;
#endif

	inline D3DBaseBuffer*    GetBaseBuffer() const
	{
		return reinterpret_cast<D3DBaseBuffer*>(GetNativeResource());
	}
	inline D3DBuffer* GetBuffer() const
	{
		return reinterpret_cast<D3DBuffer*>(GetBaseBuffer());
	}

	void Unbind()
	{
		// Not implemented on any platform
	}

	virtual ~CDeviceBuffer();

	size_t GetDeviceSize() const
	{
		return m_nBaseAllocatedSize;
	}

	int32 Release()
	{
		int32 nRef = Cleanup();

		assert(nRef >= 0);

		if (nRef == 0 && !m_bNoDelete)
		{
			delete this;
		}

		return nRef;
	}

	void SetNoDelete(bool noDelete)
	{
		m_bNoDelete = noDelete;
	}

private:
	CDeviceBuffer() : m_nBaseAllocatedSize(0), m_bNoDelete(false)
#if (CRY_RENDERER_DIRECT3D >= 110) && (CRY_RENDERER_DIRECT3D < 120) && defined(USE_NV_API)
		, m_handleMGPU(NULL)
#endif
#if (CRY_RENDERER_DIRECT3D >= 110) && (CRY_RENDERER_DIRECT3D < 120) && CRY_PLATFORM_DURANGO
		, m_pLayout(NULL)
#endif
	{
	}

	CDeviceBuffer(const CDeviceBuffer&);
	CDeviceBuffer& operator=(const CDeviceBuffer&);



private:
	int32 Cleanup();
};

