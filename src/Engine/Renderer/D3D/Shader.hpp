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

struct ICVar;

class CHWShader
{
public:
	IUnknown* m_D3DShader;
	CHWShader(IUnknown* pShader) : m_D3DShader(pShader)
	{
		auto r = m_D3DShader->Release();
	}
};

class CShader : public IShader
{
public:
	// Inherited via IShader
	~CShader();
	virtual int GetID() override;
	virtual void AddRef() override;
	virtual int Release() override;
	virtual IShader::Type GetType() override;
	virtual const char* GetName() override;
	virtual eVertexFormat GetVertexFormat(void) override;
	virtual bool Reload() override;

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
