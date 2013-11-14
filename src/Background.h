#pragma once

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Background : public Entity2D {
public:
	virtual void Initialize() {
		m_tile1.loadFromFile("../media/crinkle.jpg");
		m_tile1_spr.setTexture(m_tile1);
		m_tile1_spr.setColor( sf::Color(163+80, 74+80, 163+80, 100) );

		m_marquee.loadFromFile("../media/marquee.png");
		m_marquee.setSmooth(true);
		m_marquee_spr.setTexture(m_marquee);
		m_marquee_spr.setScale((float)m_app->GetSize().x/256.0, (float)m_app->GetSize().y/256.0);

		m_icons[0].loadFromFile("../media/icons_tribal/tribal1.png");
		m_icons[1].loadFromFile("../media/icons_tribal/tribal2.png");
		m_icons[2].loadFromFile("../media/icons_tribal/tribal3.png");
		m_icons[3].loadFromFile("../media/icons_tribal/tribal4.png");

		for( int i = 0; i != 20; ++i )
			m_icondata.push_back( sf::Vector3i(rand()%m_app->GetSize().x, rand()%m_app->GetSize().y, rand()%4) ); 

		m_iconSprite.setColor( sf::Color(255, 255, 255, 20) );
	}

	virtual void Draw() {
		for(int i = 0; i != (int)ceil( (float)m_app->GetSize().x / 512.0f ); ++i) {
			for(int j = 0; j != (int)ceil( (float)m_app->GetSize().y / 512.0f ); ++j) {
				m_tile1_spr.setPosition(512*i, 512*j);
				m_app->m_window.draw( m_tile1_spr );
			}
		}

		for( int i = 0; i != 20; ++i ) {
			m_iconSprite.setTexture( m_icons[m_icondata[i].z] );
			m_iconSprite.setPosition( m_icondata[i].x, m_icondata[i].y );
			m_iconSprite.setRotation( m_icondata[i].x%35 * 10 );
			m_app->m_window.draw(m_iconSprite);
		}

		m_app->m_window.draw(m_marquee_spr);
	}

	sf::Texture m_tile1;
	sf::Sprite m_tile1_spr;

	sf::Texture m_marquee;
	sf::Sprite m_marquee_spr;

	sf::Texture m_icons[4];
	sf::Sprite m_iconSprite;
	std::vector< sf::Vector3i > m_icondata;
};