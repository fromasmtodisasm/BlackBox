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
	virtual void bind() = 0;
	virtual void setUnit(GLuint unit) = 0;
  virtual void setType(const char* TextureType) = 0;
  virtual std::string typeToStr() = 0;
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
  virtual int getId() = 0;
};