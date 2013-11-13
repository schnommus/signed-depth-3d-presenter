#include "SDFont.h"

#include <algorithm>

SDFont::SDFont( std::string definitionFileName, std::string glyphsFileName ) {
	m_def_directory = definitionFileName;
	m_img_directory = glyphsFileName;

	BuildGlyphs();
}

SDFont::~SDFont() {
	for( std::map< char, Glyph* >::iterator it = m_glyphmap.begin(); it != m_glyphmap.end(); ++it ) {
		delete it->second;
	}
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

const Glyph &SDFont::GlyphForChar( char c ) {
	try {
		return *m_glyphmap[c];
	} catch (std::runtime_error &e) {
		std::cout << "Invalid glyph referenced!" << std::endl;
		return GlyphForChar('?');
	}
}

void SDFont::BuildGlyphs() {
	std::cout << "Caching font at " << m_def_directory << "... ";

	// Open image of all packed glyphs
	m_font_image.loadFromFile(m_img_directory);
	m_font_texture.loadFromImage(m_font_image);
	m_font_texture.setSmooth(true);

	// Text file with all character positions, offsets etc
	std::ifstream file;
	file.open(m_def_directory, std::ios::in );

	std::string line;
	std::istringstream ss;

	while( getline (file, line) ) {
      std::vector< std::string > elements = split(line, ' ');

	  // Lambda for stream for element after the '=' sign in text file at index from left of line
	  auto StreamForValueAt = [&] (int index) -> std::istringstream& { return ss.str(split( elements[index], '=' )[1]), ss.clear(), ss; };

	  if( elements[0] == "info" ) {
		  m_fontName = StreamForValueAt(1).str();
	  }

	  if( elements[0] == "char" ) {
		  Glyph &g = *new Glyph;

		  StreamForValueAt(1) >> g.char_id;
		  StreamForValueAt(2) >> g.image_x;
		  StreamForValueAt(3) >> g.image_y;
		  StreamForValueAt(4) >> g.char_size_x;
		  StreamForValueAt(5) >> g.char_size_y;
		  StreamForValueAt(6) >> g.x_offset;
		  StreamForValueAt(7) >> g.y_offset;
		  StreamForValueAt(8) >> g.x_advance;

		  // Calculate UV coordinates
		  float sz_x = m_font_texture.getSize().x, sz_y = m_font_texture.getSize().y;
		  g.u1 = (float)g.image_x / sz_x;
		  g.u2 = ((float)g.image_x + (float)g.char_size_x) / sz_x;
		  g.v1 = (float)g.image_y / sz_y;
		  g.v2 = ((float)g.image_y + (float)g.char_size_y) / sz_y;

#ifdef VERBOSE_GLYPH_GENERATION
		  std::cout << "id: " << g.char_id  << " \'" << (char)g.char_id << "\', ";
		  std::cout << "x: " << g.image_x << ", ";
		  std::cout << "y: " << g.image_y << ", ";
		  std::cout << "w: " << g.char_size_x << ", ";
		  std::cout << "h: " << g.char_size_y  << ", ";
		  std::cout << "xo: " << g.x_offset  << ", ";
		  std::cout << "yo: " << g.y_offset  << ", ";
		  std::cout << "off: " << g.x_advance << std::endl;
#endif

		  m_glyphmap[g.char_id] = &g;
	  }
    }
	file.close();

	std::cout << "Done. " << m_fontName << ", cached " << m_glyphmap.size() << " glyphs." << std::endl;
}