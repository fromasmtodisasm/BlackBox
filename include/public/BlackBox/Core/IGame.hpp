#pragma once

#ifdef IGAME_EXPORTS
  #define IGAME_API DLL_EXPORT
#else
  #define IGAME_API DLL_IMPORT
#endif

#include <BlackBox/Core/Version.hpp>
#include <BlackBox/Core/MathHelper.hpp>

#include <string>

//////////////////////////////////////////////////////////////////////////
typedef unsigned short EntityClassId;			//< unique identifier for the entity class (defined in ClassRegistry.lua)

//////////////////////////////////////////////////////////////////////////
struct ISystem;
struct IInputHandler;
struct ITagPoint;
struct	IXAreaMgr;

/*
This structure stores the informations to identify an entity type
@see CEntityClassRegistry
*/
//////////////////////////////////////////////////////////////////////////
struct EntityClass
{
	// type id
	EntityClassId				ClassId;
	// class name inside the script file
	string							strClassName;
	// script relative file path
	string							strScriptFile;
	// script fully specified file path (Relative to root folder).
	string							strFullScriptFile;
	// Game type of this entity (Ex. Weapon,Player).
	string							strGameType;
	//specify that this class is not level dependent and is not loaded from LevelData.xml
	bool								bReserved;
	//
	bool								bLoaded;

	EntityClass() { ClassId = 0;bLoaded=false; }
	// Copy constrctor required by STL containers.
	EntityClass( const EntityClass &ec ) { *this = ec; }
	// Copy operator required by STL containers.
	EntityClass& operator=( const EntityClass &ec )
	{
		bReserved=ec.bReserved;
		ClassId = ec.ClassId;
		strClassName = ec.strClassName;
		strScriptFile = ec.strScriptFile;
		strFullScriptFile = ec.strFullScriptFile;
		strGameType = ec.strGameType;
		bLoaded=ec.bLoaded;
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////
/* This interface allows to load or create new entity types
@see CEntityClassRegistry
*/
struct IEntityClassRegistry
{
	/*Retrieves an entity class by name
	@param str entity name
	@return EntityClass ptr if succeded, NULL if failed
	*/
	virtual EntityClass *GetByClass(const char *sClassName,bool bAutoLoadScript=true)= 0;
	//virtual EntityClass *GetByClass(const string &str)= 0;
	/*Retrieves an entity class by ClassId
	@param ClassId class id
	@return EntityClass ptr if succeded, NULL if failed
	*/
	virtual EntityClass *GetByClassId(const EntityClassId ClassId,bool bAutoLoadScript=true)= 0;
	/*Adds a class type into the registry
	@param ClassId class id
	@param sClassName class name(into the script file)
	@param sScriptFile script file path
	@param pLog pointer to the log interface
	@param bForceReload if set to true force script to be eloaded for already registered class.
	@return true if added, false if failed
	*/
	virtual bool AddClass(const EntityClassId ClassId,const char* sClassName,const char* sScriptFile,bool bReserved=false,bool bForceReload=false) = 0;

	/*move the iterator to the begin of the registry
	*/
	virtual void MoveFirst() = 0;
	/*get the next entity class into the registry
	@return a pointer to the next entityclass, or NULL if is the end
	*/
	virtual EntityClass *Next() = 0;
	/*return the count of the entity classes
	@return the count of the entity classes
	*/
	virtual int Count() = 0;

	virtual bool LoadRegistryEntry(EntityClass *pClass, bool bForceReload=false) = 0;
	// debug to OutputDebugString()
	virtual void Debug()=0;
};

// Description of the Game MOD.
//////////////////////////////////////////////////////////////////////////
struct SGameModDescription
{
  // Constructor.
  SGameModDescription()
  {
    dwFlags = 0;
  };

  // Mod's name.
  std::string sName;
  // Mod's title.
  std::string sTitle;
  // Folder where this mod is located.
  std::string sFolder;
  // Mod's author.
  std::string sAuthor;
  // Mod Version.
  SFileVersion version;
  // Mod description.
  std::string sDescription;
  // Website.
  std::string sWebsite;
  // Mod flags
  int	dwFlags;
};

//////////////////////////////////////////////////////////////////////
// Interface to access Game modifications parameters.
struct IGameMods
{
  // Returns description of the currently active game mode.
  // @returns NULL if the game mod is not found.
  virtual const SGameModDescription* GetModDescription(const char* sModName) const = 0;
  // @returns name of the mod currently active, never returns 0
  virtual const char* GetCurrentMod() const = 0;
  // Sets the currently active game mod.
  // @returns true if Mod is successfully set, false if Mod set failed.
  virtual bool SetCurrentMod(const char* sModName, bool bNeedsRestart = false) = 0;
  // Returns modified path for the currently active mod/tc (if any)
  // @returns true if there is an active mod, false otherwise
  virtual const char* GetModPath(const char* szSource) = 0;
};

//////////////////////////////////////////////////////////////////////
struct ITagPointManager
{
  // This function creates a tag point in the game world
  //virtual ITagPoint* CreateTagPoint(const std::std::string& name, const Vec3& pos, const Vec3& angles) = 0;

  // Retrieves a tag point by name
  virtual ITagPoint* GetTagPoint(const std::string& name) = 0;

  // Deletes a tag point from the game
  virtual void RemoveTagPoint(ITagPoint* pPoint) = 0;

  virtual void AddRespawnPoint(ITagPoint* pPoint) = 0;
  virtual void RemoveRespawnPoint(ITagPoint* pPoint) = 0;
};

//////////////////////////////////////////////////////////////////////
enum EGameCapability
{
  EGameMultiplayer = 1,
  EGameClient,
  EGameServer,
  EGameDevMode,
};

//	Exposes the basic functionality to initialize and run the game.
struct IGame {
  virtual bool Init(struct ISystem* pSystem, bool bDedicatedSrv, bool bInEditor, const char* szGameMod) = 0;
  virtual bool Update() = 0;
  virtual bool Run(bool& bRelaunch) = 0;
  virtual void Stop() = 0;
  virtual void SendMessage(const char* s) = 0;
  //Shutdown and destroy the module (delete this)
  virtual void Release() = 0;

  // saves player configuration
  virtual void SaveConfiguration(const char* sSystemCfg, const char* sGameCfg, const char* sProfile) = 0;

  // This is used by editor for changing properties from scripts (no restart).
  virtual void ReloadScripts() = 0;

  virtual bool GetModuleState(EGameCapability eCap) = 0;

  // is called from time to time during loading (usually network updates)
  // currently only called for server map loading
  virtual void UpdateDuringLoading() = 0;

  //virtual ITagPointManager* GetTagPointManager();
  virtual IXAreaMgr* GetAreaManager() = 0;
  virtual ITagPointManager* GetTagPointManager() = 0;
};

extern IGame* p_gIGame;
extern "C" {
  typedef IGame* (*PFNCREATEGAMEINSTANCE)();
// interface of the DLL
  IGAME_API IGame* CreateIGame();
}