#pragma once

#include <SFML/Graphics.hpp>
#include "tween/tweener_group.hpp"

class Application;

struct Vector3lf : public sf::Vector3<double> {
public:
	Vector3lf &operator=( sf::Vector3f &rhs ) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	Vector3lf( const sf::Vector3f &rhs ) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
};

struct Keyframe {
	Keyframe( Vector3lf position, Vector3lf rotation, unsigned int entID ) :
m_position(position), m_rotation(rotation), m_selectedEntityID(entID) {}
	Vector3lf m_position, m_rotation;
	unsigned int m_selectedEntityID;
};

class CameraController {
public:
	CameraController( sf::Vector3f position = sf::Vector3f(0, 0, 0), sf::Vector3f rotation = sf::Vector3f(0, 0, 0) )
		: m_position(position), m_rotation(rotation) { }

	virtual void Update( float delta, Application &app ) = 0;

	virtual void AddNewKeyframe( Keyframe &keyframe );

	virtual void PrevKeyframe();

	virtual void NextKeyframe();

	virtual void Transform();

	Vector3lf m_position, m_rotation;
	unsigned int overriddenSelectionID;

protected:
	virtual void ApplyCurrentKeyframe();

	std::vector<Keyframe> m_keyframes;
	claw::tween::tweener_group m_positionTweeners, m_rotationTweeners;
	int m_currentKeyframe;
};

class FirstPersonCamera : public CameraController {
public:
	virtual void Update( float delta, Application &app );

private:
	sf::Vector2f m_mousePrev;
};