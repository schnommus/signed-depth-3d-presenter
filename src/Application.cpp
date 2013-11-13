#include "Application.h"

#include <SFML/OpenGL.hpp>

#include <iostream>
#include <sstream>

Application::Application()
	: m_window(sf::VideoMode::getDesktopMode(), "Application", sf::Style::Fullscreen )
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

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio = static_cast<float>(m_window.getSize().x) / m_window.getSize().y;
	glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

	str = new String3D("Hello :D", m_SDFontManager.Fetch("../media/sdf1.txt", "../media/sdf1.png"), sf::Vector3f(0, 0, -100), sf::Vector3f(0, 0, 0), m_textShader_default );
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
}

void Application::Draw() {
	m_window.pushGLStates();
		m_window.clear(); // Could also draw 2D background here
	m_window.popGLStates();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// CAMERA TRANSFORM GOES HERE

	glClear(GL_DEPTH_BUFFER_BIT);

	// ALL OTHER OPENGL DRAWING HERE
	str->Draw();

	m_window.pushGLStates();

		// ALL OTHER SFML DRAWING HERE

		DrawFPS();

	m_window.popGLStates();

	m_window.display();
}

void Application::DrawFPS() {
	std::ostringstream oss;
	oss << 1.0f/m_delta << " FPS";
	sf::Text fps(oss.str(), m_debugFont, 20U );
	m_window.draw(fps);
}
