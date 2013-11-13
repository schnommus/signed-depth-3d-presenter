#include "String3D.h"

String3D::String3D( std::string string, SDFont &font, sf::Vector3f position, sf::Vector3f rotation, sf::Shader &shader ) :
	m_font(&font), m_shader(&shader) {

	this->m_position = position;
	this->m_rotation = rotation;
	this->m_colour = sf::Color::White;
	
	m_scale = 0.4f;
	m_string = string;

	Update();
}

void String3D::Draw() {

	glPushMatrix();

		glTranslatef(m_position.x, m_position.y, m_position.z);
		glRotatef(m_rotation.x, 1.f, 0.f, 0.f);
		glRotatef(m_rotation.y, 0.f, 1.f, 0.f);
		glRotatef(m_rotation.z, 0.f, 0.f, 1.f);
		glScalef(m_scale, -m_scale, m_scale); //-ve scale y because y_offset works opposite: strange..
		for( int i = 0; i != m_characters.size(); ++i ) {
			glPushMatrix();
				// Character position is relative to string position
				glTranslatef(m_characters[i].m_position.x, m_characters[i].m_position.y, 0);
				m_characters[i].m_colour = m_colour;
				m_characters[i].Draw();
			glPopMatrix();
		}

	glPopMatrix();
}

void String3D::Update() {
	m_characters.clear();
	m_characters.resize(m_string.size());

	float currentOffset = 0.0f;
	for( int i = 0; i != m_string.size(); ++i ) {
		Character3D *c = &m_characters[i];
		c->Create(m_string[i], *m_font, *m_shader);
		c->m_position.x = currentOffset + c->m_font->GlyphForChar(c->m_character).x_offset;
		float offs = c->m_font->GlyphForChar(c->m_character).y_offset;
		c->m_position.y = -offs;//-c->m_font->GlyphForChar(c->m_character).y_offset;
		currentOffset += c->m_font->GlyphForChar(c->m_character).x_advance;
	}
}

void Character3D::Create( char character, SDFont &font, sf::Shader &shader ) {
	m_font = &font;
	m_shader = &shader;
	m_character = character;
	CreateGeometry();
}

void Character3D::Draw() {
	m_shader->setParameter("finalcolour", this->m_colour);

	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	sf::Texture::bind(&m_font->Font_texture());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), m_verticies);
	glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), m_verticies + 3);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	sf::Shader::bind( m_shader );

	// Draw the verticies
	glDrawArrays(GL_QUADS, 0, 4);

	sf::Shader::bind( 0 );
}

void Character3D::CreateGeometry() {
	const Glyph &myGlyph = m_font->GlyphForChar(m_character);

	GLfloat verts[] = {
		0, 0, 0, myGlyph.u1, myGlyph.v1,
		(float)myGlyph.char_size_x, 0, 0, myGlyph.u2, myGlyph.v1,
		(float)myGlyph.char_size_x, (float)myGlyph.char_size_y, 0, myGlyph.u2, myGlyph.v2,
		0, (float)myGlyph.char_size_y, 0, myGlyph.u1, myGlyph.v2,
	};

	memcpy(m_verticies, verts, (5*4)*sizeof(GLfloat));
}
