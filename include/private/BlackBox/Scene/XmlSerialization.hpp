#include <BlackBox/Core/MathHelper.hpp>

#include <tinyxml2.h>

namespace serialization {
  Vec3 loadColorAttribute(tinyxml2::XMLElement* element);
  
  template<typename T>
  T loadElement(tinyxml2::XMLElement& element, const char* name);

  inline Vec3 loadElement(tinyxml2::XMLElement& element, const char* name)
  {
    glm::vec3 result;

    tinyxml2::XMLElement* vector = element.FirstChildElement(name);
    tinyxml2::XMLElement* X = vector->FirstChildElement("X");
    tinyxml2::XMLElement* Y = vector->FirstChildElement("Y");
    tinyxml2::XMLElement* Z = vector->FirstChildElement("Z");
    result.x = X->FloatText();
    result.y = Y->FloatText();
    result.z = Z->FloatText();

    return result;
  }

  tinyxml2::XMLElement* writeAttribute(tinyxml2::XMLDocument& xmlDoc, glm::vec3& color, const char* name);

  tinyxml2::XMLElement* writeElement(tinyxml2::XMLDocument& xmlDoc, glm::vec3& vec, const char* name);

  template<typename T>
  inline tinyxml2::XMLElement* writeElement(tinyxml2::XMLDocument& xmlDoc, T value, const char* name)
  {
    tinyxml2::XMLElement* result = xmlDoc.NewElement(name);
    result->SetText(value);
    return result;
  }

}
