#pragma once

class CCamera;
struct IScene;
class SkyBox;

struct IPostRenderCallback
{
  virtual void PostRender() = 0;
};

struct IPreRenderCallback
{
  virtual void PreRender() = 0;
};

struct IWorld 
{
public:
  virtual void Draw(float dt) = 0;
  virtual void SetCamera(CCamera* camera) = 0;
  virtual void SetScene(IScene* scene) = 0;
  virtual void Update(float deltatime) = 0;
  virtual void SetPretRenderCallback(IPreRenderCallback* callBack) = 0;
  virtual void SetPostRenderCallback(IPostRenderCallback* callBack) = 0;
  virtual IScene* GetActiveScene() = 0;
};
