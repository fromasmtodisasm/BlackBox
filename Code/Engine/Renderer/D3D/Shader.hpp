#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

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

enum ED3DShError
{
	ED3DShError_NotCompiled,
	ED3DShError_CompilingError,
	ED3DShError_Fake,
	ED3DShError_Ok,
	ED3DShError_Compiling,
};

class CHWShader : public NoCopy
{
  public:
	_smart_ptr<ID3DBlob> m_ByteCode{};
	ID3D11Resource*		 m_D3DShader{};
	IShader::Type		 m_Type{};
	string				 m_EntryFunc;
	CHWShader(IShader::Type type, string entry)
		: m_Type(type)
		, m_EntryFunc(entry)
	{
		//auto r = m_D3DShader->Release();
	}

	void Bind()
	{
	}

	bool Upload(ID3DBlob* pBlob, CShader* pSH);
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

	virtual DynVertexFormat*	   GetDynVertexFormat()
	{
		#if 0
		return &format;
		#else
		return nullptr;
		#endif
	}

	bool WaitUntilLoaded();

	void			SaveBinaryShader(std::string_view name, int flags, uint64 nMaskGen);
	static CShader* LoadBinaryShader(std::string_view name, int flags, uint64 nMaskGen);

	void CreateInputLayout();
	void ReflectShader();

	static bool LoadFromEffect(CShader* pSH, PEffect pEffect, int technique = 0, int pass = 0);
	static std::pair<ID3DBlob*,ID3DBlob*> Load(const std::string_view text, IShader::Type type, const char* pEntry, bool bFromMemory);
	static CHWShader* LoadFromFile(const std::string_view text, IShader::Type type, const char* pEntry);
	static std::pair<ID3DBlob*,ID3DBlob*> LoadFromMemory(const std::vector<std::string>& text, IShader::Type type, const char* pEntry);


	string m_NameShader;
	string m_NameFile;
	int	   m_NumRefs = 0;
	int	   m_Flags	 = 0;
	int	   m_Flags2	 = 0;

	ID3D11InputLayout*		m_pInputLayout;
	D3D11_SHADER_DESC		m_Desc;
	ID3D11ShaderReflection* m_pReflection;
	DynVertexFormat		format;
	std::array<CHWShader*, Type::E_NUM> m_Shaders{0};
};
