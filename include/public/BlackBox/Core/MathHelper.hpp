#pragma once
#if USE_GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using Vec1 = glm::vec1;
using Vec2 = glm::vec2;
using IVec2 = glm::ivec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Mat2 = glm::mat2;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

using Vec3d = glm::ivec3;
using Vec4d = glm::ivec4;
using Matrix44 = glm::mat4;
#else

#endif // USE_GLM