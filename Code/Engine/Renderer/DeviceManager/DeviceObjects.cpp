#include "DeviceObjects.h"
#include "D3D\Renderer.h"
#include <BlackBox\Renderer\VertexFormats.hpp>
#include <BlackBox\System\FrameProfiler.hpp>

uint8_t SInputLayoutCompositionDescriptor::GenerateShaderMask(const InputLayoutHandle VertexFormat, D3DShaderReflection* pShaderReflection)
{
	uint8_t shaderMask = 0;

	D3D_SHADER_DESC Desc;
	pShaderReflection->GetDesc(&Desc);

	// layoutDescriptor's names will be ordered in lexicographical ascending order
	const auto* layoutDescriptor = CDeviceObjectFactory::GetInputLayoutDescriptor(VertexFormat);
	if (!layoutDescriptor || !Desc.InputParameters)
		return shaderMask;

	// Read and store lexicographically ordered pointers to reflected names
	std::vector<const char*> reflectedNames;
	reflectedNames.reserve(Desc.InputParameters);
	for (uint32 i = 0; i < Desc.InputParameters; i++)
	{
		D3D_SIGNATURE_PARAMETER_DESC Sig;
		pShaderReflection->GetInputParameterDesc(i, &Sig);
		if (!Sig.SemanticName)
			continue;

		// Insert ordered by element name
		auto it = std::lower_bound(reflectedNames.begin(), reflectedNames.end(), Sig.SemanticName, [](const char* const lhs, const char* const rhs)
								   { return ::strcmp(lhs, rhs) <= 0; });
		reflectedNames.insert(it, Sig.SemanticName);
	}

	// Check which of the names that are expected by the vertex format actually exist as input in the VS
	auto layout_it = layoutDescriptor->m_Declaration.cbegin();
	auto vs_it	   = reflectedNames.cbegin();
	for (int i = 0; layout_it != layoutDescriptor->m_Declaration.cend() && vs_it != reflectedNames.cend(); ++i, ++layout_it)
	{
		int compResult;
		while (vs_it != reflectedNames.cend() && (compResult = ::strcmp(*vs_it, layout_it->SemanticName)) < 0)
		{
			++vs_it;
		}

		// Match?
		if (compResult == 0)
		{
			shaderMask |= 1 << i;
			++vs_it;
		}
	}

	return shaderMask;
}

CDeviceObjectFactory CDeviceObjectFactory::m_singleton;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDeviceObjectFactory::CDeviceObjectFactory()
{
}

CDeviceObjectFactory::~CDeviceObjectFactory()
{
#if (CRY_RENDERER_DIRECT3D >= 120)
	/* No need to do anything */;
#elif (CRY_RENDERER_DIRECT3D >= 110)
	/* Context already deconstructed */;
#elif (CRY_RENDERER_VULKAN >= 10)
	/* No need to do anything */;
#endif
}



////////////////////////////////////////////////////////////////////////////
// InputLayout API

std::vector<SInputLayout> CDeviceObjectFactory::m_vertexFormatToInputLayoutCache;
std::unordered_map<SInputLayoutCompositionDescriptor, CDeviceObjectFactory::SInputLayoutPair, SInputLayoutCompositionDescriptor::hasher> CDeviceObjectFactory::s_InputLayoutCompositions;

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_Empty[] = {{}}; // Empty

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_C4B[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_C4B, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(SVF_P3F_C4B, color), D3D11_INPUT_PER_VERTEX_DATA, 0},
};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_N[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_T2F[] =
	{
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(SVF_T2F, st), D3D11_INPUT_PER_VERTEX_DATA, 0}};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_N_C4B[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N_C4B, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N_C4B, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(SVF_P3F_N_C4B, color), D3D11_INPUT_PER_VERTEX_DATA, 0},
};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_T2F[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_T2F, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(SVF_P3F_T2F, st), D3D11_INPUT_PER_VERTEX_DATA, 0}};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_N_T2F[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N_T2F, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N_T2F, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(SVF_P3F_N_T2F, st), D3D11_INPUT_PER_VERTEX_DATA, 0}};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_N_C4B_T2F[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N_C4B_T2F, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_N_C4B_T2F, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(SVF_P3F_N_C4B_T2F, color), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(SVF_P3F_N_C4B_T2F, st), D3D11_INPUT_PER_VERTEX_DATA, 0}};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_P3F_C4B_T2F[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SVF_P3F_C4B_T2F, xyz), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(SVF_P3F_C4B_T2F, color), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(SVF_P3F_C4B_T2F, st), D3D11_INPUT_PER_VERTEX_DATA, 0}};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_T3F_B4F_N3F[] =
	{
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, VSF_TANGENTS, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, VSF_TANGENTS, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, VSF_TANGENTS, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}};

static const D3D11_INPUT_ELEMENT_DESC VertexDecl_T4S_B4S[] =
	{
		{"TANGENT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, VSF_TANGENTS, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, VSF_TANGENTS, 8, D3D11_INPUT_PER_VERTEX_DATA, 0}};

static const struct
{
	size_t							numDescs;
	const D3D11_INPUT_ELEMENT_DESC* inputDescs;
} VertexDecls[eVertexFormat::VERTEX_FORMAT_NUMS] =
{
	{0, VertexDecl_Empty},
	{ARRAY_COUNT(VertexDecl_P3F), VertexDecl_P3F},
	{ARRAY_COUNT(VertexDecl_P3F_C4B), VertexDecl_P3F_C4B},
	{ARRAY_COUNT(VertexDecl_P3F_T2F), VertexDecl_P3F_T2F},
	{ARRAY_COUNT(VertexDecl_P3F_C4B_T2F), VertexDecl_P3F_C4B_T2F},
	{0, VertexDecl_Empty},
	{0, VertexDecl_Empty},
	{ARRAY_COUNT(VertexDecl_P3F_N), VertexDecl_P3F_N},
	{ARRAY_COUNT(VertexDecl_P3F_N_C4B), VertexDecl_P3F_N_C4B},
	{ARRAY_COUNT(VertexDecl_P3F_N_T2F), VertexDecl_P3F_N_T2F},
	{ARRAY_COUNT(VertexDecl_P3F_N_C4B_T2F), VertexDecl_P3F_N_C4B_T2F},
	{0, VertexDecl_Empty},
	{0, VertexDecl_Empty},
	{0, VertexDecl_Empty},
	{0, VertexDecl_Empty},
	{ARRAY_COUNT(VertexDecl_T2F), VertexDecl_T2F},
	{0, VertexDecl_Empty},

};

void CDeviceObjectFactory::AllocatePredefinedInputLayouts()
{
	m_vertexFormatToInputLayoutCache.reserve(eVertexFormat::VERTEX_FORMAT_NUMS);
	for (size_t i = 0; i < eVertexFormat::VERTEX_FORMAT_NUMS; ++i)
		CreateCustomVertexFormat(VertexDecls[i].numDescs, VertexDecls[i].inputDescs);
}

void CDeviceObjectFactory::ReleaseInputLayouts()
{
	m_vertexFormatToInputLayoutCache.clear();
}

void CDeviceObjectFactory::TrimInputLayouts()
{
	m_vertexFormatToInputLayoutCache.erase(m_vertexFormatToInputLayoutCache.begin() + eVertexFormat::VERTEX_FORMAT_NUMS, m_vertexFormatToInputLayoutCache.end());
}

const SInputLayout* CDeviceObjectFactory::GetInputLayoutDescriptor(const InputLayoutHandle VertexFormat)
{
	uint64_t idx = static_cast<uint64_t>(VertexFormat);
	const auto size = m_vertexFormatToInputLayoutCache.size();
	if (idx >= size)
	{
		CryWarning(EValidatorModule::VALIDATOR_MODULE_RENDERER, EValidatorSeverity::VALIDATOR_ERROR, "GetInputLayoutDescriptor(): Supplied handle is neither a predefined format nor was created via CreateCustomVertexFormat()!");
		return nullptr;
	}

	return &m_vertexFormatToInputLayoutCache[static_cast<size_t>(idx)];
}

const CDeviceObjectFactory::SInputLayoutPair* CDeviceObjectFactory::GetOrCreateInputLayout(const SShaderBlob* pVertexShader, EStreamMasks StreamMask, const InputLayoutHandle VertexFormat)
{
	CRY_ASSERT(pVertexShader);
	if (!pVertexShader)
		return nullptr;

	// Reflect
	void* pShaderReflBuf;

	{
		FRAME_PROFILER("D3DReflect", PROFILE_RENDERER);
		HRESULT hr = D3DReflection(pVertexShader->m_pShaderData, pVertexShader->m_nDataSize, IID_D3DShaderReflection, &pShaderReflBuf);
		CRY_VERIFY(SUCCEEDED(hr) && pShaderReflBuf);
	}

	D3DShaderReflection* pShaderReflection = (D3DShaderReflection*)pShaderReflBuf;

	// Create the composition descriptor
	SInputLayoutCompositionDescriptor compositionDescriptor(VertexFormat, StreamMask, pShaderReflection);

	auto it = s_InputLayoutCompositions.find(compositionDescriptor);
	if (it == s_InputLayoutCompositions.end() || it->first != compositionDescriptor)
	{
		// Create the input layout for the current permutation
		auto il = CreateInputLayoutForPermutation(pVertexShader, compositionDescriptor, StreamMask, VertexFormat);
		auto deviceInputLayout = CreateInputLayout(il, pVertexShader);

		auto pair = std::make_pair(il, deviceInputLayout);
		// Insert with hint
		return &s_InputLayoutCompositions.insert(it, std::make_pair(compositionDescriptor, pair))->second;
	}

	SAFE_RELEASE(pShaderReflection);

	return &it->second;
}

const CDeviceObjectFactory::SInputLayoutPair* CDeviceObjectFactory::GetOrCreateInputLayout(const SShaderBlob* pVS, const InputLayoutHandle VertexFormat)
{
	return GetOrCreateInputLayout(pVS, VSM_NONE, VertexFormat);
}

SInputLayout CDeviceObjectFactory::CreateInputLayoutForPermutation(const SShaderBlob* m_pConsumingVertexShader, const SInputLayoutCompositionDescriptor &compositionDescription, EStreamMasks StreamMask, const InputLayoutHandle VertexFormat)
{
	bool bInstanced = (StreamMask & VSM_INSTANCED) != 0;

	const auto* layoutDescriptor = GetInputLayoutDescriptor(VertexFormat);
	CRY_ASSERT(layoutDescriptor);
	if (!layoutDescriptor)
		return SInputLayout({});

	// Copy layout declarations that also exists in shader, using the shaderMask of the composition description
	std::vector<D3D11_INPUT_ELEMENT_DESC> decs;
	decs.reserve(layoutDescriptor->m_Declaration.size());
	for (int i=0; i<layoutDescriptor->m_Declaration.size(); ++i)
			decs.push_back(layoutDescriptor->m_Declaration[i]);

	// Create instanced vertex declaration
	if (bInstanced)
	{
		for (size_t n = 0; n < decs.size(); n++)
		{
			D3D11_INPUT_ELEMENT_DESC& elem = decs[n];

			elem.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elem.InstanceDataStepRate = 1;
		}
	}

	// Append additional streams
	for (int n = 1; n < VSF_NUM; n++)
	{
		if (!(StreamMask & (1 << n)))
			continue;

		InputLayoutHandle AttachmentFormat = 0;
		switch (n)
		{
		case VSF_TANGENTS: AttachmentFormat = eVertexFormat::VERTEX_FORMAT_T3F_B3F_N3F; break;
			#if 0
		case VSF_QTANGENTS: AttachmentFormat = EDefaultInputLayouts::Q4F; break;
			#endif
			#ifdef ENABLE_NORMALSTREAM_SUPPORT
		case VSF_NORMALS: AttachmentFormat = eVertexFormat::VERTEX_FORMAT_N3F; break;
			#endif
		}

		const auto* addLayout = GetInputLayoutDescriptor(AttachmentFormat);
		CRY_ASSERT(addLayout);
		if (addLayout)
		{
			for (size_t n = 0; n < addLayout->m_Declaration.size(); n++)
				decs.push_back(addLayout->m_Declaration[n]);
		}
	}

	return SInputLayout(std::move(decs));
}

InputLayoutHandle CDeviceObjectFactory::CreateCustomVertexFormat(size_t numDescs, const D3D11_INPUT_ELEMENT_DESC* inputLayout)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> decs;
	for (int n = 0; n < numDescs; ++n) 
	{
		// Insert ordered by element name
		auto it = std::lower_bound(decs.begin(), decs.end(), inputLayout[n], [](const D3D11_INPUT_ELEMENT_DESC& lhs, const D3D11_INPUT_ELEMENT_DESC& rhs)
		{
			return ::strcmp(lhs.SemanticName, rhs.SemanticName) <= 0;
		});
		decs.insert(it, inputLayout[n]);
	}

	// Find existing vertex format or store a new one
	auto it = std::find(m_vertexFormatToInputLayoutCache.begin(), m_vertexFormatToInputLayoutCache.end(), decs);
	auto idx = it - m_vertexFormatToInputLayoutCache.begin();
	if (it == m_vertexFormatToInputLayoutCache.end())
	{
		m_vertexFormatToInputLayoutCache.emplace_back(std::move(decs));
	}

	return InputLayoutHandle(static_cast<uint8_t>(idx));
}

////////////////////////////////////////////////////////////////////////////
// InputLayout API

CDeviceInputLayout* CDeviceObjectFactory::CreateInputLayout(const SInputLayout& pLayout, const SShaderBlob* m_pConsumingVertexShader)
{
	if (!m_pConsumingVertexShader || !m_pConsumingVertexShader->m_pShaderData)
		return nullptr;

	const int   nSize = m_pConsumingVertexShader->m_nDataSize;
	const void* pVSData = m_pConsumingVertexShader->m_pShaderData;

	CDeviceInputLayout* Layout;
	HRESULT hr = E_FAIL;
	#if 0
	if (FAILED(hr = /*gcpRendD3D->*/GetDevice()->CreateInputLayout(&pLayout.m_Declaration[0], pLayout.m_Declaration.size(), pVSData, nSize, &Layout)))
	#else
	if (FAILED(hr = GetDevice()->CreateInputLayout(&pLayout.m_Declaration[0], pLayout.m_Declaration.size(), pVSData, nSize, &Layout)))
	#endif
	{
		CRY_ASSERT(false);
		return Layout;
	}

	return Layout;
}



// Shader API

uint8* CDeviceObjectFactory::Map(D3DBuffer* buffer, uint32 subresource, buffer_size_t offset, buffer_size_t size, D3D11_MAP mode)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource = {0};
	/*gcpRendD3D->*/GetDeviceContext()->Map(buffer, subresource, mode, 0, &mapped_resource);
	return reinterpret_cast<uint8*>(mapped_resource.pData);
}

void CDeviceObjectFactory::Unmap(D3DBuffer* buffer, uint32 subresource, buffer_size_t offset, buffer_size_t size, D3D11_MAP mode)
{
	/*gcpRendD3D->*/GetDeviceContext()->Unmap(buffer, subresource);
}

ID3D11VertexShader* CDeviceObjectFactory::CreateVertexShader(const void* pData, size_t bytes)
{
	ID3D11VertexShader* pResult;
	return SUCCEEDED(/*gcpRendD3D->*/GetDevice()->CreateVertexShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

ID3D11PixelShader* CDeviceObjectFactory::CreatePixelShader(const void* pData, size_t bytes)
{
	ID3D11PixelShader* pResult;
	return SUCCEEDED(/*gcpRendD3D->*/GetDevice()->CreatePixelShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

ID3D11GeometryShader* CDeviceObjectFactory::CreateGeometryShader(const void* pData, size_t bytes)
{
	ID3D11GeometryShader* pResult;
	return SUCCEEDED(/*gcpRendD3D->*/GetDevice()->CreateGeometryShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

ID3D11HullShader* CDeviceObjectFactory::CreateHullShader(const void* pData, size_t bytes)
{
	ID3D11HullShader* pResult;
	return SUCCEEDED(/*gcpRendD3D->*/GetDevice()->CreateHullShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

ID3D11DomainShader* CDeviceObjectFactory::CreateDomainShader(const void* pData, size_t bytes)
{
	ID3D11DomainShader* pResult;
	return SUCCEEDED(/*gcpRendD3D->*/GetDevice()->CreateDomainShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}

ID3D11ComputeShader* CDeviceObjectFactory::CreateComputeShader(const void* pData, size_t bytes)
{
	ID3D11ComputeShader* pResult;
	return SUCCEEDED(/*gcpRendD3D->*/GetDevice()->CreateComputeShader(pData, bytes, nullptr, &pResult)) ? pResult : nullptr;
}
