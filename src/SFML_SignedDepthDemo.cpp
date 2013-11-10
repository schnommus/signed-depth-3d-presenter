#include <SFML/Graphics.hpp>

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
	text.setPosition(960, 240);

	sf::Font oldFont; oldFont.loadFromFile("../media/delicious-bold.otf");
	sf::Text oldtext( sf::String("NormalText"), oldFont, 100 );
	oldtext.setOrigin( oldtext.getGlobalBounds().width/2, oldtext.getGlobalBounds().height/2 );
	oldtext.setPosition(960, 440);

	sf::Text oldtext2( sf::String("AlphaTest"), oldFont, 100 );
	oldtext2.setOrigin( oldtext2.getGlobalBounds().width/2, oldtext2.getGlobalBounds().height/2 );
	oldtext2.setPosition(960, 690);


	sf::Clock delta;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		text.setScale(6+5*sin(delta.getElapsedTime().asSeconds()),6+5*sin(delta.getElapsedTime().asSeconds()));
		oldtext.setScale(6+5*sin(delta.getElapsedTime().asSeconds()),6+5*sin(delta.getElapsedTime().asSeconds()));
		oldtext2.setScale(6+5*sin(delta.getElapsedTime().asSeconds()),6+5*sin(delta.getElapsedTime().asSeconds()));

        window.clear();
		window.draw(background);

		window.draw(text, &sdfShader);
		window.draw(oldtext);
		window.draw(oldtext2, &sdfShader);

        window.display();
    }

    return 0;
}