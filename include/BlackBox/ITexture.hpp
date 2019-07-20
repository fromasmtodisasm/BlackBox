#pragma once

struct Image
{
	int width;
	int height;
	int bpp;
	void* data;

	Image() : width(0), height(0), bpp(0), data(nullptr) {}
	~Image();
	bool load(const char* name, bool *hasAlpha);
	void free();
};

struct ITexture
{
	virtual bool load(const char* name) = 0;
  virtual void setType(const char* TextureType) = 0;
  virtual std::string typeToStr() = 0;
};