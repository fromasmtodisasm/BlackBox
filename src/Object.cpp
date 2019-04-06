#include "Object.hpp"
#include "VertexBuffer.hpp"
#include <fstream>
#include <iostream>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Object::Object() : angle(0.0f)
{

}

void Object::parse(std::string filename, std::vector<Vertex> &vs, CShaderProgram **shader)
{
  std::ifstream in;
  std::string line;
  std::string vsh;
  std::string fsh;
  std::vector<glm::vec3> *faces_;

  faces_ = new vector<glm::vec3>;

  in.open("res\\" + filename, std::ifstream::in);
  if (in.fail()) return;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());
    char trash;
    if (!line.compare(0, 2, "v ")) {
      iss >> trash;
      Vertex v;
      for (int i = 0; i < 3; i++) iss >> v[i];
      vs.push_back(v);
    }
    if (!line.compare(0, 5, "vsh: ")) {
      size_t pos = line.find(" ");
      if (pos != std::string::npos)
        vsh = string(line.substr(pos + 1));
    }
    if (!line.compare(0, 5, "fsh: ")) {
      size_t pos = line.find(" ");
      if (pos != std::string::npos)
        fsh = string(line.substr(pos + 1));
    }
    /*
    else if (!line.compare(0, 2, "f ")) {
      std::vector<Vec3i> f;
      Vec3i tmp;
      iss >> trash;
      while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
        for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
        f.push_back(tmp);
      }
      faces_.push_back(f);
    }
    */
  }
  *shader = new CShaderProgram("res\\" + vsh, "res\\" + fsh);
}

void Object::draw() {
  VertexBuffer *vb = m_Mesh->getVertexBuffer();
  glm::mat4x4 model(1.0f);
  m_Shader->use();
  model = glm::rotate(model, angle.x, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
  model = glm::rotate(model, angle.y, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  model = glm::rotate(model, angle.z, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
  m_Shader->setUniformValue("Model", model);
  vb->draw();
}

void Object::setType(OBJType type)
{
  m_type = type;
}

CShaderProgram * Object::getShaderProgram()
{
  return m_Shader;
}

void Object::move(glm::vec3 v) {}

void Object::rotate(float angle, glm::vec3 v) {
  if (v.x) this->angle.x += angle;
  if (v.y) this->angle.y += angle;
  if (v.z) this->angle.z += angle;
}

void Object::scale(glm::vec3 v) {}


Object * Object::load(string path)
{
  Object *obj = nullptr;
  Mesh *mesh;
  VertexBuffer *vb;
  CShaderProgram *shader;
  std::vector<Vertex> v;
  parse(path, v, &shader);
  float *data = reinterpret_cast<float*>(reinterpret_cast<Vertex*>(v.data()));
  
  vb = new VertexBuffer(v.data(), static_cast<GLint>(v.size()));
  mesh = new Mesh(vb, nullptr);
  obj = new Object();
  obj->m_Mesh = mesh;
  obj->m_Shader = shader;
  obj->m_Shader->create();
	return obj;
}
