#include "Utils.h"

//#include <CryIcon.h>
//#include <FileUtils.h>
//#include <PathUtils.h>
//#include <QtUtil.h>

//#include <CrySystem/SystemInitParams.h>
//#include <CrySerialization/yasli/JSONIArchive.h>
//#include <CryString/CryStringUtils.h>

#include <QApplication>
#include <QDirIterator>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>
#include <BlackBox/Core/Path.hpp>

string FindCryEngineRootFolder()
{
	QDir dir(qApp->applicationDirPath());
	while (dir.exists())
	{

		QDirIterator iterator(dir.absolutePath(), QStringList() << "cryengine.cryengine", QDir::Files);
		if (iterator.hasNext())
		{
			return QtUtil::ToString(dir.absolutePath());
		}

		if (!dir.cdUp())
		{
			break;
		}
	}

	return "";
}

bool IsProjectSpecifiedInSystemConfig(const string& engineFolder)
{
	const string fileName = PathUtil::Make(engineFolder.data(), "system.cfg");

	QFile configFile(fileName.c_str());
	if (!configFile.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream stream(&configFile);
	while (!stream.atEnd())
	{
		QString line = stream.readLine();
		
		//Simplest check: the line should be started from it, and not commented
		if (0 == line.indexOf("sys_project"))
		{
			return true;
		}
	}
	
	return false;
}

string FindProjectInFolder(const string& folder)
{
	QFileInfo fileInfo(PathUtil::AddSlash(folder).c_str());
	QDirIterator iterator(fileInfo.absolutePath(), QStringList() << "*.cryproject", QDir::Files);
	if (!iterator.hasNext())
	{
		return "";
	}

	iterator.next();
	return QtUtil::ToString(iterator.fileInfo().absoluteFilePath());
}

string AskUserToSpecifyProject(QWidget* pParent, bool runOnSandboxInit, CSelectProjectDialog::Tab tabToShow)
{
	CSelectProjectDialog dlg(pParent, runOnSandboxInit, tabToShow);
	if (runOnSandboxInit)
	{
		const string startupProject = dlg.GetProjectManager().GetStartupProject();
		if (!startupProject.empty())
		{
			return startupProject;
		}
	}

	if (dlg.exec() != QDialog::Accepted)
	{
		return "";
	}

	return dlg.GetPathToProject();
}

void AppendProjectPathToCommandLine(const string& projectPath, SSystemInitParams& systemParams)
{
	cry_strcat(systemParams.szSystemCmdLine, " -project ");
	cry_strcat(systemParams.szSystemCmdLine, projectPath.data());
}

