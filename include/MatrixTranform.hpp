#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

std::vector<float> operator * (std::vector<float> vec, glm::mat4 m);


// vector = vector * m && vector = vector / m

// vec = changeMatrix(vec,mat,vec2,'*')