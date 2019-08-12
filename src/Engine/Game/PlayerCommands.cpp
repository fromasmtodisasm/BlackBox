#include <BlackBox/Game/PlayerCommands.hpp>
#include <BlackBox/Game/CPlayer.h>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/Game/CGame.hpp>

MoveForward::MoveForward(CPlayer* player) : player(player), camera(nullptr)
{
  camera = player->getCurrentCamera();
  deltatime = player->getGame()->getDeltaTime();
}

void MoveForward::execute()
{
  camera->ProcessKeyboard(Movement::BACKWARD, deltatime);
}
