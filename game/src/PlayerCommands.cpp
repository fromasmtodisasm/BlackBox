#include <Game.hpp>
#include <PlayerCommands.hpp>
#include <Player.h>
#include <BlackBox/Camera.hpp>

MoveForward::MoveForward(CPlayer* player) : player(player), camera(nullptr)
{
  camera = player->getCurrentCamera();
  //deltatime = player->getGame()->getDeltaTime();
}

void MoveForward::execute()
{
  camera->ProcessKeyboard(Movement::BACKWARD, deltatime);
}
