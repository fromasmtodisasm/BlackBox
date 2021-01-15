#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <map>
#include <memory>
#include <string>

enum class ShaderBinaryFormat
{
	SPIRV = 1,
	VENDOR = 1 << 2
};

struct ICVar;

class CShader;
class CBaseShaderProgram;
class CShaderProgram;

class CShader : public IShader
{
public:
	// Inherited via IShader
	virtual int GetID() override;
	virtual void AddRef() override;
	virtual int Release() override;
	virtual IShader::Type GetType() override;
	virtual const char* GetName() override;
	virtual eVertexFormat GetVertexFormat(void) override;
};

CShader::Type str2typ(string type);
