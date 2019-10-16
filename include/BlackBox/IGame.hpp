#pragma once

#include <BlackBox/MathHelper.hpp>

#include <string>

//////////////////////////////////////////////////////////////////////////
struct ISystem;
struct IInputHandler;
struct ITagPoint;

//////////////////////////////////////////////////////////////////////
struct ITagPointManager
{
  // This function creates a tag point in the game world
  //virtual ITagPoint* CreateTagPoint(const std::string& name, const Vec3& pos, const Vec3& angles) = 0;

  // Retrieves a tag point by name
  virtual ITagPoint* GetTagPoint(const std::string& name) = 0;

  // Deletes a tag point from the game
  virtual void RemoveTagPoint(ITagPoint* pPoint) = 0;

  virtual void AddRespawnPoint(ITagPoint* pPoint) = 0;
  virtual void RemoveRespawnPoint(ITagPoint* pPoint) = 0;
};


struct IGame {
	virtual bool init(ISystem *pSystem) = 0;
	virtual bool update() = 0;
	virtual bool run() = 0;
  virtual float getDeltaTime() = 0;
  virtual float getTime() = 0;
  virtual float getFPS() = 0;
  virtual void Stop() = 0;

  virtual IInputHandler *getInputHandler() = 0;

};

extern IGame *p_gIGame;
extern "C" {
  IGame* CreateIGame(const char *title);
}
