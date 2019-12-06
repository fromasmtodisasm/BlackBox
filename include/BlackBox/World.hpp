#pragma once
#include <BlackBox/IWorld.hpp>

#include <map>
#include <string>

class Scene;

class World : public IWorld
{
  //friend class GameGUI;
private:
  Scene* activeScene;
  IPostRenderCallback* m_PostRender = nullptr;
  IPreRenderCallback* m_PreRender = nullptr;
public:
  static float gravity;
  World();
  void Draw(float dt);

  void SetCamera(CCamera* camera);
  void SetScene(IScene* scene);
  void Update(float deltatime);
  void SetPretRenderCallback(IPreRenderCallback* callBack);
  void SetPostRenderCallback(IPostRenderCallback* callBack);
  IScene* GetActiveScene();
};