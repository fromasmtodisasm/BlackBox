#if 0
#include <BlackBox/3DEngine/StatObject.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/Terrain.hpp>
#include <BlackBox/Renderer/VertexBuffer.hpp>

#include <stb_image.h>

Terrain::Terrain()
	: img(std::make_unique<Image>())
{
}

Terrain::Face Terrain::getFace(size_t v1, size_t v2, size_t v3)
{
	Face face;
	face.v[0].pos = glm::vec3(vb[v1 - 1].pos);
	face.v[1].pos = glm::vec3(vb[v2 - 1].pos);
	face.v[2].pos = glm::vec3(vb[v3 - 1].pos);

	glm::vec3 vec1	 = face.v[1].pos - face.v[0].pos;
	glm::vec3 vec2	 = face.v[2].pos - face.v[0].pos;
	glm::vec3 normal = glm::normalize(glm::cross(vec1, vec2));

	face.v[0].normal = glm::normalize(normal);
	face.v[1].normal = glm::normalize(normal);
	face.v[2].normal = glm::normalize(normal);

	return face;
}

CStatObj* Terrain::load(const char* heightmap)
{
	int width = 0, height = 0, channels;
	uint8_t* data = nullptr;
	bool res	  = false;

	uint8_t* ptr = stbi_load(heightmap, &width, &height, &channels, STBI_rgb);
	if (ptr)
	{
		// Assign the image properties
		if (width && height)
		{
			// Copy the loaded pixels to the pixel buffer
			data = new unsigned char[width * height * channels];
			memcpy(data, ptr, width * height * channels);
			res = true;
			// Free the loaded pixels (they are now in our own pixel buffer)
			stbi_image_free(ptr);
		}
		else
		{
			return nullptr;
		}
	}

	std::vector<face> f;
	int k = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vertex v;
			int height = k;
			v.pos.x	   = (float)j; // / (width - 1);
			v.pos.y	   = data[height];
			v.pos.z	   = (float)i; // / (height - 1);
			vb.push_back(v);
			k += channels;
		}
	}
	std::vector<Vertex> vertecies;
	int index1;
	int index2;
	int index3;
	int index4;
	for (int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			index1 = (width * i) + j;			   // Bottom left.
			index2 = (width * i) + (j + 1);	   // Bottom right.
			index3 = (width * (i + 1)) + j;	   // Upper left.
			index4 = (width * (i + 1)) + (j + 1); // Upper right.
			Vertex v[4] = {
				vb[index1],
				vb[index2],
				vb[index3],
				vb[index4]
			};
			vertecies.push_back(v[3 - 1]);
			vertecies.push_back(v[4 - 1]);
			vertecies.push_back(v[4 - 1]);
			vertecies.push_back(v[1 - 1]);
			vertecies.push_back(v[1 - 1]);
			vertecies.push_back(v[3 - 1]);

			vertecies.push_back(v[1 - 1]);
			vertecies.push_back(v[4 - 1]);
			vertecies.push_back(v[4 - 1]);
			vertecies.push_back(v[2 - 1]);
			vertecies.push_back(v[2 - 1]);
			vertecies.push_back(v[1 - 1]);
		}
	}

	//fclose(fout);

	//VertexArrayObject* vb = new VertexArrayObject(vertecies.data(), static_cast<GLint>(vertecies.size()), GL_LINES, VertexArrayObject::Attributes());
	//VertexArrayObject* vb = new VertexArrayObject(vertecies.data(), static_cast<GLint>(vertecies.size()), RenderPrimitive::LINES, VertexArrayObject::Attributes());
	//std::vector<Mesh> mesh;
	//auto mesh = std::make_shared<std::vector<Mesh>>();
#if 0
	Mesh _mesh(vb, nullptr);
	//_mesh.bb = bb;
	mesh->push_back(_mesh);
	auto obj	= Object::getEmpty();
	obj->m_Mesh = mesh;
	obj->m_path = "terrain";
	img->free();
	return obj;
#endif
	return nullptr;
}

void Terrain::draw()
{
	glCheck(glBindVertexArray(VAO));
	glCheck(glDrawArrays(GL_TRIANGLES, 0, size));
	glCheck(glBindVertexArray(0));
}

float Terrain::getHeight(int x, int y)
{
	return 40 * ((unsigned char*)(img->data))[y * img->width + x] / 255.0f;
}
#endif