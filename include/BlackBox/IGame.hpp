#pragma once

#include <BlackBox/Version.hpp>
#include <BlackBox/MathHelper.hpp>

#include <string>

#define GameWarning(...) void(0)

//////////////////////////////////////////////////////////////////////////
struct ISystem;
struct IInputHandler;
struct ITagPoint;
struct	IXAreaMgr;

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
  virtual bool Init(ISystem* pSystem) = 0;
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
  IGame* CreateIGame(const char* title);
}