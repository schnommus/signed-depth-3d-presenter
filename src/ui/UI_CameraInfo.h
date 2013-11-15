#pragma once

#include "../Entity.h"
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include "../Application.h"
#include <boost/lexical_cast.hpp>

struct PropertyReference {
public:
	std::string m_type;
	boost::any m_target;
	sfg::Widget::Ptr m_widget;
};

class UI_CameraInfo : public Entity2D {
public:
	virtual void Initialize() {
		m_lastid = -1;

		m_window = sfg::Window::Create();
		m_window->SetTitle( "Entity Selector" );

		m_app->m_desktop.Add( m_window );
	}

	virtual void Update( float delta ) {
		if(m_app->SelectedEntityId() != m_lastid) {
			CreateForm();
		}
		m_lastid = m_app->SelectedEntityId();

		if(m_app->SelectedEntityId() != 0) {
			for( int i = 0; i != m_propertyReferences.size(); ++i ) {
				if( m_propertyReferences[i].m_type == "string" ) {
					sfg::Entry *field = dynamic_cast<sfg::Entry*>(m_propertyReferences[i].m_widget.get());
					*boost::any_cast<std::string*>(m_propertyReferences[i].m_target) = field->GetText();
				} else if ( m_propertyReferences[i].m_type == "float" ) {
					sfg::SpinButton *spin = dynamic_cast<sfg::SpinButton*>(m_propertyReferences[i].m_widget.get());
					*boost::any_cast<float*>(m_propertyReferences[i].m_target) = spin->GetValue();
				} else if ( m_propertyReferences[i].m_type == "int" ) {
					sfg::SpinButton *spin = dynamic_cast<sfg::SpinButton*>(m_propertyReferences[i].m_widget.get());
					*boost::any_cast<int*>(m_propertyReferences[i].m_target) = (int)spin->GetValue();
				}
			}
			m_app->m_entitymanager.GetEntityWithId(m_app->SelectedEntityId())->PropertyAltered();
		}
	}

	void CreateForm() {
		m_propertyReferences.clear();
		m_window->RemoveAll();

		m_label = sfg::Label::Create("Heyo");

		m_box = sfg::Box::Create( sfg::Box::VERTICAL, 5.0f );
		m_box->Pack( m_label );

		if( m_app->SelectedEntityId() != 0 ) {
			m_label->SetText( std::string("ID: ") + boost::lexical_cast<std::string>( m_app->SelectedEntityId() ));

			Entity *e = m_app->m_entitymanager.GetEntityWithId(m_app->SelectedEntityId());

			sfg::Label::Ptr typeLabel = sfg::Label::Create( std::string("Type: ") + std::string(typeid(*e).name()) );
			m_box->Pack( typeLabel );

			for(int i = 0; i != e->NamedProperties().size(); ++i) {
				NamedProperty &property = e->NamedProperties()[i];

				sfg::Box::Ptr propertyBox = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
				propertyBox->Pack(sfg::Label::Create(property.m_name), false, false);

				// Is it a string property? Entry widget.
				try{
					std::string *s = boost::any_cast<std::string*>(property.m_value);
					sfg::Entry::Ptr entry = sfg::Entry::Create( *s );
					PropertyReference pr = {"string", property.m_value, entry };
					m_propertyReferences.push_back(pr);
					propertyBox->Pack(entry);
				} catch (boost::bad_any_cast &e ) {}

				// Is it a float property? Spinbox widget.
				try {
					float *f = boost::any_cast<float*>(property.m_value);
					sfg::SpinButton::Ptr spin = sfg::SpinButton::Create(-500, 500, 1);
					spin->SetValue( *f );
					spin->SetRequisition( sf::Vector2f( 80.f, 0.f ) );
					spin->SetDigits( 2 );
					PropertyReference pr = {"float", property.m_value, spin };
					m_propertyReferences.push_back(pr);
					propertyBox->Pack(spin);
				} catch (boost::bad_any_cast &e ) {}

				// Is it an int property? Spinbox widget.
				try {
					int *f = boost::any_cast<int*>(property.m_value);
					sfg::SpinButton::Ptr spin = sfg::SpinButton::Create(-500, 500, 1);
					spin->SetValue( *f );
					spin->SetRequisition( sf::Vector2f( 80.f, 0.f ) );
					spin->SetDigits( 2 );
					PropertyReference pr = {"int", property.m_value, spin };
					m_propertyReferences.push_back(pr);
					propertyBox->Pack(spin);
				} catch (boost::bad_any_cast &e ) {}

				m_box->Pack(propertyBox);
			}
		} else {
			m_label->SetText("No entity selected.");
		}

		m_window->Add( m_box );
	}

protected:
	sfg::Label::Ptr m_label;
	sfg::Box::Ptr m_box;
	sfg::Window::Ptr m_window;
	unsigned int m_lastid;

	std::vector<PropertyReference> m_propertyReferences;
};