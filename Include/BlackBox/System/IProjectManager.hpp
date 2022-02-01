#pragma once

using CryGUID = int64[2];
class SmartScriptObject; 

namespace Serialization
{
	class Archive
	{

	public:
		enum Type
		{
			INPUT,
			OUTPUT
		};
		Archive(Type type)
			: m_Type(type)
		{
		}
		bool isOutput() { return m_Type == OUTPUT; }
		bool isInput() { return m_Type == INPUT; }
		template<class T>
		bool operator()(const T& value, const char* name, const char* label);

		Type m_Type;
	};
	template<class T>
	bool Archive::operator()(const T& value, const char* name, const char* label)
	{
		// static_assert(HasSerializeOverride<T>(), "Type has no serialize method/override!");
		// return YASLI_SERIALIZE_OVERRIDE(*this, const_cast<T&>(value), name, label);
	}

	using IArchive = Archive;

}

//! Latest version of the project syntax
//! Bump this when syntax changes, or default plug-ins are added
//! This allows us to automatically migrate and support older versions
//! Version 0 = pre-project system, allows for migrating from legacy (game.cfg etc) to .cryproject
constexpr unsigned int LatestProjectFileVersion = 4;

struct SProject
{
	// Serialize the project file
	bool Serialize(SmartScriptObject& ar);

	unsigned int version = 0;
	// why do we need this?
	string type;
	// Project name
	string	name;
	CryGUID guid;
	// Path to the .cryproject file
	string filePath;
	string engineVersionId;

	// Directory containing the .cryproject file
	string rootDirectory;
	// Directory game will search for assets in (relative to project directory)
	string assetDirectory;
	// Full path to the asset directory
	string assetDirectoryFullPath;

	// Directory containing native and managed code (relative to project directory)
	string codeDirectory;

	// List of plug-ins to load
	//std::vector<SPluginDefinition> plugins;

	std::unordered_map<string, string, stl::hash_strcmp<string>> legacyGameDllPaths;

	// Specialized CVar values for the project
	std::map<string, string> consoleVariables;
	// Specialized console commands for the project
	std::map<string, string> consoleCommands;
};

template<unsigned int version>
struct SProjectFileParser
{
};

template<>
struct SProjectFileParser<LatestProjectFileVersion>
{
	void Serialize(Serialization::IArchive& ar, SProject& project)
	{
		struct SRequire
		{
			SRequire(SProject& _project)
				: project(_project)
			{
			}

			void Serialize(Serialization::IArchive& ar)
			{
				ar(project.engineVersionId, "engine", "engine");
				//ar(project.plugins, "plugins", "plugins");
			}

			SProject& project;
		};

		struct SInfo
		{
			SInfo(SProject& _project)
				: project(_project)
			{
			}

			void Serialize(Serialization::IArchive& ar)
			{
				ar(project.name, "name", "name");
				ar(project.guid, "guid", "guid");
			}

			SProject& project;
		};

		struct SContent
		{
			struct SLibrary
			{
				struct SShared
				{
					void Serialize(Serialization::IArchive& ar)
					{
						ar(libPathAny, "any", "any");
						ar(libPathWin64, "win_x64", "win_x64");
						ar(libPathWin32, "win_x86", "win_x86");
					}

					string libPathAny;
					string libPathWin64;
					string libPathWin32;
				};

				void Serialize(Serialization::IArchive& ar)
				{
					ar(name, "name", "name");
					ar(shared, "shared", "shared");
				}

				string	name;
				SShared shared;
			};

			SContent(SProject& _project)
				: project(_project)
			{
			}

			void Serialize(Serialization::IArchive& ar)
			{
				if (ar.isOutput())
				{
					assetDirectories = {project.assetDirectory};
					codeDirectories	 = {project.codeDirectory};

					libraries.resize(1);
					libraries[0].name				 = project.name;
					libraries[0].shared.libPathAny	 = project.legacyGameDllPaths["any"];
					libraries[0].shared.libPathWin64 = project.legacyGameDllPaths["win_x64"];
					libraries[0].shared.libPathWin32 = project.legacyGameDllPaths["win_x86"];
				}

				ar(assetDirectories, "assets", "assets");
				ar(codeDirectories, "code", "code");
				ar(libraries, "libs", "libs");

				if (ar.isInput())
				{
					project.assetDirectory = !assetDirectories.empty() ? assetDirectories.front() : string("");
					project.codeDirectory  = !codeDirectories.empty() ? codeDirectories.front() : string("");

					if (!libraries.empty())
					{
						project.legacyGameDllPaths["any"]	  = libraries[0].shared.libPathAny;
						project.legacyGameDllPaths["win_x64"] = libraries[0].shared.libPathWin64;
						project.legacyGameDllPaths["win_x86"] = libraries[0].shared.libPathWin32;
					}
				}
			}

			std::vector<string>	  assetDirectories;
			std::vector<string>	  codeDirectories;
			std::vector<SLibrary> libraries;

			SProject& project;
		};

		ar(project.type, "type", "type");
		ar(SInfo(project), "info", "info");
		ar(SContent(project), "content", "content");
		ar(SRequire(project), "require", "require");

		ar(project.consoleVariables, "console_variables", "console_variables");
		ar(project.consoleCommands, "console_commands", "console_commands");
	}
};



//! Main interface used to manage the currently run project (known by the .cryproject extension).
struct IProjectManager
{
	virtual ~IProjectManager() {}

	//! Gets the human readable name of the game, for example used for updating the window title on desktop
	virtual const char* GetCurrentProjectName() const = 0;
	virtual bool		ParseProjectFile()			  = 0;
	//! Gets the absolute path to the root of the project directory, where the .cryproject resides.
	//! \return Path without trailing separator.
	virtual const char* GetCurrentProjectDirectoryAbsolute() const = 0;
};