#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "SDFontReader.h"

int main()
{
	SDFontReader thefont("../media/sdf1.txt", "../media/sdf1.png");
	
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_SignedDistance", sf::Style::Fullscreen);
    
	sf::Texture backgroundTex; backgroundTex.loadFromFile("../media/background.jpg");
	sf::Sprite background(backgroundTex);

	sf::Shader sdfShader;
	sdfShader.loadFromFile("../media/sdf.frag", sf::Shader::Fragment);
	
	sf::Image textimg = thefont.ImageForString("DistanceFields");
	sf::Texture texttex; texttex.loadFromImage( textimg ); texttex.setSmooth(true);
	sf::Sprite text(texttex);
	text.setOrigin( text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
	text.setPosition(960, 540);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	// Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);


	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	sf::Texture::bind(&texttex);

	float sz_x = texttex.getSize().x/4;
	float sz_y = texttex.getSize().y/4;
	GLfloat cube[] =
    {
        // position, texture coordinates
        0, -sz_x, -sz_y,  0, 0,
        0,  sz_x, -sz_y,  1, 0,
        0, -sz_x,  sz_y,  0, 1,
        0, -sz_x,  sz_y,  0, 1,
        0,  sz_x, -sz_y,  1, 0,
        0,  sz_x,  sz_y,  1, 1,
    };

	 // Enable position and texture coordinates vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), cube);
    glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), cube + 3);

    // Disable normal and color vertex components
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

	sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

		window.pushGLStates();
			window.clear();
			window.draw(background);
		window.popGLStates();


		// Clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // We get the position of the mouse cursor, so that we can move the box accordingly
        float x =  sf::Mouse::getPosition(window).x * 200.f / window.getSize().x - 100.f;
        float y = -sf::Mouse::getPosition(window).y * 200.f / window.getSize().y + 100.f;

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, -150.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 50.f, 1.f, 0.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 30.f, 0.f, 1.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 90.f, 0.f, 0.f, 1.f);

		sf::Shader::bind( &sdfShader );

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 6);

		sf::Shader::bind( 0 );

        window.display();
    }

    return 0;
}