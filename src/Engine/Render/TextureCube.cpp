#include <BlackBox/Render/TextureCube.hpp>

#include <vector>

TextureCube::TextureCube() : skybox_root(texture_root + "skys/")
{
}

TextureCube::TextureCube(std::string name)
{
}

void TextureCube::setType(const char* TextureType)
{
}

std::string TextureCube::typeToStr()
{
	return std::string();
}

bool TextureCube::load(const char* name)
{
	GLenum inputFormat = GL_RGB;
	GLenum internalFormat = GL_RGB;
	GLenum inputDataType = GL_UNSIGNED_BYTE;

	std::vector<std::string> faces = 
	{
		"_back.jpg",
		"_bottom.jpg",
		"_front.jpg",
		"_left.jpg",
		"_right.jpg",
		"_top.jpg"
	};

	Image img;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		if (!img.load((skybox_root + name + faces[i]).c_str(), nullptr))
			return false;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, internalFormat, img.width, img.height, 0, inputFormat, GL_UNSIGNED_BYTE, img.data
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}
