#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Texture.hpp>
#include <ctime>
#include <iostream>
#include <memory>

using namespace std;

Texture::Texture()
{
	id = 0;
}

Texture::Texture(GLuint id)
{
	BaseTexture::id = id;
	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width));
	glCheck(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height));
}

Texture::Texture(std::string name)
{
	this->name = name;
}

void Texture::setType(const char* type)
{
	std::string t(type);

	if (t == "diffuse")
		this->type = DIFFUSE;
	else if (t == "specular")
		this->type = SPECULAR;
	else if (t == "bump")
		this->type = BUMP;
	else if (t == "normal")
		this->type = NORMAL;
	else if (t == "mask")
		this->type = MASK;
	else if (t == "emissive")
		this->type = EMISSIVE;
	else
		this->type = UNKNOWN;
}

const char* Texture::typeToStr()
{
	std::string result;
	switch (type)
	{
	case TextureType::DIFFUSE:
		return "diffuse";
	case TextureType::SPECULAR:
		return "specular";
	case TextureType::BUMP:
		return "bump";
	case TextureType::NORMAL:
		return "normal";
	case TextureType::MASK:
		return "mask";
	default:
	{
		cout << "Error: unknown texture type" << endl;
		return "";
	}
	}
}

bool Texture::load(const char* name)
{
	Image img;
	bool hasAlpha = false;
	if (!img.load((texture_root + name).c_str(), &hasAlpha))
		return false;
	auto t = create(img.width, img.height, UNKNOWN, hasAlpha, name, true, img.data);
	if (t != nullptr)
	{
		std::swap(*this, *t);
	}
	return t == nullptr;
}

Texture* Texture::create(int width, int height, TextureType type, bool hasAlpha, const std::string& name, bool createMipChain, void* data, bool is_msaa)
{
	Texture* t	= new Texture;
	bool status = true;
	GLint internalFormat, inputFormat;
	GLint filterMin, filterMag;
	GLint wrapS, wrapT;
	GLint inputDataType;
	GLenum textureTarget = !is_msaa ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
	t->isMS				 = is_msaa;

	gl::CreateTextures2D(textureTarget, 1, &t->id);
	switch (type)
	{
	case TextureType::DEPTH:
		internalFormat = inputFormat = GL_DEPTH_COMPONENT;
		filterMin = filterMag = GL_LINEAR;
		wrapS = wrapT = GL_CLAMP_TO_BORDER;
		inputDataType = GL_FLOAT;
		//attachment =
		break;
	case TextureType::LDR_RENDER_TARGET:
		internalFormat = GL_RGB8;
		inputFormat	   = GL_RGB;
		filterMin = filterMag = GL_LINEAR;
		wrapS = wrapT = GL_CLAMP_TO_EDGE;
		inputDataType = GL_UNSIGNED_BYTE;
		break;
	case TextureType::HDR_RENDER_TARGET:
		internalFormat = GL_RGBA16F;
		inputFormat	   = GL_RGBA;
		filterMin = filterMag = GL_LINEAR;
		wrapS = wrapT = GL_CLAMP_TO_EDGE;
		inputDataType = GL_FLOAT;
		break;
	default:
		internalFormat = GL_RGB8;
		inputFormat	   = GL_RGB;
		filterMin = filterMag = GL_LINEAR;
		wrapS = wrapT = GL_CLAMP_TO_EDGE;
		inputDataType = GL_UNSIGNED_BYTE;
		gl::TextureParameteri(t->id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		gl::TextureParameteri(t->id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (hasAlpha)
		{
			inputFormat	   = GL_RGBA;
			internalFormat = GL_SRGB8_ALPHA8;
		}
	}

	if (is_msaa)
	{
		auto num_samples = gEnv->pConsole->GetCVar("r_MSAA_samples")->GetIVal();
		glTextureStorage2DMultisample(t->id, num_samples, internalFormat, width, height, true);
		gl::TexImage2DMS(textureTarget, num_samples, internalFormat, width, height, GL_TRUE);
	}
	else
	{
		//gl::TextureParameteri(t->id, GL_TEXTURE_MIN_FILTER, filterMin);
		//gl::TextureParameteri(t->id, GL_TEXTURE_MAG_FILTER, filterMag);

		if (type == DEPTH)
		{
			float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			glTexParameterfv(t->id, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTextureStorage2D(t->id, 1, internalFormat, width, height);
		if (UNKNOWN == type)
		{
			std::vector<char> pixels(width * height * 4);
			if (data == nullptr)
				data = pixels.data();
			gl::TextureImage2D(
				t->id, 0, 0, 0,
				width, height,
				inputFormat, inputDataType, data);
		}
	}

	if (createMipChain)
	{
		glCheck(glGenerateMipmap(textureTarget));
	}

	debuger::texture_label(t->id, name);
	return t;
}

void Texture::bind()
{
	gl::ActiveTexture(GL_TEXTURE0 + unit);
	gl::BindTexture2D(GL_TEXTURE_2D, id);
}

void Texture::setUnit(GLuint unit)
{
	this->unit = unit;
}
