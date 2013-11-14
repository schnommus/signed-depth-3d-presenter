#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "SDFont.h"
#include "String3D.h"

#include "ResourceManager.h"
#include "CameraController.h"
#include "EntityManager.h"

class Application {
public:
	Application();

	void Initialize();

	void Run();

	const sf::Vector2u &GetSize();

private:
	void Draw();

	void Logic();

	void DrawFPS();

	// Window, OpenGL context
	sf::RenderWindow m_window;

	// Scene camera controllers
	FirstPersonCamera m_firstPersonCamera;

	// Some default assets
	sf::Font m_debugFont;
	sf::Shader m_textShader_default;

	// Members dealing with time
	sf::Clock m_deltaClock;
	float m_delta;
	float m_elapsedTime;

	// Resource management classes
	ResourceManager<SDFont> m_SDFontManager;

	// Entity management classes
	EntityManager m_entitymanager,
				  m_entitymanager_ui;

};