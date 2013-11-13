#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "SDFont.h"
#include "String3D.h"

#include "tween\single_tweener.hpp"
#include "tween\easing\easing_elastic.hpp"
#include "tween\easing\easing_quad.hpp"
#include "tween\easing\easing_sine.hpp"

namespace tween = claw::tween;

int main()
{
	SDFont thefont("../media/sdf1.txt", "../media/sdf1.png");
	
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_SignedDistance", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(false);

	sf::Font f; f.loadFromFile("../media/Delicious-Bold.otf");
    
	sf::Texture backgroundTex; backgroundTex.loadFromFile("../media/background.jpg");
	sf::Sprite background(backgroundTex);

	sf::Shader sdfShader;
	sdfShader.loadFromFile("../media/sdf.frag", sf::Shader::Fragment);

	String3D elements[] = { String3D("opengl", thefont, sf::Vector3f(0, 0, 90), sf::Vector3f(0, 0, 0), sdfShader ),
							String3D("sdfonts", thefont, sf::Vector3f(90, 0, 0), sf::Vector3f(0, 90, 0), sdfShader ),
							String3D("and", thefont, sf::Vector3f(0, 0, -90), sf::Vector3f(0, 180, 0), sdfShader ),
							String3D("tweening", thefont, sf::Vector3f(-90, 0, 0), sf::Vector3f(0, -90, 0), sdfShader ), };

	tween::single_tweener elementTweeners[4];
	
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

	int activeElement = 0;

	sf::Clock clock;
	sf::Clock fpsClock;

	sf::Vector3f camera_rotation(0, 0, 0);
	sf::Vector3f camera_rotation_rad(0, 0, 0);
	sf::Vector3f camera_position(0, 0, 150);

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
					++activeElement; if( activeElement > 3 ) activeElement = 0;
				}
				if (event.key.code == sf::Keyboard::Left) {
					--activeElement; if( activeElement < 0 ) activeElement = 3;
				}
			}

            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

		if(sf::Mouse::isButtonPressed(sf::Mouse::Middle) ) {
			camera_rotation.y += ((float)sf::Mouse::getPosition().x-(float)window.getSize().x/2)/7.0;
			camera_rotation.x += ((float)sf::Mouse::getPosition().y-(float)window.getSize().y/2)/7.0;
			sf::Mouse::setPosition(sf::Vector2i(window.getSize().x/2, window.getSize().y/2), window);
		}

		//camera_rotation_rad = sf::Vector3f(camera_rotation.x/180*3.14159, camera_rotation.y/180*3.14159, camera_rotation.z/180*3.14159 );
		
		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ) {
			camera_rotation.z -= 100.0f*delta;
		} if( sf::Keyboard::isKeyPressed(sf::Keyboard::E) ) {
			camera_rotation.z += 100.0f*delta;
		}

		window.pushGLStates();
			window.clear();
			window.draw(background);
		window.popGLStates();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(camera_rotation.x, 1.f, 0.f, 0.f);
		glRotatef(camera_rotation.y, 0.f, 1.f, 0.f);
		glRotatef(camera_rotation.z, 0.f, 0.f, 1.f);

		GLfloat modelMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);

		float speed = 100.0*delta;
		if( sf::Keyboard::isKeyPressed(sf::Keyboard::W) ) {
			camera_position.x -= modelMatrix[0*4+2]*speed;
			camera_position.y -= modelMatrix[1*4+2]*speed;
			camera_position.z -= modelMatrix[2*4+2]*speed;
		} if( sf::Keyboard::isKeyPressed(sf::Keyboard::S) ) {
			camera_position.x += modelMatrix[0*4+2]*speed;
			camera_position.y += modelMatrix[1*4+2]*speed;
			camera_position.z += modelMatrix[2*4+2]*speed;
		} if( sf::Keyboard::isKeyPressed(sf::Keyboard::A) ) {
			camera_position.x -= modelMatrix[0*4+0]*speed;
			camera_position.y -= modelMatrix[1*4+0]*speed;
			camera_position.z -= modelMatrix[2*4+0]*speed;
		} if( sf::Keyboard::isKeyPressed(sf::Keyboard::D) ) {
			camera_position.x += modelMatrix[0*4+0]*speed;
			camera_position.y += modelMatrix[1*4+0]*speed;
			camera_position.z += modelMatrix[2*4+0]*speed;
		} if( sf::Keyboard::isKeyPressed(sf::Keyboard::F) ) {
			camera_position.x -= modelMatrix[0*4+1]*speed;
			camera_position.y -= modelMatrix[1*4+1]*speed;
			camera_position.z -= modelMatrix[2*4+1]*speed;
		} if( sf::Keyboard::isKeyPressed(sf::Keyboard::R) ) {
			camera_position.x += modelMatrix[0*4+1]*speed;
			camera_position.y += modelMatrix[1*4+1]*speed;
			camera_position.z += modelMatrix[2*4+1]*speed;
		}


		glTranslatef(-camera_position.x, -camera_position.y, -camera_position.z );

		glClear(GL_DEPTH_BUFFER_BIT);

		for( int i = 0; i != 4; ++i ) {
			if( i == activeElement ) {
				elementTweeners[i] = tween::single_tweener( elementFactors[i], 1.0, 0.04, claw::tween::easing_sine::ease_in_out );
			} else {
				elementTweeners[i] = tween::single_tweener( elementFactors[i], 0.0, 0.04, claw::tween::easing_sine::ease_in_out );
			}
			elementTweeners[i].update(delta);
			elements[i].m_position.y = elementFactors[i]*30-30;
			elements[i].m_colour.a = elementFactors[i]*128+126;
			elements[i].Draw();
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