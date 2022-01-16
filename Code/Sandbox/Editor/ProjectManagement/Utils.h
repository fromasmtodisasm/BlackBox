// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "ProjectManagement/UI/SelectProjectDialog.h"
//#include <CrySerialization/Forward.h>

struct SSystemInitParams;

class CryIcon;
class QWidget;

// Searches for cryengine.cryengine from this EXE-folder, and moving higher
string FindCryEngineRootFolder();

// In the engineFolder, find system.cfg;
// Return true, if any line of this file starts with "sys_project"
bool IsProjectSpecifiedInSystemConfig(const string& engineFolder);

// Find first *.cryproject file in the folder (without search in subfolders)
string FindProjectInFolder(const string& folder);

// Ask user to select a project
string AskUserToSpecifyProject(QWidget* pParent, bool runOnSandboxInit, CSelectProjectDialog::Tab tabToShow);

// Unified way of adding project path to command line
void AppendProjectPathToCommandLine(const string& projectPath, SSystemInitParams& systemParams);

