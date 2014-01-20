#pragma once

#include "Entity.h"
#include <SFGUI/SFGUI.hpp>

// Adds functionality to 2D entities required by UI
class UIEntity : public Entity2D {
public:
	// Used to prevent 3D selection behind UI
	bool IsMouseInsideWindow() { return m_mouseInside; }

protected:
	void SetupGUIUsageStates( sfg::Window::Ptr window) {
		window->GetSignal(sfg::Widget::OnMouseEnter).Connect( &UIEntity::MouseEnter, this );
		window->GetSignal(sfg::Widget::OnMouseLeave).Connect( &UIEntity::MouseLeave, this );
	}
	void MouseEnter() {m_mouseInside = true;}
	void MouseLeave() {m_mouseInside = false;}
	bool m_mouseInside;
};