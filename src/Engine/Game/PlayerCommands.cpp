#include <BlackBox/PlayerCommands.hpp>
#include <BlackBox/CPlayer.h>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/CGame.hpp>

MoveForward::MoveForward(CPlayer* player) : player(player), camera(nullptr)
{
  camera = player->getCamera();
  deltatime = player->getGame()->getDeltaTime();
}

void MoveForward::execute()
{
  camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltatime);
}
