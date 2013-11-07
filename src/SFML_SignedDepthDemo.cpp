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
	float off, xo, yo;
	sf::Texture tex;
};

const glyph &glyphForCharacter( const std::vector< glyph* > &glyphs, char c ) {
	for( int i = 0; i != glyphs.size(); ++i ) {
		if( (char)glyphs[i]->id == c )
			return *glyphs[i];
	}
	std::cout << "Invalid glyph referenced!" << std::endl;
	return glyph();
}

int main()
{
	std::ifstream file;
	file.open("../media/sdf1.txt", std::ios::in );
	std::string line;

	sf::Image fontImage; fontImage.loadFromFile("../media/sdf1.png");

	std::vector< glyph* > glyphs;

	while( getline (file, line) ) {
      std::vector< std::string > elements = split(line, ' ');
	  if( elements[0] == "char" ) {
		  glyph *gp = new glyph;
		  glyph &g = *gp;
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
		  std::istringstream xostr(split( elements[6], '=' )[1]); xostr >> g.xo;
		  std::cout << "xo: " << g.xo  << ", ";
		  std::istringstream yostr(split( elements[7], '=' )[1]); yostr >> g.yo;
		  std::cout << "yo: " << g.yo  << ", ";
		  std::istringstream offstr(split( elements[8], '=' )[1]); offstr >> g.off;
		  std::cout << "off: " << g.off << std::endl;
		  g.tex.loadFromImage( fontImage, sf::IntRect(g.x, g.y, g.w, g.h) );
		  g.tex.setSmooth(true);
		  glyphs.push_back(&g);
	  }
    }
	file.close();

	std::string testString("Hello, SDF fonts in SFML! :D");

	std::vector< sf::Sprite > spriteString;

	float off = 0.0f;
	for( int i = 0; i != testString.size(); ++i ) {
		const glyph &g = glyphForCharacter( glyphs, testString[i] );
		sf::Sprite s( g.tex );
		s.setPosition(off+g.xo, 300-g.yo);
		spriteString.push_back( s );
		off += g.off;
	}
	
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_SignedDistance", sf::Style::Fullscreen);
    

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


        window.clear();
		window.draw(background);

		for( int i = 0; i != spriteString.size(); ++i ) {
			spriteString[i].setScale( 1.0+0.1*sin(delta.getElapsedTime().asSeconds()+(float)i/4.0), 1.0+0.1*sin(delta.getElapsedTime().asSeconds()+(float)i/4.0) );
			window.draw(spriteString[i], &sdfShader);
		}

        window.display();
    }

    return 0;
}