#include "String3D.h"

String3D::String3D( std::string string, SDFontReader &font, sf::Vector3f position, sf::Vector3f rotation ) :
	fontReader(font) {

	this->position = position;
	this->rotation = rotation;

	this->colour = sf::Color::White;

	contents = string;
	img = font.ImageForString(contents);
	tex.loadFromImage( img );
	tex.setSmooth(true);
	
}

void String3D::Draw( sf::RenderTarget &target, sf::Shader &shader ) {
	shader.setParameter("finalcolour", this->colour);

	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	sf::Texture::bind(&tex);

	float sz_x = tex.getSize().x/4;
	float sz_y = tex.getSize().y/4;

	GLfloat verticies[] = {
        // position, texture coordinates
        -sz_x, -sz_y, 0,  0, 1,
        sz_x,  -sz_y, 0,  1, 1,
        -sz_x, sz_y,  0,  0, 0,
        -sz_x, sz_y,  0,  0, 0,
        sz_x,  -sz_y, 0,  1, 1,
        sz_x,  sz_y,  0,  1, 0,
    };

	 // Enable position and texture coordinates vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), verticies);
    glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), verticies + 3);

	glPushMatrix();

		glTranslatef(position.x, position.y, position.z);
		glRotatef(rotation.x, 1.f, 0.f, 0.f);
		glRotatef(rotation.y, 0.f, 1.f, 0.f);
		glRotatef(rotation.z, 0.f, 0.f, 1.f);

		sf::Shader::bind( &shader );

		// Draw the verticies
		glDrawArrays(GL_TRIANGLES, 0, 6);

		sf::Shader::bind( 0 );

	glPopMatrix();
}