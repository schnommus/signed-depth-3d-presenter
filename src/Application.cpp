#include "Application.h"

#include <SFML/OpenGL.hpp>

#include <iostream>
#include <sstream>
#include <cmath>

#include "Background.h"

Application::Application()
	: m_window(sf::VideoMode::getDesktopMode(), "Application", sf::Style::Fullscreen ), 
	  m_entitymanager(this), m_entitymanager_ui(this), m_entitymanager_background(this)
{ std::cout << "Opened context." << std::endl; }

void Application::Initialize() {
	m_window.setVerticalSyncEnabled(false);

	// Fetch some default assets
	std::cout << "Loading default assets..." << std::endl;
	m_debugFont.loadFromFile("../media/font_debug.ttf");
	m_textShader_default.loadFromFile("../media/text_default.frag", sf::Shader::Fragment);

	// Set up OpenGL environment
	std::cout << "Fiddling with OpenGL..." << std::endl;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	// Configure the viewport (the same size as the window)
	// TODO: Take care of relativity: new sizes; text-clipping?
	glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);

	// Assign picking buffer
	glSelectBuffer( PICK_BUFFER_SIZE, m_pickBuffer );

	m_entitymanager_background.AddEntity( new Background() );

	for( int i = 0; i != 200; ++i ) {
		String3D *str = dynamic_cast<String3D*>(m_entitymanager.AddEntity( new String3D ));
		str->SetString("Text", &m_SDFontManager.Fetch("../media/sdf1.txt", "../media/sdf1.png"), &m_textShader_default);
		str->m_position.x = rand()%500-250;
		str->m_position.y = rand()%500-250;
		str->m_position.z = rand()%500-250;
		str->m_rotation.x = rand()%360;
		str->m_rotation.y = rand()%360;
		str->m_rotation.z = rand()%360;
	}
}

void Application::Run() {
	std::cout << "Entering core logic..." << std::endl;
	while (m_window.isOpen()) {
		Logic();
		Draw();
	}
}

void Application::Logic() {
	// Handle time
	m_delta = m_deltaClock.restart().asSeconds();
	m_elapsedTime += m_delta;

	// Check for events
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();

		// TODO: Remove. Use Alt-F4 for quitting instead; internal GUI quit logic
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape ) {
			m_window.close();
		}

		if (event.type == sf::Event::Resized)
			glViewport(0, 0, event.size.width, event.size.height);
	}

	// Camera updates
	m_firstPersonCamera.Update(m_delta, *this);

	// Other logic
	m_entitymanager_background.UpdateEntities( m_delta );
	m_entitymanager_ui.UpdateEntities( m_delta );
	m_entitymanager.UpdateEntities( m_delta );

	m_entitymanager_ui.DoDeletions();
	m_entitymanager.DoDeletions();
	m_entitymanager_background.DoDeletions();
}

void Application::Draw() {
	m_window.pushGLStates();
		m_window.clear(); // Could also draw 2D background here
		m_entitymanager_background.DrawEntities();
	m_window.popGLStates();

	glRenderMode( GL_SELECT );
		Draw_OpenGL(true);
	int nHits = glRenderMode( GL_RENDER );
		Draw_OpenGL(false);

	if(nHits > 0)
		m_entitymanager.GetEntityWithId(m_pickBuffer[3])->MouseOver();

	m_window.pushGLStates();

		// SFML DRAWING HERE
		m_entitymanager_ui.DrawEntities();

		DrawFPS();

	m_window.popGLStates();

	m_window.display();
}

void Application::Draw_OpenGL(bool selection) {
	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if( selection ) {
		int viewport[4] = {0, 0, m_window.getSize().x, m_window.getSize().y};
		gluPickMatrix( (double)sf::Mouse::getPosition().x, (double)m_window.getSize().y-(double)sf::Mouse::getPosition().y, PICK_TOL, PICK_TOL, viewport );
	}

	GLfloat ratio = static_cast<float>(m_window.getSize().x) / m_window.getSize().y;
	glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

	// Blank all names before draws
	glInitNames();
	glPushName(0xFFFFFFFF);

	// Begin camera-space rendering
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// CAMERA TRANSFORMS HERE
	m_firstPersonCamera.Transform();

	glClear(GL_DEPTH_BUFFER_BIT);

	// OPENGL DRAWING HERE
	m_entitymanager.DrawEntities();
}

void Application::DrawFPS() {
	std::ostringstream oss;
	oss << 1.0f/m_delta << " FPS";
	sf::Text fps(oss.str(), m_debugFont, 20U );
	m_window.draw(fps);
}

const sf::Vector2u &Application::GetSize() {
	return m_window.getSize();
}
