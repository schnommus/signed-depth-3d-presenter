#pragma once

#include "SDFontReader.h"

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

class String3D {
public:
	String3D( std::string string, SDFontReader &font, sf::Vector3f position, sf::Vector3f rotation );

	void Draw( sf::RenderTarget &target, sf::Shader &shader );

	sf::Vector3f position;
	sf::Vector3f rotation;
private:
	std::string contents;
	SDFontReader &fontReader;

	sf::Image img;
	sf::Texture tex;
};