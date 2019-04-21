#pragma once

#include <BlackBox/CCamera.hpp>
#include <BlackBox/InputHandler.hpp>


class CameraController : public IInputEventListener
{
	CCamera *m_camera;
public:
	CameraController(CCamera *camera) : m_camera(camera)
	{

	}

	bool OnInputEvent(sf::Event &event)
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			//m_keys.insert(event.key.code);
			return true;
		case sf::Event::KeyReleased:
			//m_keys.erase(event.key.code);
			return true;
		}
		return false;
	}
};
