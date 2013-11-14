#pragma once

#include "SDFont.h"

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"

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

class String3D : public Entity3D {
public:

	String3D();

	virtual void Draw();

	virtual void MouseOver();

	virtual void Update( float delta );

	void UpdateGeometry();

	void SetString( std::string string, SDFont *font, sf::Shader *shader );

	sf::Vector2f m_bounds;
	sf::Color m_colour;

private:
	std::string m_string;
	SDFont *m_font;
	sf::Shader *m_shader;
	std::vector<Character3D> m_characters;
};