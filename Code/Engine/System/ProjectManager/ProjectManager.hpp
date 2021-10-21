#pragma once

#include <BlackBox/System/IProjectManager.hpp>

class CProjectManager : public IProjectManager
{
public:
	CProjectManager();
	// Inherited via IProjectManager
	virtual const char* GetCurrentProjectName() const override;
	virtual bool ParseProjectFile() override;

protected : 
	void RegisterCVars();

	// Legacy CVars
	ICVar* m_sys_game_name;
	ICVar* m_sys_dll_game;
	ICVar* m_sys_game_folder;
};