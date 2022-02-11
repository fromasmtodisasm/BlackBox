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
