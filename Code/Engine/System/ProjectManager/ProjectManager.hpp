#pragma once

#include <BlackBox/System/IProjectManager.hpp>

class CProjectManager : public IProjectManager
{
public:
	CProjectManager();
	// Inherited via IProjectManager
	virtual const char* GetCurrentProjectName() const override;
	virtual bool ParseProjectFile() override;
	virtual const char*  GetCurrentProjectDirectoryAbsolute() const override;

protected : 
	void RegisterCVars();
protected : 
	SProject m_project;

	ICVar* m_sys_project{};

	// Legacy CVars
	ICVar* m_sys_game_name{};
	ICVar* m_sys_dll_game{};
	ICVar* m_sys_game_folder{};
};