#pragma once

#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"
#include "tween/tweener_group.hpp"
#include "glm/gtc/quaternion.hpp"

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
	Keyframe( Vector3lf position, glm::quat rotation, unsigned int entID ) :
m_position(position), m_rotation(rotation), m_selectedEntityID(entID) {}
	Vector3lf m_position;
	glm::quat m_rotation;
	unsigned int m_selectedEntityID;
};

class CameraController {
public:
	CameraController( sf::Vector3f position = sf::Vector3f(0, 0, 0), sf::Vector3f rotation = sf::Vector3f(0, 0, 0) )
		: m_position(position), m_rotation(rotation), m_mouseWheelDelta(0), m_rotationLerpValue(0), m_currentKeyframe(0), m_overriddenSelectionID(0) { }

	virtual void Update( float delta, Application &app ) = 0;

	virtual void AddNewKeyframe( Keyframe &keyframe );

	virtual void PrevKeyframe();

	virtual void NextKeyframe();

	virtual void Transform();

	glm::mat4 m_rotationMatrix;

	Vector3lf m_position, m_rotation;
	unsigned int m_overriddenSelectionID;

	int m_mouseWheelDelta;
	int m_currentKeyframe;
	std::vector<Keyframe> m_keyframes;

protected:
	virtual void ApplyCurrentKeyframe();

	claw::tween::tweener_group m_positionTweeners, m_rotationTweeners;
	

	glm::quat m_previousQuaternion, m_currentQuaternion;
	double m_rotationLerpValue;
};

class FirstPersonCamera : public CameraController {
public:
	virtual void Update( float delta, Application &app );

private:
	sf::Vector2f m_mousePrev;
};