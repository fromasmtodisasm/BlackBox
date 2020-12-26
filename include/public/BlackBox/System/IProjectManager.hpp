#pragma once

//! Main interface used to manage the currently run project (known by the .cryproject extension).
struct IProjectManager
{
	virtual ~IProjectManager() {}

	//! Gets the human readable name of the game, for example used for updating the window title on desktop
	virtual const char* GetCurrentProjectName() const = 0;
	virtual bool ParseProjectFile()					  = 0;
};