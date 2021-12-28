#pragma once

class CGameEngine;

struct IGuiElement
{
	virtual void Draw() = 0;
};

struct IEditor
{
	virtual bool Init() = 0;
	virtual void Draw() = 0;
};

#pragma warning(disable: 4251)