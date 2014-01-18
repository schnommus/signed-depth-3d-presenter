#pragma once

#include "../UIEntity.h"
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include "../Application.h"
#include <boost/lexical_cast.hpp>

class UI_Taskbar : public UIEntity {
public:
	virtual void Initialize() {
		m_window = sfg::Window::Create();
		m_window->SetTitle( "Taskbar" );

		m_window->GetSignal(sfg::Widget::OnMouseEnter).Connect( &UI_Taskbar::MouseEnter, this );
		m_window->GetSignal(sfg::Widget::OnMouseLeave).Connect( &UI_Taskbar::MouseLeave, this );

		m_app->m_desktop.Add( m_window );

		sfg::Box::Ptr master_box = sfg::Box::Create( sfg::Box::HORIZONTAL );

		// NAVIGATION
		m_previousKeyframeButton = sfg::Button::Create();
		m_nextKeyframeButton = sfg::Button::Create();

		m_previousKeyframeButton->SetLabel("<--");
		m_nextKeyframeButton->SetLabel("-->");

		m_previousKeyframeButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &Application::PreviousKeyframe, m_app);
		m_nextKeyframeButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &Application::NextKeyframe, m_app);

		sfg::Frame::Ptr nav_frame = sfg::Frame::Create("Keyframe Navigation");
		sfg::Box::Ptr nav_box = sfg::Box::Create( sfg::Box::HORIZONTAL );

		nav_box->Pack( m_previousKeyframeButton );
		nav_box->Pack( m_nextKeyframeButton );

		m_keyframeDetails = sfg::Label::Create("(frame 3 of 12)");
		nav_box->Pack(m_keyframeDetails);
		
		nav_box->SetSpacing( 5.f );
		nav_frame->Add( nav_box );
		master_box->Pack(nav_frame);

		// SLIDE CREATION
		sfg::Frame::Ptr create_keyframe_frame = sfg::Frame::Create("Keyframe Creation");
		sfg::Box::Ptr create_keyframe_box = sfg::Box::Create( sfg::Box::HORIZONTAL );
		m_createKeyframeButton = sfg::Button::Create();
		m_createKeyframeButton->SetLabel("Create Keyframe (at end)");
		m_createKeyframeButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &Application::CreateKeyframe, m_app);
		m_deleteKeyframeButton = sfg::Button::Create();
		m_deleteKeyframeButton->SetLabel("Delete Keyframe (current)");
		m_deleteKeyframeButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &Application::DeleteCurrentKeyframe, m_app);
		create_keyframe_box->Pack( m_createKeyframeButton );
		create_keyframe_box->Pack( m_deleteKeyframeButton );

		create_keyframe_box->SetSpacing( 5.f );

		create_keyframe_frame->Add(create_keyframe_box);
		master_box->Pack(create_keyframe_frame);

		master_box->SetSpacing( 5.f );

		m_window->Add( master_box );
	}

	virtual void Update( float delta ) {
		std::string navtext = 
			std::string("(Keyframe ") +
			boost::lexical_cast<std::string>( m_app->m_firstPersonCamera.m_currentKeyframe + 1) +
			" of " +
			boost::lexical_cast<std::string>( m_app->m_firstPersonCamera.m_keyframes.size() ) +
			")";
		m_keyframeDetails->SetText(navtext);

		if( m_app->m_firstPersonCamera.m_keyframes.size() == 0 ) {
			m_keyframeDetails->SetText("No Keyframes");
		}
	}

protected:
	void MouseEnter() {m_mouseInside = true;}
	void MouseLeave() {m_mouseInside = false;}

	sfg::Window::Ptr m_window;
	sfg::Button::Ptr m_previousKeyframeButton,
					 m_nextKeyframeButton,
					 m_createKeyframeButton,
					 m_deleteKeyframeButton;

	sfg::Label::Ptr m_keyframeDetails;
};