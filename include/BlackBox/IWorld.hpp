#pragma once

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
  void draw(float dt);

  void setCamera(CCamera* camera);
  void setScene(Scene* scene);
  void update(float deltatime);
  void setPretRenderCallback(IPreRenderCallback* callBack);
  void setPostRenderCallback(IPostRenderCallback* callBack);
  Scene* getActiveScene() const;
};