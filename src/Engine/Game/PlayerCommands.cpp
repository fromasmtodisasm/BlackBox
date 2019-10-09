#include <BlackBox/Game/PlayerCommands.hpp>
#include <BlackBox/Game/Player.h>
#include <BlackBox/Camera.hpp>
#include <BlackBox/Game/Game.hpp>

MoveForward::MoveForward(CPlayer* player) : player(player), camera(nullptr)
{
  camera = player->getCurrentCamera();
  deltatime = player->getGame()->getDeltaTime();
}

void MoveForward::execute()
{
  camera->ProcessKeyboard(Movement::BACKWARD, deltatime);
}
