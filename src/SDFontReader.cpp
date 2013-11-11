#include "SDFontReader.h"

#include <algorithm>

SDFontReader::SDFontReader( std::string definitionFileName, std::string glyphsFileName ) {
	definitionsDirectory = definitionFileName;
	glyphsDirectory = glyphsFileName;

	BuildGlyphs();
}

SDFontReader::~SDFontReader() {
	for( int i = 0; i != glyphs.size(); ++i ) { delete glyphs[i]; }
}

std::vector<std::string> split(std::string &s, char delim) {
	std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
		if(!item.empty())
			elems.push_back(item);
    }
    return elems;
}

const glyph &SDFontReader::GlyphForCharacter( char c ) {
	for( int i = 0; i != glyphs.size(); ++i ) {
		if( (char)glyphs[i]->id == c )
			return *glyphs[i];
	}
	std::cout << "Invalid glyph referenced!" << std::endl;
	return glyph();
}

sf::Image SDFontReader::ImageForString( std::string s ) {
	const int Y_BUFFER = 33; //For some reason, all characters are offset in the textfile. Implicit linespacing?

	float size_x = 0.0f;
	float size_y = 0.0f;
	for( int i = 0; i != s.size(); ++i ) {
		const glyph &g = GlyphForCharacter( s[i] );
		size_x += g.off;
		size_y = std::max(size_y, g.h+g.yo-Y_BUFFER);
	}
	
	sf::Image stringImage;
	stringImage.create( int(ceil(size_x)), int(ceil(size_y)), sf::Color(0, 0, 0, 0) );

	float off_x = 0.0f;
	for( int i = 0; i != s.size(); ++i ) {
		const glyph &g = GlyphForCharacter( s[i] );
		stringImage.copy( g.img, off_x+g.xo, ceil(size_y)-g.yo-Y_BUFFER, sf::IntRect(0, 0, g.w, g.h) );
		off_x += g.off;
	}

	return stringImage;
}

void SDFontReader::BuildGlyphs() {
	std::cout << "Caching font at " << definitionsDirectory << "... ";

	// Open image of all packed glyphs
	sf::Image fontImage;
	fontImage.loadFromFile(glyphsDirectory);

	// Textfile with all character positions, offsets etc
	std::ifstream file;
	file.open(definitionsDirectory, std::ios::in );

	std::string line;
	std::istringstream ss;

	while( getline (file, line) ) {
      std::vector< std::string > elements = split(line, ' ');

	  // Lambda for stream for element after the '=' sign in textfile at index from left of line
	  auto StreamForValueAt = [&] (int index) -> std::istringstream& { return ss.str(split( elements[index], '=' )[1]), ss.clear(), ss; };

	  if( elements[0] == "info" ) {
		  fontName = StreamForValueAt(1).str();
	  }

	  if( elements[0] == "char" ) {
		  glyph &g = *new glyph;

		  StreamForValueAt(1) >> g.id;
		  StreamForValueAt(2) >> g.x;
		  StreamForValueAt(3) >> g.y;
		  StreamForValueAt(4) >> g.w;
		  StreamForValueAt(5) >> g.h;
		  StreamForValueAt(6) >> g.xo;
		  StreamForValueAt(7) >> g.yo;
		  StreamForValueAt(8) >> g.off;

#ifdef VERBOSE_GLYPH_GENERATION
		  std::cout << "id: " << g.id  << " \'" << (char)g.id << "\', ";
		  std::cout << "x: " << g.x << ", ";
		  std::cout << "y: " << g.y << ", ";
		  std::cout << "w: " << g.w << ", ";
		  std::cout << "h: " << g.h  << ", ";
		  std::cout << "xo: " << g.xo  << ", ";
		  std::cout << "yo: " << g.yo  << ", ";
		  std::cout << "off: " << g.off << std::endl;
#endif

		  g.img.create(g.w, g.h);
		  g.img.copy(fontImage, 0, 0, sf::IntRect(g.x, g.y, g.w, g.h) );
		  g.tex.loadFromImage(g.img);

		  glyphs.push_back(&g);
	  }
    }
	file.close();

	std::cout << "Done. " << fontName << ", unpacked " << glyphs.size() << " glyphs." << std::endl;
}