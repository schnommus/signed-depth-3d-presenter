#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "SDFont.h"
#include "String3D.h"

#include "ResourceManager.h"

class Application {
public:
	Application();

	void Initialize();

	void Run();

private:
	void Draw();

	void Logic();

	void DrawFPS();

	// Window, OpenGL context
	sf::RenderWindow m_window;

	// Some default assets
	sf::Font m_debugFont;
	sf::Shader m_textShader_default;

	// Members dealing with time
	sf::Clock m_deltaClock;
	float m_delta;
	float m_elapsedTime;

	// Resource management classes
	ResourceManager<SDFont> m_SDFontManager;

	// TODO: Remove!
	String3D *str;
};