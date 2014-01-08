#include "CameraController.h"
#include "Application.h"
#include "tween/easing/easing_quad.hpp"
#include "tween/single_tweener.hpp"

#include <SFML/OpenGL.hpp>

void CameraController::Transform() {
	glRotatef(-m_rotation.x, 1.f, 0.f, 0.f);
	glRotatef(-m_rotation.y, 0.f, 1.f, 0.f);
	glRotatef(-m_rotation.z, 0.f, 0.f, 1.f);

	glTranslatef(-m_position.x, -m_position.y, -m_position.z );
}

void CameraController::PrevKeyframe() {
	if( m_currentKeyframe > 0) --m_currentKeyframe;
	ApplyCurrentKeyframe();
}

void CameraController::NextKeyframe() {
	if( m_currentKeyframe != m_keyframes.size()-1) ++m_currentKeyframe;
	ApplyCurrentKeyframe();
}

void CameraController::ApplyCurrentKeyframe() {
	if( m_keyframes.size() > m_currentKeyframe ) {
		m_positionTweeners.clear();
		m_positionTweeners.insert( claw::tween::single_tweener( m_position.x, m_keyframes[m_currentKeyframe].m_position.x, 1.0, claw::tween::easing_quad::ease_in_out ) );
		m_positionTweeners.insert( claw::tween::single_tweener( m_position.y, m_keyframes[m_currentKeyframe].m_position.y, 1.0, claw::tween::easing_quad::ease_in_out ) );
		m_positionTweeners.insert( claw::tween::single_tweener( m_position.z, m_keyframes[m_currentKeyframe].m_position.z, 1.0, claw::tween::easing_quad::ease_in_out ) );

		m_rotationTweeners.clear();
		m_rotationTweeners.insert( claw::tween::single_tweener( m_rotation.x, m_keyframes[m_currentKeyframe].m_rotation.x, 1.0, claw::tween::easing_quad::ease_in_out ) );
		m_rotationTweeners.insert( claw::tween::single_tweener( m_rotation.y, m_keyframes[m_currentKeyframe].m_rotation.y, 1.0, claw::tween::easing_quad::ease_in_out ) );
		m_rotationTweeners.insert( claw::tween::single_tweener( m_rotation.z, m_keyframes[m_currentKeyframe].m_rotation.z, 1.0, claw::tween::easing_quad::ease_in_out ) );
	
		overriddenSelectionID = m_keyframes[m_currentKeyframe].m_selectedEntityID;
	}
}

void CameraController::AddNewKeyframe( Keyframe &keyframe ) {
	m_keyframes.push_back( keyframe );
}



void FirstPersonCamera::Update( float delta, Application &app ) {

	// Load a blank matrix (Outside draw loop; nothing is overwritten)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Transform based on rotation
	glRotatef(-m_rotation.x, 1.f, 0.f, 0.f);
	glRotatef(-m_rotation.y, 0.f, 1.f, 0.f);
	glRotatef(-m_rotation.z, 0.f, 0.f, 1.f);

	// Pull it into memory
	GLfloat modelMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);

	glLoadIdentity();

	float speed = 100.0*delta;

	// Translate/rotate camera if MMB down
	if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {

		if( !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ) {
			m_rotation.y -= ((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
			m_rotation.x -= ((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
		} else {
			m_position.x -= modelMatrix[0*4+0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
			m_position.y -= modelMatrix[1*4+0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
			m_position.z -= modelMatrix[2*4+0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
			m_position.x += modelMatrix[0*4+1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
			m_position.y += modelMatrix[1*4+1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
			m_position.z += modelMatrix[2*4+1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
		}

		// Wrap screen when mouse hits edges
		if( sf::Mouse::getPosition().x == app.GetSize().x-1 )
			sf::Mouse::setPosition( sf::Vector2i(0, sf::Mouse::getPosition().y) );
		else if( sf::Mouse::getPosition().y == app.GetSize().y-1 )
			sf::Mouse::setPosition( sf::Vector2i(sf::Mouse::getPosition().x, 0) );
		else if( sf::Mouse::getPosition().x == 0 )
			sf::Mouse::setPosition( sf::Vector2i(app.GetSize().x-1, sf::Mouse::getPosition().y) );
		else if( sf::Mouse::getPosition().y == 0 )
			sf::Mouse::setPosition( sf::Vector2i(sf::Mouse::getPosition().x, app.GetSize().y-1) );
	}

	float zoomSpeed = 5.0f;
	if( m_mouseWheelDelta != 0 ) {
		m_position.x += zoomSpeed*m_mouseWheelDelta*modelMatrix[0*4+2]*speed;
		m_position.y += zoomSpeed*m_mouseWheelDelta*modelMatrix[1*4+2]*speed;
		m_position.z += zoomSpeed*m_mouseWheelDelta*modelMatrix[2*4+2]*speed;
		m_mouseWheelDelta = 0;
	}

	m_mousePrev.x = (float)sf::Mouse::getPosition().x;
	m_mousePrev.y = (float)sf::Mouse::getPosition().y;

	// Left/Right roll controls
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ) {
		m_rotation.z -= 100.0f*delta;
	} if( sf::Keyboard::isKeyPressed(sf::Keyboard::E) ) {
		m_rotation.z += 100.0f*delta;
	}

	m_positionTweeners.update( delta );
	m_rotationTweeners.update( delta );
}