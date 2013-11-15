#pragma once

#include "../Entity.h"
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include "../Application.h"

class UI_CameraInfo : public Entity2D {
public:
	virtual void Initialize() {
		m_label = sfg::Label::Create("Heya world!");
		m_button = sfg::Button::Create("SFGUI Greetin'..");

		m_box = sfg::Box::Create( sfg::Box::VERTICAL, 5.0f );
		m_box->Pack( m_label );
		m_box->Pack( m_button, false );

		m_window = sfg::Window::Create();
		m_window->SetTitle( "Hello world!" );
		m_window->Add( m_box );

		m_app->m_desktop.Add( m_window );
	}

protected:
	sfg::Label::Ptr m_label;
	sfg::Button::Ptr m_button;
	sfg::Box::Ptr m_box;
	sfg::Window::Ptr m_window;
};