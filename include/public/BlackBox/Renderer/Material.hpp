#pragma once
#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Core/MathHelper.hpp>

#include <vector>
#include <memory>

class CShader;
class CBaseShaderProgram;
class Object;

struct Material
{
  BaseTexture
    /**diffuse,*/
    * specular,
    * emissive,
    * bump,
    * normal,
    * mask;
  float shininess;
  float emissive_factor = 1.0f;
  glm::vec3 diffuseColor;
  float alpha;

  std::vector<BaseTexture*> diffuse;

  //BaseShaderProgramRef program;
  IShaderProgram *program;
  std::string program_name;
  std::shared_ptr<std::string> name;
  bool hasTexture = false;
  bool enabledNormal = true;
  int current_diffuse = 0;

  Material() :
    //diffuse(nullptr),
    specular(nullptr),
    bump(nullptr),
    normal(nullptr),
    mask(nullptr),
    emissive(nullptr),
    emissive_factor(1.0f),
    diffuseColor({ 0,0,0 }),
    shininess(64.0f),
    alpha(1.0),
    program(nullptr)
  {
  }
  void apply(Object* object);
  void setTexture(Texture* texture, const char* type);
  void nextDiffuse();
  void prevDiffuse();
private:
  void activeTexture(const char* uniform, BaseTexture* texture);
};

template<typename T>
struct uv_type { const static UniformValue::Type type; };

template<> struct uv_type<float> { const static UniformValue::Type type = UniformValue::Type::FLOAT_VAL; };
template<> struct uv_type<int> { const static UniformValue::Type type = UniformValue::Type::INT_VAL; };
template<> struct uv_type<glm::vec1> { const static UniformValue::Type type = UniformValue::Type::V1_VAL; };
template<> struct uv_type<glm::vec2> { const static UniformValue::Type type = UniformValue::Type::V2_VAL; };
template<> struct uv_type<glm::vec3> { const static UniformValue::Type type = UniformValue::Type::V3_VAL; };
template<> struct uv_type<glm::vec4> { const static UniformValue::Type type = UniformValue::Type::V4_VAL; };
template<> struct uv_type<glm::mat2> { const static UniformValue::Type type = UniformValue::Type::M2_VAL; };
template<> struct uv_type<glm::mat3> { const static UniformValue::Type type = UniformValue::Type::M3_VAL; };
template<> struct uv_type<glm::mat4> { const static UniformValue::Type type = UniformValue::Type::M4_VAL; };
template<> struct uv_type<ITexture*> { const static UniformValue::Type type = UniformValue::Type::Samp_VAL; };
template<> struct uv_type<CCamera*> { const static UniformValue::Type type = UniformValue::Type::CAMERA_VAL; };


template<class T>
auto add_uniform(SRenderParams &renderParams, std::string_view name, T value)
{
	UniformValue uv(name, uv_type<T>::type);
	uv.Set(value);
	renderParams.uniforms.push_back(uv);
}