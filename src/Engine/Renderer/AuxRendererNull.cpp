#include "AuxRendererNull.hpp"

bool CTextureNull::load(const char* name)
{
	return false;
}

void CTextureNull::bind()
{
}

void CTextureNull::setUnit(uint unit)
{
}

uint CTextureNull::getUnit() const
{
	return uint();
}

void CTextureNull::setType(const char* TextureType)
{
}

const char* CTextureNull::typeToStr()
{
	return nullptr;
}

int CTextureNull::getWidth() const
{
	return 0;
}

int CTextureNull::getHeight() const
{
	return 0;
}

int CTextureNull::getId() const
{
	return 0;
}

uint64 CTextureNull::getBindlesId() const
{
	return uint64();
}

void CTextureNull::bind() const
{
}

void CShaderNull::AddRef()
{
}

int CShaderNull::Release()
{
	return 0;
}

int CShaderNull::Reload()
{
	return 0;
}

bool CShaderNull::Create(const char* label)
{
	return false;
}

void CShaderNull::Attach(ShaderInfo& shader)
{
}

IShaderProgram::ShaderInfo& CShaderNull::AttachInternal(ShaderInfo& src, ShaderInfo& dst)
{
	// TODO: insert return statement here
	static ShaderInfo si;
	return si;
}

void CShaderNull::Detach(ShaderInfo& shader)
{
}

bool CShaderNull::Dispatch(int x, int y, int z, uint barriers)
{
	return false;
}

bool CShaderNull::DispatchIndirect()
{
	return false;
}

bool CShaderNull::Link()
{
	return false;
}

void CShaderNull::Use()
{
}

void CShaderNull::Unuse()
{
}

void CShaderNull::DeleteProgram()
{
}

int CShaderNull::GetUniformLocation(const char* format, ...)
{
	return 0;
}

int CShaderNull::GetUniformLocation(string& name)
{
	return 0;
}

const char* CShaderNull::GetShaderName(IShader::Type type)
{
	return nullptr;
}

void CShaderNull::Uniform(const int value, const char* format, ...)
{
}

void CShaderNull::Uniform(const unsigned int value, const char* format, ...)
{
}

void CShaderNull::Uniform(const float value, const char* format, ...)
{
}

void CShaderNull::Uniform(const Vec1 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const Vec2 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const Vec3 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const Vec4 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const Mat2 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const Mat3 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const Mat4 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const glm::ivec4 value, const char* format, ...)
{
}

void CShaderNull::Uniform(const ITexture* texture, const char* format, ...)
{
}

void CShaderNull::Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label)
{
}

void CShaderNull::BindTexture2D(uint texture, int unit, const char* sampler)
{
}

void CShaderNull::BindTextureUnit2D(uint texture, int unit)
{
}

uint CShaderNull::Get()
{
	return uint();
}

void CShaderNull::setup()
{
}

void CShaderNull::Dump()
{
}
