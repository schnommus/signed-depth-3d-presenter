#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "SDFontReader.h"
#include "String3D.h"

int main()
{
	SDFontReader thefont("../media/sdf1.txt", "../media/sdf1.png");
	
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_SignedDistance", sf::Style::Fullscreen);

	sf::Font f; f.loadFromFile("../media/Delicious-Bold.otf");
    
	sf::Texture backgroundTex; backgroundTex.loadFromFile("../media/background.jpg");
	sf::Sprite background(backgroundTex);

	sf::Shader sdfShader;
	sdfShader.loadFromFile("../media/sdf.frag", sf::Shader::Fragment);

	String3D text("DistanceFields", thefont, sf::Vector3f(0, 0, -150), sf::Vector3f(0, 0, 0) );
	String3D text2("!@!@($*#&@(*#", thefont, sf::Vector3f(0, 100, -150), sf::Vector3f(0, 0, 0) );

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

	sf::Clock clock;
	sf::Clock fpsClock;

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

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 0, 100*sin(clock.getElapsedTime().asSeconds()), 0, -150, 0, 1, 0 );

		glClear(GL_DEPTH_BUFFER_BIT);

		text.Draw( window, sdfShader );

		text2.Draw( window, sdfShader );
		text2.rotation.y = clock.getElapsedTime().asSeconds()*50;
		text2.rotation.z = clock.getElapsedTime().asSeconds()*90;

		window.pushGLStates();
			std::ostringstream oss;
			oss << 1.0f/fpsClock.restart().asSeconds() << " FPS";
			sf::Text fps(oss.str(), f );
			window.draw(fps);
		window.popGLStates();

        window.display();
    }

    return 0;
}