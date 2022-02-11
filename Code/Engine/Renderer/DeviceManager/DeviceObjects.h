#pragma once

#include "DeviceResources.hpp"

struct SInputLayoutCompositionDescriptor
{
	const InputLayoutHandle VertexFormat;
	const uint8_t           StreamMask; // EStreamMasks
	const uint8_t           ShaderMask;
	const bool              bInstanced;

	static uint8_t          GenerateShaderMask(const InputLayoutHandle VertexFormat, D3DShaderReflection* pShaderReflection);

	SInputLayoutCompositionDescriptor(InputLayoutHandle VertexFormat, EStreamMasks Stream, D3DShaderReflection* pShaderReflection) noexcept
	    : VertexFormat(VertexFormat)
	    , StreamMask(static_cast<uint8_t>(Stream & VSM_MASK))
	    , ShaderMask(GenerateShaderMask(VertexFormat, pShaderReflection))
	    , bInstanced((Stream & VSM_INSTANCED) != 0)
	{
	}

	SInputLayoutCompositionDescriptor(SInputLayoutCompositionDescriptor&&)      = default;
	SInputLayoutCompositionDescriptor(const SInputLayoutCompositionDescriptor&) = default;

	bool operator==(const SInputLayoutCompositionDescriptor& rhs) const noexcept
	{
		return static_cast<size_t>(*this) == static_cast<size_t>(rhs);
	}
	bool operator!=(const SInputLayoutCompositionDescriptor& rhs) const noexcept { return !(*this == rhs); }

	struct hasher
	{
		size_t operator()(const SInputLayoutCompositionDescriptor& d) const noexcept
		{
			const auto x = static_cast<size_t>(d);
			return std::hash<size_t>()(x);
		}
	};

private:
	explicit operator size_t() const
	{
		return static_cast<size_t>(StreamMask) | (static_cast<size_t>(ShaderMask) << 8) | (static_cast<size_t>(VertexFormat.value) << 16) | (static_cast<size_t>(bInstanced) << 24);
	}
};

class CDeviceObjectFactory
{
	static CDeviceObjectFactory m_singleton;

	CDeviceObjectFactory();
	~CDeviceObjectFactory();

public:
	void         AssignDevice(D3DDevice* pDevice);
	static ILINE CDeviceObjectFactory& GetInstance()
	{
		return m_singleton;
	}

	// Low-level resource management API (TODO: remove D3D-dependency by abstraction)
	enum EResourceAllocationFlags : uint32
	{
		// for Create*Texture() only
		BIND_DEPTH_STENCIL               = BIT(0), // Bind flag
		BIND_RENDER_TARGET               = BIT(1), // Bind flag
		BIND_UNORDERED_ACCESS            = BIT(2), // Bind flag

		BIND_VERTEX_BUFFER               = BIT(3), // Bind flag, DX11+Vk
		BIND_INDEX_BUFFER                = BIT(4), // Bind flag, DX11+Vk
		BIND_CONSTANT_BUFFER             = BIT(5), // Bind flag, DX11+Vk

		BIND_SHADER_RESOURCE             = BIT(6), // Bind flag, any shader stage
		BIND_STREAM_OUTPUT               = BIT(7),

		// Bits [8, 15] free

		USAGE_UAV_READWRITE              = BIT(16), // Reading from UAVs is only possible through typeless formats under DX11
		USAGE_UAV_OVERLAP                = BIT(17), // Concurrent access to UAVs should be allowed
		USAGE_UAV_COUNTER                = BIT(18), // Allocate a counter resource for the UAV as well (size = ?)
		USAGE_AUTOGENMIPS                = BIT(19), // Generate mip-maps automatically whenever the contents of the resource change
		USAGE_STREAMING                  = BIT(20), // Use placed resources and allow changing LOD clamps for streamable textures
		USAGE_STAGE_ACCESS               = BIT(21), // Use persistent buffer resources to stage uploads/downloads to the texture
		USAGE_STRUCTURED                 = BIT(22), // Resource contains structured data instead of fundamental datatypes
		USAGE_INDIRECTARGS               = BIT(23), // Resource can be used for indirect draw/dispatch argument buffers
		USAGE_RAW                        = BIT(24), // Resource can be bound byte-addressable
		USAGE_LODABLE                    = BIT(25), // Resource consists of multiple LODs which can be selected at GPU run-time

		// for UMA or persistent MA only
		USAGE_DIRECT_ACCESS              = BIT(26),
		USAGE_DIRECT_ACCESS_CPU_COHERENT = BIT(27),
		USAGE_DIRECT_ACCESS_GPU_COHERENT = BIT(28),

		// The CPU access flags determine the heap on which the resource will be placed.
		// For porting old heap flags, you should use the following table to select the heap.
		// Note for CDevTexture: When USAGE_STAGE_ACCESS is set, the heap is always DEFAULT, and additional dedicated staging resources are created alongside.

		// CPU_READ | CPU_WRITE -> D3D HEAP
		//    no    |    no     -> DEFAULT
		//    no    |    yes    -> DYNAMIC
		//    yes   | yes or no -> STAGING
		USAGE_CPU_READ                   = BIT(29),
		USAGE_CPU_WRITE                  = BIT(30),

		USAGE_HIFREQ_HEAP                = BIT(31) // Resource is reallocated every frame or multiple times each frame, use a recycling heap with delayed deletes
	};

	// Resource Usage	| Default	| Dynamic	| Immutable	| Staging
	// -----------------+-----------+-----------+-----------+--------
	// GPU - Read       | yes       | yes1      | yes       | yes1, 2
	// GPU - Write      | yes1      |           |           | yes1, 2
	// CPU - Read       |           |           |           | yes1, 2
	// CPU - Write      |           | yes       |           | yes1, 2
	//
	// 1 - This is restricted to ID3D11DeviceContext::CopySubresourceRegion, ID3D11DeviceContext::CopyResource,
	//     ID3D11DeviceContext::UpdateSubresource, and ID3D11DeviceContext::CopyStructureCount.
	// 2 - Cannot be a depth - stencil buffer or a multi-sampled render target.

	//=============================================================================

	static uint8* CDeviceObjectFactory::Map(D3DBuffer* buffer, uint32 subresource, buffer_size_t offset, buffer_size_t size, D3D11_MAP mode);

	static void   CDeviceObjectFactory::Unmap(D3DBuffer* buffer, uint32 subresource, buffer_size_t offset, buffer_size_t size, D3D11_MAP mode);

	////////////////////////////////////////////////////////////////////////////
	// InputLayout API

	static void   AllocatePredefinedInputLayouts();
	static void   TrimInputLayouts();
	static void   ReleaseInputLayouts();

public:
	using SInputLayoutPair = std::pair<SInputLayout, CDeviceInputLayout*>;

	static const SInputLayout*     GetInputLayoutDescriptor(const InputLayoutHandle VertexFormat);

	// Higher level input-layout composition / / / / / / / / / / / / / / / / / /
	static SInputLayout            CreateInputLayoutForPermutation(const SShaderBlob* m_pConsumingVertexShader, const SInputLayoutCompositionDescriptor& compositionDescription, EStreamMasks StreamMask, const InputLayoutHandle VertexFormat);
	static const SInputLayoutPair* GetOrCreateInputLayout(const SShaderBlob* pVS, EStreamMasks StreamMask, const InputLayoutHandle VertexFormat);
	static const SInputLayoutPair* GetOrCreateInputLayout(const SShaderBlob* pVS, const InputLayoutHandle VertexFormat);
	static InputLayoutHandle       CreateCustomVertexFormat(size_t numDescs, const D3D11_INPUT_ELEMENT_DESC* inputLayout);

	// Shader API
	ID3D11VertexShader*            CreateVertexShader(const void* pData, size_t bytes);

	ID3D11PixelShader*             CreatePixelShader(const void* pData, size_t bytes);

	ID3D11GeometryShader*          CreateGeometryShader(const void* pData, size_t bytes);

	ID3D11HullShader*              CreateHullShader(const void* pData, size_t bytes);

	ID3D11DomainShader*            CreateDomainShader(const void* pData, size_t bytes);

	ID3D11ComputeShader*           CreateComputeShader(const void* pData, size_t bytes);

private:
	////////////////////////////////////////////////////////////////////////////
	// InputLayout API

	// A heap containing all permutations of InputLayout, they are global and are never evicted
	static CDeviceInputLayout*                                                                                                CreateInputLayout(const SInputLayout& pState, const SShaderBlob* m_pConsumingVertexShader);
	static std::vector<SInputLayout>                                                                                          m_vertexFormatToInputLayoutCache;

	// Higher level input-layout composition / / / / / / / / / / / / / / / / / /
	static std::unordered_map<SInputLayoutCompositionDescriptor, SInputLayoutPair, SInputLayoutCompositionDescriptor::hasher> s_InputLayoutCompositions;
};

static ILINE CDeviceObjectFactory& GetDeviceObjectFactory() { return CDeviceObjectFactory::GetInstance(); }
