#pragma once

#include <QIcon>
#include <vector>

// Information required for:
// 1. Represent existing project in GUI
// 2. Update different json configs
struct SProjectDescription
{
	SProjectDescription();

	//void Serialize(Serialization::IArchive& ar);
	int  GetVersionMajor() const;
	int  GetVersionMinor() const;
	bool IsValid() const;

	bool FindAndUpdateCryProjPath();

	string  id;         // Unix data time of the project creation (seconds)
	string  name;
	string  rootFolder;
	int     state;

	string  engineKey;
	string  engineVersion;
	int     engineBuild;

	string  templateName; // Statistics, not used in Sandbox
	string  templatePath; // Statistics, not used in Sandbox
	uint64  lastOpened;   // Unix data time (seconds)
	string  language;     // Can be empty, used by statistics only

	string  fullPathToCryProject; // Used in Sandbox only
	bool    startupProject;

	QIcon icon;
};

// Loads and keeps track of project descriptions
// These information is shared between Sandbox and Launcher in single Json file
class CProjectManager
{
  public:
	CProjectManager();

	const std::vector<SProjectDescription>& GetProjects() const;
	const std::vector<SProjectDescription>& GetHiddenProjects() const;

	const SProjectDescription* GetLastUsedProject() const;
	string					   GetStartupProject() const;
	void					   ImportProject(const string& fullPathToProject);
	void					   AddProject(const SProjectDescription& projectDescr); // Create from template
	void					   ToggleStartupProperty(const SProjectDescription& projectDescr);
	void					   DeleteProject(const SProjectDescription& projectDescr, bool removeFromDisk);
	bool					   SetCurrentTime(const SProjectDescription& projectDescr);
};

