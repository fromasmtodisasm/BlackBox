#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/Object.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/ShaderManager.hpp>

namespace Pipeline {

#if 0
IShaderProgram* bindProgram(const char* name)
{
    auto mat = MaterialManager::instance()->getMaterial(name);
    decltype(mat->program) shader;
    if (mat != nullptr)
    {
        if (mat->program != nullptr)
            shader = mat->program;
        else
            shader = ShaderManager::instance()->getDefaultProgram();
    }
    shader->Use();
    return shader;
}
#endif

}

void BoundingBox::draw(Mat4 transform)
{

}
