#pragma once

#include <IEditor.h>

struct MainMenu : IGuiElement
{
	// Inherited via IGuiElement
	virtual void Draw() override;
};
