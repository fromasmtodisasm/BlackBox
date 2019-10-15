#pragma once

#include <BlackBox/Render/BaseTexture.hpp>
#include <BlackBox/Object.hpp>

#include <vector>
#include <stb_image.h>


class Terrain
{
public:
	struct Face
	{
		Vertex v[3];
	};
	Terrain()
	{

	}
	Face getFace(int v1, int v2, int v3)
	{
		Face face;
		face.v[0].pos = glm::vec3(vb[v1 - 1].pos);
		face.v[1].pos = glm::vec3(vb[v2 - 1].pos);
		face.v[2].pos = glm::vec3(vb[v3 - 1].pos);

		glm::vec3 vec1 = face.v[1].pos - face.v[0].pos;
		glm::vec3 vec2 = face.v[2].pos - face.v[0].pos;
		glm::vec3 normal = glm::normalize(glm::cross(vec1,vec2));

		face.v[0].normal = glm::normalize(normal);
		face.v[1].normal = glm::normalize(normal);
		face.v[2].normal = glm::normalize(normal);

		return face;
	}
	Object *load(const char *heightmap)
	{
		int width = 0, height = 0, channels;
		uint8_t* data = nullptr;
		bool res = false;

		uint8_t* ptr = stbi_load(heightmap, &width, &height, &channels, STBI_rgb);
		if (ptr)
		{
			// Assign the image properties
			if (width && height)
			{
				// Copy the loaded pixels to the pixel buffer
				data = new unsigned char[width * height * channels];
				memcpy(data, ptr, width * height * 3);
				res = true;
			}
			// Free the loaded pixels (they are now in our own pixel buffer)
			stbi_image_free(ptr);
		}

		std::vector<face> f;
		int k = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vertex v;
				v.pos.x = 4 * (float)j;// / (width - 1);
				v.pos.y = 160.f * data[k] / 255.0f;
				v.pos.z = 4 *(float)i;// / (height - 1);
				vb.push_back(v);
				k += 3;
			}
		}
		//fprintf(fout, "# Auto generated via HMtoObjConverter\n\n");
		//fprintf(fout, "# Vertex cnt %d\n", vb.size());

		/*
		for (int i = 0; i < vb.size(); i++)
		{
			fprintf(fout, "v %f %f %f\n", vb[i].x, vb[i].y, vb[i].z);
		}
		*/

		std::vector<Vertex> vertecies;
		for (int i = 0; i < height - 1; i++)
		{
			for (int j = 0; j < width - 1; j++)
			{
				// First triangle in quad
				//fprintf(fout, "f %d %d %d\n", (i * width + j) + 1, (i * width + j + 1) + 1, ((i + 1) * width + j) + 1);
				auto f = getFace((i * width + j) + 1, (i * width + j + 1) + 1, ((i + 1) * width + j) + 1);
				vertecies.push_back(f.v[0]);
				vertecies.push_back(f.v[1]);
				vertecies.push_back(f.v[2]);
				// Second triangle in quad
				//fprintf(fout, "f %d %d %d\n", (i * width + j + 1) + 1, ((i + 1) * width + j + 1) + 1, ((i + 1) * width + j) + 1);
				f = getFace((i * width + j) + 1, (i * width + j + 1) + 1, ((i + 1) * width + j) + 1);
				vertecies.push_back(f.v[2]);
				vertecies.push_back(f.v[1]);
				vertecies.push_back(f.v[0]);
			}
		}

		//fclose(fout);

		VertexArrayObject *vb = new VertexArrayObject(vertecies.data(), static_cast<GLint>(vertecies.size()), GL_TRIANGLES, VertexArrayObject::Attributes());
		 //std::vector<Mesh> mesh;
		auto mesh = std::make_shared<std::vector<Mesh>>();
		Mesh _mesh(vb, nullptr);
		//_mesh.bb = bb;
		mesh->push_back(_mesh);
		auto obj = Object::getEmpty();
		obj->m_Mesh = mesh;
		obj->m_path = "";
		//obj->m_RenderMode = GL_LINE;

		/*
		size = vertecies.size();
		glCheck(glGenVertexArrays(1, &VAO));
		glCheck(glBindVertexArray(VAO));
			glCheck(glGenBuffers(1, &VBO));
			glCheck(glBindBuffer(GL_VERTEX_ARRAY, VBO));
			glCheck(glBufferData(GL_VERTEX_ARRAY, vertecies.size() * sizeof(Vertex), vertecies.data(), GL_STATIC_DRAW));

			glCheck(glEnableVertexAttribArray(0));
			glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)NULL));

			glCheck(glEnableVertexAttribArray(1));
			glCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offsetof(Vertex, normal))));

		glCheck(glBindVertexArray(0));
		*/
		
		img.free();
		return obj;
	}

	void draw()
	{
		glCheck(glBindVertexArray(VAO));
		glCheck(glDrawArrays(GL_TRIANGLES, 0, size));
		glCheck(glBindVertexArray(0));
	}

private:
	float getHeight(int x, int y)
	{
		return 40 * ((unsigned char*)(img.data))[y * img.width + x] / 255.0f;
	}

	std::vector<Vertex> vb;
	Image img;
	GLuint VBO, VAO;
	int size;


};
