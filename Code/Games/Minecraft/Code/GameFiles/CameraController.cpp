#include "pch.hpp"
#include "CameraController.hpp"
#include "Minecraft.h"

void CCameraController::ProcessKeyboard(Movement direction, float deltaTime, float value)
{
	float velocity      = CurrentCamera()->MovementSpeed * deltaTime * value;

	auto  moveDirection = glm::vec3(
        CurrentCamera()->Front.x,
        CurrentCamera()->mode == CCamera::Mode::FPS ? 0 : CurrentCamera()->Front.y,
        CurrentCamera()->Front.z);

	if (direction == Movement::FORWARD)
	{
		minecraft->player.move(moveDirection, velocity);
	}
	if (direction == Movement::BACKWARD)
	{
		minecraft->player.move(moveDirection, -velocity);
	}
	if (direction == Movement::LEFT)
	{
		minecraft->player.move(CurrentCamera()->Right, velocity);
	}
	if (direction == Movement::RIGHT)
	{
		minecraft->player.move(CurrentCamera()->Right, -velocity);
	}
}
