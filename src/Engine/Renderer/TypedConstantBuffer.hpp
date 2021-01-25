#pragma once

#include "BufferManager.hpp"
//#include "D3D/Renderer.h"

class CD3DRenderer;

template<typename T, size_t Alignment=16>
class CTypedConstantBuffer : private NoCopy
{
protected:
	T&                 m_hostBuffer;
	CConstantBufferPtr m_constantBuffer;

private:
	// NOTE: enough memory to hold an aligned struct size + the adjustment of a possible unaligned start
	uint8              m_hostMemory[((sizeof(T) + (Alignment - 1)) & (~(Alignment - 1))) + (Alignment - 1)];
	T& AlignHostBuffer() { return *reinterpret_cast<T*>(Align(uintptr_t(m_hostMemory), Alignment)); }

public:
	CTypedConstantBuffer() : m_hostBuffer(AlignHostBuffer()) { ZeroStruct(m_hostBuffer); }
	CTypedConstantBuffer(const CTypedConstantBuffer<T, Alignment>& cb) : m_hostBuffer(AlignHostBuffer()), m_constantBuffer(nullptr) { m_hostBuffer = cb.m_hostBuffer; }
	CTypedConstantBuffer(CConstantBufferPtr incb) : m_hostBuffer(AlignHostBuffer()), m_constantBuffer(incb) {}

	void Clear()
	{
		m_constantBuffer.reset();
	}

	bool               IsDeviceBufferAllocated() { return m_constantBuffer != nullptr; }
	CConstantBufferPtr GetDeviceConstantBuffer()
	{
		if (!m_constantBuffer)
		{
			CreateDeviceBuffer();
		}
		return m_constantBuffer;
	}

	static CD3DRenderer* ForceTwoPhase() { extern CD3DRenderer gRenderer; return &gRenderer; }

	void CreateDeviceBuffer()
	{
		int size = sizeof(T);
		m_constantBuffer = ForceTwoPhase()->m_DevBufMan.CreateConstantBuffer(size); // TODO: this could be a good candidate for dynamic=false
		CopyToDevice();
	}
	
	void CopyToDevice()
	{
		#if 0
		m_constantBuffer->UpdateBuffer(&m_hostBuffer, Align(sizeof(m_hostBuffer), Alignment));
		#endif
	}

	void UploadZeros()
	{
		ZeroStruct(m_hostBuffer);
		CopyToDevice();
	}

	T*       operator->()       { return &m_hostBuffer; }
	const T* operator->() const { return &m_hostBuffer; }

	T&       operator=(const T& hostData)
	{
		return m_hostBuffer = hostData;
	}
};
