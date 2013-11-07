#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> &split(std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
		if(!item.empty())
			elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

struct glyph {
	int id;
	int x, y, w, h;
	float off;
};

int main()
{
	std::ifstream file;
	file.open("../media/sdf1.txt", std::ios::in );
	std::string line;

	std::vector< glyph > glyphs;

	while( getline (file, line) ) {
      std::vector< std::string > elements = split(line, ' ');
	  if( elements[0] == "char" ) {
		  glyph g;
		  std::istringstream idstr(split( elements[1], '=' )[1]); idstr >> g.id;
		  std::cout << "id: " << g.id  << " \'" << (char)g.id << "\', ";
		  std::istringstream xstr(split( elements[2], '=' )[1]); xstr >> g.x;
		  std::cout << "x: " << g.x << ", ";
		  std::istringstream ystr(split( elements[3], '=' )[1]); ystr >> g.y;
		  std::cout << "y: " << g.y << ", ";
		  std::istringstream wstr(split( elements[4], '=' )[1]); wstr >> g.w;
		  std::cout << "w: " << g.w << ", ";
		  std::istringstream hstr(split( elements[5], '=' )[1]); hstr >> g.h;
		  std::cout << "h: " << g.h  << ", ";
		  std::istringstream offstr(split( elements[8], '=' )[1]); offstr >> g.off;
		  std::cout << "off: " << g.off << std::endl;
		  glyphs.push_back(g);
	  }
    }
	file.close();

	
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

		sdfSprite.setScale( 6+3*sin(delta.getElapsedTime().asSeconds()), 6+3*sin(delta.getElapsedTime().asSeconds()) );
		sdfSprite.setRotation( delta.getElapsedTime().asSeconds()*45.0 );


        window.clear();
		window.draw(background);
        window.draw(sdfSprite, &sdfShader);
        window.display();
    }

    return 0;
}