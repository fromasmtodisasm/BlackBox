#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/Scene/XmlSerialization.hpp>

using namespace tinyxml2;

namespace serialization {
  Vec3 loadColorAttribute(tinyxml2::XMLElement* element)
  {
    glm::vec3 color;
    if (element == nullptr)
      //TODO: handle this
      return glm::vec3();
    color.r = element->FloatAttribute("r");
    color.g = element->FloatAttribute("g");
    color.b = element->FloatAttribute("b");
    return color;
  }


  tinyxml2::XMLElement* writeAttribute(tinyxml2::XMLDocument& xmlDoc, glm::vec3& color, const char* name)
  {
    XMLElement* result = xmlDoc.NewElement(name);
    result->SetAttribute("x", color.r);
    result->SetAttribute("y", color.g);
    result->SetAttribute("z", color.b);
    return result;
  }

  tinyxml2::XMLElement* writeElement(tinyxml2::XMLDocument& xmlDoc, glm::vec3& vec, const char* name)
  {
    XMLElement* result = xmlDoc.NewElement(name);
    XMLElement* X = xmlDoc.NewElement("X");
    XMLElement* Y = xmlDoc.NewElement("Y");
    XMLElement* Z = xmlDoc.NewElement("Z");

    X->SetText(vec.x);
    Y->SetText(vec.y);
    Z->SetText(vec.z);
    result->InsertEndChild(X);
    result->InsertEndChild(Y);
    result->InsertEndChild(Z);
    return result;
  }

}
