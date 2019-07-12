#include <BlackBox/Game/PlayerCommands.hpp>
#include <BlackBox/Game/CPlayer.h>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/Game/CGame.hpp>

MoveForward::MoveForward(CPlayer* player) : player(player), camera(nullptr)
{
  camera = player->getCamera();
  deltatime = player->getGame()->getDeltaTime();
}

void MoveForward::execute()
{
  camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltatime);
}
