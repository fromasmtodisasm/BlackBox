#pragma once
#include <BlackBox/Camera.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/Render/VertexBuffer.hpp>
#include <BlackBox/Scene.hpp>

#include <map>
#include <string>

using std::string;

struct IPostRenderCallback
{
  virtual void PostRender() = 0;
};

struct IPreRenderCallback
{
  virtual void PreRender() = 0;
};

class World {
  friend class GameGUI;
private:
  Scene *activeScene;
  IPostRenderCallback* m_PostRender = nullptr;
  IPreRenderCallback* m_PreRender = nullptr;
public:
  static float gravity;
  World();
  void draw(float dt);

  void setCamera(CCamera *camera);
  void setScene(Scene* scene);
  void update(float deltatime);
  void setPretRenderCallback(IPreRenderCallback* callBack);
  void setPostRenderCallback(IPostRenderCallback* callBack);
  Scene *getActiveScene() const;
};
