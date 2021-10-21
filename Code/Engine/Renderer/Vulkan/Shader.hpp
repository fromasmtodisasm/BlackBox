#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include "../Shaders/Effect.hpp"

enum class ShaderBinaryFormat
{
	SPIRV = 1,
	VENDOR = 1 << 2
};

using spirv_bin = std::vector<uint32>;

struct ICVar;

#if 0
using VertexShader = ID3D10VertexShader;
using GeometryShader = ID3D10GeometryShader;
using PixelShader = ID3D10PixelShader;

using PVertexShader = ID3D10VertexShader*;
using PGeometryShader = ID3D10GeometryShader*;
using PPixelShader = ID3D10PixelShader*;
#endif

class CHWShader : public NoCopy
{
public:
	VkShaderModule m_D3DShader;
	spirv_bin m_Bytecode;
	IShader::Type m_Type;

	CHWShader(VkShaderModule pShader, spirv_bin&& pCode, IShader::Type type) : m_D3DShader(pShader), m_Bytecode(pCode), m_Type(type)
	{
		//auto r = m_D3DShader->Release();
	}

	void Bind()
	{
		
	}
};

class CShader : public IShader
{
public:
	// Inherited via IShader
	~CShader();
	int GetID() override;
	void AddRef() override;
	int Release() override;
	IShader::Type GetType() override;
	const char* GetName() override;
	eVertexFormat GetVertexFormat(void) override;
	bool Reload() override;
	void Bind() override;

	void SaveBinaryShader(std::string_view name, int flags, uint64 nMaskGen);
	static CShader* LoadBinaryShader(std::string_view name, int flags, uint64 nMaskGen);

	static CHWShader* LoadFromEffect(PEffect pEffect, Type type);
	static CHWShader* Load(const std::string_view text, IShader::Type type, const char* pEntry, bool bFromMemory);
	static CHWShader* LoadFromFile(const std::string_view text, IShader::Type type, const char* pEntry);
	static CHWShader* LoadFromMemory(const std::vector<std::string>& text, IShader::Type type, const char* pEntry);

	std::array<CHWShader*, Type::E_NUM> m_Shaders{0};

	int m_NumRefs = 0;
};

CShader::Type str2typ(string type);
