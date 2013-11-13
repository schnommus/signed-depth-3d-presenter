#pragma once

#include <SFML/Graphics.hpp>

class Application;

class CameraController {
public:
	CameraController( sf::Vector3f position = sf::Vector3f(0, 0, 0), sf::Vector3f rotation = sf::Vector3f(0, 0, 0) )
		: m_position(position), m_rotation(rotation) { }

	virtual void Update( float delta, Application &app ) = 0;

	virtual void Transform();

	sf::Vector3f m_position, m_rotation;
};

class FirstPersonCamera : public CameraController {
public:
	virtual void Update( float delta, Application &app );
private:
	sf::Vector2f m_mousePrev;
};