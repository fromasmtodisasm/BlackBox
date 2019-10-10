#pragma once
#include <BlackBox/common.h>
#include <BlackBox/Render/VertexArrayObject.hpp>
#include <BlackBox/Render/Pipeline.hpp>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <memory>
#include <string>
#include <vector>

class VertexArrayObject;
class IndexBuffer;
struct Vertex;

using VertexData = std::vector<Vertex>;
using IndexData = std::vector<int>;

enum VertexAtribute
{
	VA_POSITION,
	VA_NORMAL,
	VA_UV,
	VA_TANGENT,
	VA_BTANGENT,
	VA_NUM
};

struct indexed_vertex
{
  unsigned int v;
  unsigned int vt;
  unsigned int n;
};

struct face
{
  indexed_vertex v[3];
};

struct Vertex
{
  glm::vec3 pos;
  glm::vec2 uv;
  glm::vec3 normal;
  glm::vec3 tangent;
  glm::vec3 btangent;

	Vertex() : pos(0), uv(0), normal(0), tangent(0), btangent(0) {}
	Vertex(glm::vec3 pos, glm::vec2 uv) : pos(pos), uv(uv), normal(0), tangent(0), btangent(0)
	{}
  Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 n) : pos(pos), uv(uv), normal(n), tangent(0), btangent(0)
  {}
};

struct VerteciesInfo
{
	VertexData data;
	bool attributes[VA_NUM];
	bool indexed;
	IndexData indecies;
	VerteciesInfo() : attributes(), indexed(false)
	{}
};

struct Mesh;

struct BoundingBox
{
	glm::vec3 min;
	glm::vec3 max;
	Mesh* mesh;
	BoundingBox(glm::vec3 min, glm::vec3 max)
		:
		min(min), max(max), mesh(nullptr)
	{

	}
	BoundingBox() : min(0.0f), max(1.0f), mesh(nullptr)
	{

	}

	void currentFace(std::vector<Vertex> &current_face)
	{
		cmpVertex(current_face[0].pos);
		cmpVertex(current_face[1].pos);
		cmpVertex(current_face[2].pos);
	}

	void cmpVertex(glm::vec3 &v)
	{
		if (v.x < min.x) min.x = v.x;
		if (v.x > max.x) max.x = v.x;
		if (v.y < min.y) min.y = v.y;
		if (v.y > max.y) max.y = v.y;
		if (v.z < min.z) min.z = v.z;
		if (v.z > max.z) max.z = v.z;
	}

	void draw();
};


struct Mesh {
public:
  VertexArrayObject *m_Verts;
  IndexBuffer *m_Indexes;
	BoundingBox bb;
public:
  std::shared_ptr<std::string> m_Path;
  Mesh(VertexArrayObject *verts, IndexBuffer *indxs);
	Mesh() = default;

  VertexArrayObject *getVertexBuffer();
  IndexBuffer *getIndexBuffer();
};

