#pragma once

#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//#define VERBOSE_GLYPH_GENERATION

struct glyph {
	int id;
	int x, y, w, h;
	float off, xo, yo;
	sf::Image img;
};

class SDFontReader {
public:
	SDFontReader( std::string definitionFileName, std::string glyphsFileName );
	~SDFontReader();

	sf::Image ImageForString( std::string s );

private:
	void BuildGlyphs();
	const glyph& GlyphForCharacter( char c );

	std::string definitionsDirectory, glyphsDirectory, fontName;

	std::vector< glyph* > glyphs;
};