#include "CameraController.h"
#include "Application.h"

#include <SFML/OpenGL.hpp>

void CameraController::Transform() {
	glRotatef(m_rotation.x, 1.f, 0.f, 0.f);
	glRotatef(m_rotation.y, 0.f, 1.f, 0.f);
	glRotatef(m_rotation.z, 0.f, 0.f, 1.f);

	glTranslatef(-m_position.x, -m_position.y, -m_position.z );
}


void FirstPersonCamera::Update( float delta, Application &app ) {
	// Rotate camera if MMB down
	if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		m_rotation.y += ((float)sf::Mouse::getPosition().x-m_mousePrev.x)/7.0;
		m_rotation.x += ((float)sf::Mouse::getPosition().y-m_mousePrev.y)/7.0;

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

	m_mousePrev.x = (float)sf::Mouse::getPosition().x;
	m_mousePrev.y = (float)sf::Mouse::getPosition().y;

	// Left/Right roll controls
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ) {
		m_rotation.z -= 100.0f*delta;
	} if( sf::Keyboard::isKeyPressed(sf::Keyboard::E) ) {
		m_rotation.z += 100.0f*delta;
	}

	// Load a blank matrix (Outside draw loop; nothing is overwritten)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Transform based on rotation
	glRotatef(m_rotation.x, 1.f, 0.f, 0.f);
	glRotatef(m_rotation.y, 0.f, 1.f, 0.f);
	glRotatef(m_rotation.z, 0.f, 0.f, 1.f);

	// Pull it into memory
	GLfloat modelMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);

	// Use for translation directions
	float speed = 100.0*delta;
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::W) ) {
		m_position.x -= modelMatrix[0*4+2]*speed;
		m_position.y -= modelMatrix[1*4+2]*speed;
		m_position.z -= modelMatrix[2*4+2]*speed;
	} if( sf::Keyboard::isKeyPressed(sf::Keyboard::S) ) {
		m_position.x += modelMatrix[0*4+2]*speed;
		m_position.y += modelMatrix[1*4+2]*speed;
		m_position.z += modelMatrix[2*4+2]*speed;
	} if( sf::Keyboard::isKeyPressed(sf::Keyboard::A) ) {
		m_position.x -= modelMatrix[0*4+0]*speed;
		m_position.y -= modelMatrix[1*4+0]*speed;
		m_position.z -= modelMatrix[2*4+0]*speed;
	} if( sf::Keyboard::isKeyPressed(sf::Keyboard::D) ) {
		m_position.x += modelMatrix[0*4+0]*speed;
		m_position.y += modelMatrix[1*4+0]*speed;
		m_position.z += modelMatrix[2*4+0]*speed;
	} if( sf::Keyboard::isKeyPressed(sf::Keyboard::F) ) {
		m_position.x -= modelMatrix[0*4+1]*speed;
		m_position.y -= modelMatrix[1*4+1]*speed;
		m_position.z -= modelMatrix[2*4+1]*speed;
	} if( sf::Keyboard::isKeyPressed(sf::Keyboard::R) ) {
		m_position.x += modelMatrix[0*4+1]*speed;
		m_position.y += modelMatrix[1*4+1]*speed;
		m_position.z += modelMatrix[2*4+1]*speed;
	}
}
