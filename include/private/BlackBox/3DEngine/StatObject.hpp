#pragma once

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/3DEngine//IStatObj.hpp>
#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/IDrawable.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <memory>

struct Material;
using MeshList = std::shared_ptr<std::vector<Mesh>>;

class CStatObj : public IStatObj
{
public:
	CStatObj();
	~CStatObj();

private:
};

