#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "SDFontReader.h"
#include "String3D.h"

#include "tween\single_tweener.hpp"
#include "tween\easing\easing_elastic.hpp"
#include "tween\easing\easing_quad.hpp"
#include "tween\easing\easing_sine.hpp"

int main()
{
	SDFontReader thefont("../media/sdf1.txt", "../media/sdf1.png");
	
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_SignedDistance", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);

	sf::Font f; f.loadFromFile("../media/Delicious-Bold.otf");
    
	sf::Texture backgroundTex; backgroundTex.loadFromFile("../media/background.jpg");
	sf::Sprite background(backgroundTex);

	sf::Shader sdfShader;
	sdfShader.loadFromFile("../media/sdf.frag", sf::Shader::Fragment);

	String3D elements[] = { String3D("OpenGL", thefont, sf::Vector3f(0, 0, 90), sf::Vector3f(0, 0, 0) ),
							String3D("SDFonts", thefont, sf::Vector3f(90, 0, 0), sf::Vector3f(0, 90, 0) ),
							String3D("And", thefont, sf::Vector3f(0, 0, -90), sf::Vector3f(0, 180, 0) ),
							String3D("Tweening", thefont, sf::Vector3f(-90, 0, 0), sf::Vector3f(0, -90, 0) ), };

	claw::tween::single_tweener elementTweeners[4];
	
	double elementFactors[4] = {0};
	
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

	double cameraAngle = 0.0;
	claw::tween::single_tweener cameraTween(cameraAngle, 3.141*2, 2.5, claw::tween::easing_sine::ease_in_out );

	int activeElement = 0;

	sf::Clock clock;
	sf::Clock fpsClock;

    while (window.isOpen())
    {
		float delta = fpsClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				if (event.key.code == sf::Keyboard::Right) {
					cameraTween = claw::tween::single_tweener (cameraAngle, cameraTween.get_end() + 3.141/2.0, 1.0, claw::tween::easing_quad::ease_out );
					++activeElement; if( activeElement > 3 ) activeElement = 0;
				}
				if (event.key.code == sf::Keyboard::Left) {
					cameraTween = claw::tween::single_tweener (cameraAngle, cameraTween.get_end() - 3.141/2.0, 1.0, claw::tween::easing_quad::ease_out );
					--activeElement; if( activeElement < 0 ) activeElement = 3;
				}
			}

            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

		window.pushGLStates();
			window.clear();
			window.draw(background);
		window.popGLStates();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		cameraTween.update(delta);

		gluLookAt(sin(cameraAngle)*150, 0, cos(cameraAngle)*150, 0, 0, 0, 0, 1, 0 );

		glClear(GL_DEPTH_BUFFER_BIT);

		for( int i = 0; i != 4; ++i ) {
			if( i == activeElement ) {
				elementTweeners[i] = claw::tween::single_tweener( elementFactors[i], 1.0, 0.1, claw::tween::easing_sine::ease_in_out );
			} else {
				elementTweeners[i] = claw::tween::single_tweener( elementFactors[i], 0.0, 0.1, claw::tween::easing_sine::ease_in_out );
			}
			elementTweeners[i].update(delta);
			elements[i].position.y = elementFactors[i]*30-30;
			elements[i].colour.a = elementFactors[i]*128+126;
			elements[i].Draw( window, sdfShader );
		}

		window.pushGLStates();
			std::ostringstream oss;
			oss << 1.0f/delta << " FPS";
			sf::Text fps(oss.str(), f );
			window.draw(fps);
		window.popGLStates();

        window.display();
    }

    return 0;
}