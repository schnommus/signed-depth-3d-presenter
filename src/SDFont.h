#pragma once

#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

//#define VERBOSE_GLYPH_GENERATION

struct Glyph {
	int char_id;
	int image_x, image_y, char_size_x, char_size_y;
	float x_advance, x_offset, y_offset;
	float u1, u2, v1, v2;
};

class SDFont {
public:
	SDFont( std::string definitionFileName, std::string imageFileName );

	~SDFont();

	const Glyph& GlyphForChar( char c );

	const sf::Texture &Font_texture() const { return m_font_texture; }

private:
	void BuildGlyphs();

	std::string m_def_directory, m_img_directory, m_fontName;
	sf::Image m_font_image;
	sf::Texture m_font_texture;
	std::map< char, Glyph* > m_glyphmap;
};