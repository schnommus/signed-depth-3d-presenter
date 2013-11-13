#pragma once

#include "SDFont.h"

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

class Character3D {
public:
	friend class String3D;

	void Create( char character, SDFont &font, sf::Shader &shader );
	void Draw();

	sf::Color m_colour;
	sf::Vector3f m_position;
	sf::Vector3f m_rotation;

private:
	void CreateGeometry();

	GLfloat m_verticies[4*5]; // 1 Quad, 4 verts.
	SDFont *m_font;
	sf::Shader *m_shader;
	char m_character;
};

class String3D {
public:

	String3D( std::string string, SDFont &font, sf::Vector3f position, sf::Vector3f rotation, sf::Shader &shader  );

	void Draw();

	void Update();

	sf::Vector3f m_position;
	sf::Vector3f m_rotation;
	sf::Vector2f m_bounds;
	sf::Color m_colour;
	float m_scale;

private:
	std::string m_string;
	SDFont *m_font;
	sf::Shader *m_shader;
	std::vector<Character3D> m_characters;
};