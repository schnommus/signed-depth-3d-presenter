#include "CameraController.h"
#include "Application.h"
#include "tween/easing/easing_quad.hpp"
#include "tween/single_tweener.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <SFML/OpenGL.hpp>

void CameraController::Transform() {
	glLoadMatrixf( glm::value_ptr(m_rotationMatrix) );

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
		m_rotationLerpValue = 0;
		m_rotationTweeners.insert( claw::tween::single_tweener( m_rotationLerpValue, 1.0f, 1.0, claw::tween::easing_quad::ease_in_out ) );

		m_previousQuaternion = glm::quat_cast(m_rotationMatrix);
		m_currentQuaternion = m_keyframes[m_currentKeyframe].m_rotation;
	
		m_overriddenSelectionIDs = m_keyframes[m_currentKeyframe].m_selectedEntityIDs;
	}
}

void CameraController::AddNewKeyframe( Keyframe &keyframe ) {
	m_keyframes.push_back( keyframe );
}



void FirstPersonCamera::Update( float delta, Application &app ) {

	float speed = 100.0*delta;

	// Translate/rotate camera if MMB down
	if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::M) ) {
			for( int i = 0; i != app.SelectedEntityIds().size(); ++i){
				Entity3D* ent = dynamic_cast<Entity3D*>(app.m_entitymanager.GetEntityWithId(app.SelectedEntityIds()[i]));
				ent->m_position.x += m_rotationMatrix[0][0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
				ent->m_position.y += m_rotationMatrix[1][0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
				ent->m_position.z += m_rotationMatrix[2][0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;

				ent->m_position.x -= m_rotationMatrix[0][1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
				ent->m_position.y -= m_rotationMatrix[1][1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
				ent->m_position.z -= m_rotationMatrix[2][1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
			}

		} else {

			if( !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ) {
				m_rotationMatrix = glm::rotate(glm::mat4(), ((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0f, glm::vec3(0.0, 1.0, 0.0) ) * m_rotationMatrix;
				m_rotationMatrix = glm::rotate(glm::mat4(), ((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0f, glm::vec3(1.0, 0.0, 0.0) ) * m_rotationMatrix;
			} else {
				m_position.x -= m_rotationMatrix[0][0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
				m_position.y -= m_rotationMatrix[1][0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
				m_position.z -= m_rotationMatrix[2][0]*speed*((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
				
				m_position.x += m_rotationMatrix[0][1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
				m_position.y += m_rotationMatrix[1][1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
				m_position.z += m_rotationMatrix[2][1]*speed*((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;
			}
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
		if( sf::Keyboard::isKeyPressed(sf::Keyboard::M) ) {
			for( int i = 0; i != app.SelectedEntityIds().size(); ++i){
				Entity3D* ent = dynamic_cast<Entity3D*>(app.m_entitymanager.GetEntityWithId(app.SelectedEntityIds()[i]));
				ent->m_position.x -= zoomSpeed*m_mouseWheelDelta*m_rotationMatrix[0][2]*speed;
				ent->m_position.y -= zoomSpeed*m_mouseWheelDelta*m_rotationMatrix[1][2]*speed;
				ent->m_position.z -= zoomSpeed*m_mouseWheelDelta*m_rotationMatrix[2][2]*speed;
			}
		} else if ( !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ) {
			m_position.x += zoomSpeed*m_mouseWheelDelta*m_rotationMatrix[0][2]*speed;
			m_position.y += zoomSpeed*m_mouseWheelDelta*m_rotationMatrix[1][2]*speed;
			m_position.z += zoomSpeed*m_mouseWheelDelta*m_rotationMatrix[2][2]*speed;
		} else {
			m_rotationMatrix = glm::rotate(glm::mat4(), zoomSpeed*m_mouseWheelDelta*speed*3.0f, glm::vec3(0.0, 0.0, 1.0) ) * m_rotationMatrix;
		}
		m_mouseWheelDelta = 0;
	}

	m_mousePrev.x = (float)sf::Mouse::getPosition().x;
	m_mousePrev.y = (float)sf::Mouse::getPosition().y;

	m_positionTweeners.update( delta );
	m_rotationTweeners.update( delta );

	if(m_rotationLerpValue > 0.001f && m_rotationLerpValue < 0.999f ) {
		m_rotationMatrix = glm::mat4_cast( glm::slerp( m_previousQuaternion, m_currentQuaternion, (float)m_rotationLerpValue ) );
	}
}