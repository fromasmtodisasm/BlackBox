#pragma once
#include <BlackBox\Core\MathHelper.hpp>

class AuxRenderer
{
public:
    AuxRenderer(IRenderer* pRenderer);
    static void DrawAABB(Vec3 min, Vec3 max);

private:
    IRenderer* m_Renderer;
};