#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <d3d10.h>
#include "../Shaders/Effect.hpp"
#include <d3dcompiler.h>

enum class ShaderBinaryFormat
{
	SPIRV = 1,
	VENDOR = 1 << 2
};

struct ICVar;

using VertexShader = ID3D10VertexShader;
using GeometryShader = ID3D10GeometryShader;
using PixelShader = ID3D10PixelShader;

using PVertexShader = ID3D10VertexShader*;
using PGeometryShader = ID3D10GeometryShader*;
using PPixelShader = ID3D10PixelShader*;

class CHWShader : public NoCopy
{
public:
	ID3D10Resource* m_D3DShader;
	_smart_ptr<ID3DBlob> m_Bytecode;
	IShader::Type m_Type;
	CHWShader(ID3D10Resource* pShader, _smart_ptr<ID3DBlob> pCode, IShader::Type type) : m_D3DShader(pShader), m_Bytecode(pCode), m_Type(type)
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
	virtual int GetID() override;
	virtual void AddRef() override;
	virtual int Release() override;
	virtual IShader::Type GetType() override;
	virtual const char* GetName() override;
	virtual eVertexFormat GetVertexFormat(void) override;
	virtual bool Reload() override;
	virtual void Bind() override;

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
