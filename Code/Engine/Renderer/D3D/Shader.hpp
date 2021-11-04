#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include "../Shaders/Effect.hpp"
#include <d3dcompiler.h>

enum class ShaderBinaryFormat
{
	SPIRV  = 1,
	VENDOR = 1 << 2
};

struct ICVar;

using VertexShader	 = ID3D11VertexShader;
using GeometryShader = ID3D11GeometryShader;
using PixelShader	 = ID3D11PixelShader;

using PVertexShader	  = ID3D11VertexShader*;
using PGeometryShader = ID3D11GeometryShader*;
using PPixelShader	  = ID3D11PixelShader*;

class CHWShader : public NoCopy
{
  public:
	ID3D11Resource*		 m_D3DShader;
	_smart_ptr<ID3DBlob> m_Bytecode;
	IShader::Type		 m_Type;
	CHWShader(ID3D11Resource* pShader, _smart_ptr<ID3DBlob> pCode, IShader::Type type)
		: m_D3DShader(pShader)
		, m_Bytecode(pCode)
		, m_Type(type)
	{
		//auto r = m_D3DShader->Release();
	}

	void Bind()
	{
	}
};

class CShader : public IShader
{
	friend class ShaderMan;

  public:
	// Inherited via IShader
	~CShader();

	CShader& operator=(const CShader& src);

	virtual int			  GetID() override;
	virtual void		  AddRef() override;
	virtual void		  Release(bool bForce = false);
	virtual IShader::Type GetType() override;
	virtual const char*	  GetName() override;
	virtual void		  Bind() override;
	virtual int			  GetFlags() override;
	virtual int			  GetFlags2() override;

	bool WaitUntilLoaded();

	void			SaveBinaryShader(std::string_view name, int flags, uint64 nMaskGen);
	static CShader* LoadBinaryShader(std::string_view name, int flags, uint64 nMaskGen);

	void CreateInputLayout();
	void ReflectShader();

	static CHWShader* LoadFromEffect(PEffect pEffect, Type type, int technique = 0, int pass = 0);
	static CHWShader* Load(const std::string_view text, IShader::Type type, const char* pEntry, bool bFromMemory);
	static CHWShader* LoadFromFile(const std::string_view text, IShader::Type type, const char* pEntry);
	static CHWShader* LoadFromMemory(const std::vector<std::string>& text, IShader::Type type, const char* pEntry);

	std::array<CHWShader*, Type::E_NUM> m_Shaders{0};

	int m_NumRefs = 0;
	int m_Flags	  = 0;
	int m_Flags2  = 0;

	ID3D11InputLayout*		m_pInputLayout;
	D3D11_SHADER_DESC		m_Desc;
	ID3D11ShaderReflection* m_pReflection;
};

CShader::Type str2typ(string type);
