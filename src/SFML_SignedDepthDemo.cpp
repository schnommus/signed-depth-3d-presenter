#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_SignedDistance", sf::Style::Fullscreen);
    

	sf::Texture sdf; sdf.loadFromFile("../media/sdf1.png");
	sdf.setSmooth(true);
	sf::Sprite sdfSprite(sdf);
	sdfSprite.setOrigin(sdfSprite.getLocalBounds().width/2,sdfSprite.getLocalBounds().height/2);
	sdfSprite.setPosition(window.getSize().x/2, window.getSize().y/2);

	sf::Texture backgroundTex; backgroundTex.loadFromFile("../media/background.jpg");
	sf::Sprite background(backgroundTex);

	sf::Shader sdfShader;
	sdfShader.loadFromFile("../media/sdf.frag", sf::Shader::Fragment);

	sf::Clock delta;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		sdfSprite.setScale( 1.5+sin(delta.getElapsedTime().asSeconds()), 1.5+sin(delta.getElapsedTime().asSeconds()) );
		sdfSprite.setRotation( delta.getElapsedTime().asSeconds()*45.0 );


        window.clear();
		window.draw(background);
        window.draw(sdfSprite, &sdfShader);
        window.display();
    }

    return 0;
}