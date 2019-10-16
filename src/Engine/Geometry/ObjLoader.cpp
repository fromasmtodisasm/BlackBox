#include <BlackBox/ObjLoader.hpp>
#include <BlackBox/IGeometry.hpp>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <string>

bool ObjLoader::load(const char* path, VerteciesInfo &verteciesInfo,	BoundingBox &bb)
{
  std::vector<face> faces;

	auto spath = std::string(path);
	if (spath.substr(spath.size() - 4, spath.size()) == ".bin")
	{
		return loadBin(path, verteciesInfo, bb);
	}

  FILE * file = fopen(path, "r");
  if (file == NULL) {
    printf("Impossible to open the file !\n");
    return false;
  }

	bool n_resized = false;
	bool t_resized = false;

	verteciesInfo.attributes[VA_POSITION] = true;
  while (1) {

		char lineHeader[128] = { 0 };
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader
    if (strcmp(lineHeader, "v") == 0) {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      vertex_buffer.push_back(vertex);
    }
    else if (strcmp(lineHeader, "vt") == 0) {
      has_uv = true;
			verteciesInfo.attributes[VA_UV] = true;
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      uv_buffer.push_back(uv);
    }
    else if (strcmp(lineHeader, "vn") == 0) {
			verteciesInfo.attributes[VA_NORMAL] = true;
      glm::vec3 normal;
			has_normal = true;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      normal_buffer.push_back(normal);
    }
    else if (strcmp(lineHeader, "f") == 0) {
      face current_face;
      char *pattern[4] = {
        "%d/%d/%d %d/%d/%d %d/%d/%d\n",
        "%d//%d %d//%d %d//%d\n",
        "%d %d %d\n",
        "%d/%d %d/%d %d/%d\n"
      };
      int matches;
			if (!has_uv && !has_normal)
			{
				
				verteciesInfo.attributes[VA_NORMAL] = true;
				verteciesInfo.attributes[VA_UV] = true;
				if (!n_resized)
				{
					normal_buffer.resize(vertex_buffer.size());
					n_resized = true;
				}
        matches = fscanf(file, pattern[2],
          &current_face.v[0].v,
          &current_face.v[1].v,
          &current_face.v[2].v
        );

				glm::vec3 v1 = vertex_buffer[current_face.v[1].v - 1] - vertex_buffer[current_face.v[0].v - 1];
				glm::vec3 v2 = vertex_buffer[current_face.v[2].v - 1] - vertex_buffer[current_face.v[0].v - 1];
				glm::vec3 normal = glm::cross(v1, v2);

				normal_buffer[current_face.v[0].v - 1] += normal;
				normal_buffer[current_face.v[1].v - 1] += normal;
				normal_buffer[current_face.v[2].v - 1] += normal;
				
			}
      else if (has_uv && !has_normal)
      {
				verteciesInfo.attributes[VA_NORMAL] = true;
				verteciesInfo.attributes[VA_UV] = true;
				if (!n_resized)
				{
					normal_buffer.resize(vertex_buffer.size());
					n_resized = true;
				}
        matches = fscanf(file, pattern[3],
          &current_face.v[0].v, &current_face.v[0].vt,
          &current_face.v[1].v, &current_face.v[1].vt,
          &current_face.v[2].v, &current_face.v[2].vt
        );

				glm::vec3 v1 = vertex_buffer[current_face.v[1].v - 1] - vertex_buffer[current_face.v[0].v - 1];
				glm::vec3 v2 = vertex_buffer[current_face.v[2].v - 1] - vertex_buffer[current_face.v[0].v - 1];
				glm::vec3 normal = glm::cross(v1, v2);

				normal_buffer[current_face.v[0].v - 1] += normal;
				normal_buffer[current_face.v[1].v - 1] += normal;
				normal_buffer[current_face.v[2].v - 1] += normal;
      }
      else if (has_uv)
      {
        matches = fscanf(file, pattern[0],
          &current_face.v[0].v, &current_face.v[0].vt, &current_face.v[0].n,
          &current_face.v[1].v, &current_face.v[1].vt, &current_face.v[1].n,
          &current_face.v[2].v, &current_face.v[2].vt, &current_face.v[2].n
        );
      }
      else
        matches = fscanf(file, pattern[1],
          &current_face.v[0].v, &current_face.v[0].n,
          &current_face.v[1].v, &current_face.v[1].n,
          &current_face.v[2].v, &current_face.v[2].n
        );
      faces.push_back(current_face);
    }
    else {
      // Probably a comment, eat up the rest of the line
      char stupidBuffer[1000];
      fgets(stupidBuffer, 1000, file);
    }
  }
	if (faces.size() == 0)
		return false;
  bb = buildVertexData(verteciesInfo.data, faces);

  return true;
}

bool ObjLoader::loadBin(const char* path, VerteciesInfo& verteciesInfo, BoundingBox& bb)
{
	return false;
}

BoundingBox ObjLoader::buildVertexData(VertexData& vertex_data, std::vector<face>& faces)
{
    // For each vertex of each triangle
	glm::vec3 min(
		vertex_buffer[faces[0].v[0].v - 1]
	);
	glm::vec3 max(
		vertex_buffer[faces[0].v[0].v - 1]
	);
	BoundingBox bb(min, max);
	for (unsigned int current_face = 0; current_face < faces.size(); current_face++) 
	{
		for (unsigned int current_vertex = 0; current_vertex < NUMBER_OF_VERTEX; current_vertex++)
		{
			std::vector<Vertex> face(3);
			face[0].pos = vertex_buffer[faces[current_face].v[0].v - 1];
			face[1].pos = vertex_buffer[faces[current_face].v[1].v - 1];
			face[2].pos = vertex_buffer[faces[current_face].v[2].v - 1];
			if (has_uv)
			{
				face[0].uv = uv_buffer[faces[current_face].v[0].vt - 1];
				face[1].uv = uv_buffer[faces[current_face].v[1].vt - 1];
				face[2].uv = uv_buffer[faces[current_face].v[2].vt - 1];
			}
			if (has_normal)
			{
				face[0].normal = normal_buffer[faces[current_face].v[0].n - 1];
				face[1].normal = normal_buffer[faces[current_face].v[1].n - 1];
				face[2].normal = normal_buffer[faces[current_face].v[2].n - 1];
			}
			else
			{
				face[0].normal = normal_buffer[faces[current_face].v[0].v - 1];
				face[1].normal = normal_buffer[faces[current_face].v[1].v - 1];
				face[2].normal = normal_buffer[faces[current_face].v[2].v - 1];
				//calcNormal(face);
			}
			bb.currentFace(face);
			
			calcTangentSpace(face);
			vertex_data.push_back(face[0]);
			vertex_data.push_back(face[1]);
			vertex_data.push_back(face[2]);
		}
	}
	for (int i = 0; i < vertex_data.size(); i++)
	{
		vertex_data[i].normal = glm::normalize(vertex_data[i].normal);
	}
  return bb;
}

void ObjLoader::calcNormal(std::vector<Vertex>& face)
{
  glm::vec3 v1 = face[1].pos - face[0].pos;
  glm::vec3 v2 = face[2].pos - face[0].pos;
  glm::vec3 normal = glm::cross(v1,v2);

  face[0].normal += normal;
  face[1].normal += normal;
  face[2].normal += normal;
}

void ObjLoader::calcTangentSpace(std::vector<Vertex>& face)
{
  Vertex& v0 = face[0];
  Vertex& v1 = face[1];
  Vertex& v2 = face[2];

  glm::vec3 Edge1 = v1.pos - v0.pos;
  glm::vec3 Edge2 = v2.pos - v0.pos;

  float DeltaU1 = v1.uv.x - v0.uv.x;
  float DeltaV1 = v1.uv.y - v0.uv.y;
  float DeltaU2 = v2.uv.x - v0.uv.x;
  float DeltaV2 = v2.uv.y - v0.uv.y;

  float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

  glm::vec3 Tangent, Bitangent;

  Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
  Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
  Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

  Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
  Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
  Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

  v0.tangent = glm::normalize(Tangent);
  v1.tangent = glm::normalize(Tangent);
  v2.tangent = glm::normalize(Tangent);

  v0.btangent = glm::normalize(Bitangent);
  v1.btangent = glm::normalize(Bitangent);
  v2.btangent = glm::normalize(Bitangent);
}
