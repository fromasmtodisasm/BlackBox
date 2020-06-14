#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

struct SVertexPoolEntry
{
	std::vector<CVertexBuffer*> vertexBuffers;
  int totalSize = 0;
  int free = 0;
};

class CGraphicsDeviceConstantBuffer : public IGraphicsDeviceConstantBuffer
{
	void SetData(const uint8* data, size_t size) override;
};

class CBufferManager
{
  static const int SIZEOF_INDEX = sizeof(short);
  const int INIT_BUFFER_SIZE = (1024 * 1024) * 4; // 4 billion of vertices 
public:
  CBufferManager();
  ~CBufferManager();
  bool Preallocate();
  CVertexBuffer* Create(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false);
  void Release(CVertexBuffer* pVertexBuffer);
  CVertexBuffer* New(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false);
  void Create(SVertexStream* dest, const void* src, int indexcount);
  void Release(SVertexStream* pVertexStream);
  void Draw(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start = 0, int vert_sto = 0, CMatInfo* mi = NULL);
  void Update(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs/* = 0*/, int Type/* = 0*/);
  void Update(SVertexStream* dest, const void* src, int indexcount, bool bUnLock/* = true*/);
  IGraphicsDeviceConstantBuffer* CreateConstantBuffer(int size);
  
  std::map<eVertexFormat, SVertexPoolEntry> m_VertexBufferPool;
};
