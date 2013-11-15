#pragma once

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

#include <vector>

#include "SDFont.h"
#include "String3D.h"

#include "ResourceManager.h"
#include "CameraController.h"
#include "EntityManager.h"

#define PICK_BUFFER_SIZE 256
#define PICK_TOL 10

class Application {
public:
	Application();

	void Initialize();

	void Run();

	const sf::Vector2u &GetSize();

	float Delta() const { return m_delta; }

	unsigned int SelectedEntityId() const { return m_selectedEntityId; }

	// Window, OpenGL context
	sf::RenderWindow m_window;

	// Dealing with UI elements
	sfg::Desktop m_desktop;
	sfg::SFGUI m_sfgui;

	// Entity management classes
	EntityManager m_entitymanager,
		m_entitymanager_ui,
		m_entitymanager_background;

private:
	void Draw();

	void Draw_OpenGL(bool selection);

	void Logic();

	void DrawFPS();

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

	// For 3D picking operations
	unsigned int m_pickBuffer[PICK_BUFFER_SIZE];

	// (-1 if none)
	unsigned int m_selectedEntityId;
};