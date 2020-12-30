#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Texture.hpp>
#include <ctime>
#include <iostream>
#include <memory>

using namespace std;

// TODO: learn this link https://pdfs.semanticscholar.org/8482/f1211278f9e581d00d2865974dd8771c9469.pdf

uint UploadDDS(const Image& img)
{
	uint tid;
	int w = 0, h = 0;
	uint mipMapCount = img.mipMapCount;
  // prepare new incomplete texture
	glGenTextures(1, &tid);
	
  // bind the texture
  // make it complete by specifying all needed parameters and ensuring all mipmaps are filled
	glBindTexture(GL_TEXTURE_2D, tid);
	#define USE_OLD_TEXTURE_PARAMETERS
	#ifdef USE_OLD_TEXTURE_PARAMETERS
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount-1); // opengl likes array length of mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	#endif
	  
    // prepare some variables
		uint offset = 0;
		uint size = 0;
		w = img.width;
		h = img.height;
    
    // loop through sending block at a time with the magic formula
    // upload to opengl properly, note the offset transverses the pointer
    // assumes each mipmap is 1/2 the size of the previous mipmap
		for (uint i=0; i<mipMapCount; i++) {
			if(w == 0 || h == 0) { // discard any odd mipmaps 0x1 0x2 resolutions
				mipMapCount--;
				continue;
			}
			size = ((w+3)/4) * ((h+3)/4) * img.blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, i, img.format, w, h, 0, size, &img.data[0] + offset);
			offset += size;
			w /= 2;
			h /= 2;
		}
	    // discard any odd mipmaps, ensure a complete texture
	#ifdef USE_OLD_TEXTURE_PARAMETERS
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount-1);
	#endif
    // unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return tid;
}

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

Image loadDDS(const char* path)
{
// lay out variables to be used
	Image img;
	std::vector<unsigned char> header;
	
	int width;
	int height;
	uint mipMapCount;
	
	uint blockSize;
	int format;
	
	std::vector<uint8_t> buffer;
	
	GLuint tid = 0;
	
  // open the DDS file for binary reading and get file size
	FILE* f;
	if((f = fopen(path, "rb")) == 0)
		return img;
	fseek(f, 0, SEEK_END);
	long file_size = ftell(f);
	fseek(f, 0, SEEK_SET);
	
  // allocate new unsigned char space with 4 (file code) + 124 (header size) bytes
  // read in 128 bytes from the file
	header.resize(128);
	fread(header.data(), 1, 128, f);
  
  // compare the `DDS ` signature
	if(memcmp(header.data(), "DDS ", 4) != 0)
		goto exit;
	
  // extract height, width, and amount of mipmaps - yes it is stored height then width
	height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
	width = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
	mipMapCount = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);
	
  // figure out what format to use for what fourCC file type it is
  // block size is about physical chunk storage of compressed data in file (important)
	if(header[84] == 'D') {
		switch(header[87]) {
			case '1': // DXT1
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				blockSize = 8;
				break;
			case '3': // DXT3
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				blockSize = 16;
				break;
			case '5': // DXT5
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				blockSize = 16;
				break;
			case '0': // DX10
				// unsupported, else will error
				// as it adds sizeof(struct DDS_HEADER_DXT10) between pixels
				// so, buffer = malloc((file_size - 128) - sizeof(struct DDS_HEADER_DXT10));
			default: goto exit;
		}
	} else // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
		goto exit;
	
  // allocate new unsigned char space with file_size - (file_code + header_size) magnitude
  // read rest of file
	buffer.resize(file_size - 128);
	fread(buffer.data(), 1, file_size, f);

	img = Image(width, height, 0, std::move(buffer), true, format, blockSize, mipMapCount);
exit:
	fclose(f);
	return img;
	
}

bool Texture::load(const char* name)
{
	return load(std::string_view(name));
}
bool Texture::load(std::string_view name)
{
	Image img;
	bool hasAlpha = false;
	if (name.substr(name.size() - 4) == ".dds")
	{
		img = loadDDS(name.data());
	}
	else
	{
		if (!img.load((std::string(name)).c_str(), &hasAlpha))
			return false;
	}
	auto t = create(img, UNKNOWN, std::string(name), true);
	if (t != nullptr)
	{
		std::swap(*this, *t);
	}
	return t == nullptr;
}

Texture* Texture::create(const Image& img, TextureType type, const std::string& name, bool createMipChain, bool is_msaa)
{
	Texture* t	= new Texture;
	bool status = true;
	GLint internalFormat, inputFormat;
	GLint filterMin, filterMag;
	GLint wrapS, wrapT;
	GLint inputDataType;
	GLenum textureTarget = !is_msaa ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
	t->isMS				 = is_msaa;

	t->width = img.width;
	t->height = img.height;
	if (!img.compressed)
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
		if (!img.compressed)
		{
			internalFormat = GL_RGB8;
			inputFormat	   = GL_RGB;
			filterMin = filterMag = GL_LINEAR;
			wrapS = wrapT = GL_CLAMP_TO_EDGE;
			inputDataType = GL_UNSIGNED_BYTE;
			gl::TextureParameteri(t->id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			gl::TextureParameteri(t->id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (img.bpp == 4)
			{
				inputFormat	   = GL_RGBA;
				internalFormat = GL_SRGB8_ALPHA8;
			}
		}
	}

	if (is_msaa)
	{
		auto num_samples = gEnv->pConsole->GetCVar("r_MSAA_samples")->GetIVal();
		glTextureStorage2DMultisample(t->id, num_samples, internalFormat, img.width, img.height, true);
		gl::TexImage2DMS(textureTarget, num_samples, internalFormat, img.width, img.height, GL_TRUE);
	}
	else
	{
		//gl::TextureParameteri(t->id, GL_TEXTURE_MIN_FILTER, filterMin);
		//gl::TextureParameteri(t->id, GL_TEXTURE_MAG_FILTER, filterMag);
		if (img.compressed)
		{
			t->id = UploadDDS(img);	
		}
		else
		{
			if (type == DEPTH)
			{
				float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
				glTexParameterfv(t->id, GL_TEXTURE_BORDER_COLOR, borderColor);
			}

			glTextureStorage2D(t->id, 1, internalFormat, img.width, img.height);
			if (UNKNOWN == type)
			{
				std::vector<uint8_t> pixels(img.width * img.height * 4);
				gl::TextureImage2D(
					t->id, 0, 0, 0,
					img.width, img.height,
					inputFormat, inputDataType, 
					img.data.size() == 0 ? pixels.data() : &img.data[0]
				);
			}
		}

	}

	if (createMipChain && !img.compressed)
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
