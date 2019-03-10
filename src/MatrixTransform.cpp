#pragma once

#include "MatrixTranform.hpp"

std::vector<float> operator * (std::vector<float> vec, glm::mat4 m) {
  glm::mat4 rot = m;

  // glm::mat4 rot = glm::rotate(glm::mat4())

  for (int i = 0; i <= vec.size() - 3; i += 3)
  {
    glm::vec4 v = glm::vec4(vec[i], vec[i + 1], vec[i + 2], 1.0); // 0.0 - -> ; 1.0 - coord
    v = v * rot;
    vec[i] = v.x;
    vec[i + 1] = v.y;
    vec[i + 2] = v.z;
  }
  return vec;
}
