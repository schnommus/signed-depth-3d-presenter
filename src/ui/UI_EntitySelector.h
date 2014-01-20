#pragma once

#include "../UIEntity.h"
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include "../Application.h"
#include <boost/lexical_cast.hpp>

// Property type, widget and entity property pointed to
struct PropertyReference {
public:
	std::string m_type;
	boost::any m_target;
	sfg::Widget::Ptr m_widget;
};

class UI_EntitySelector : public UIEntity {
public:
	virtual void Initialize() {
		m_lastid = -1; // Signifies last entity selected = nonexistant

		m_window = sfg::Window::Create();
		m_window->SetTitle( "Entity Selector" );
		m_window->SetPosition( sf::Vector2f(30, 220) );
		
		SetupGUIUsageStates( m_window );

		m_app->m_desktop.Add( m_window );
	}


	virtual void Update( float delta ) {

		// Is there an entity selected? Go through every widget, update entities' property
		if( !m_app->SelectedEntityIds().empty() ) { 

			// New entity? Recreate property form
			if(m_app->SelectedEntityIds()[0] != m_lastid) {
				CreateForm(); 
			}

			// Avoid recreating property form every frame!
			m_lastid = m_app->SelectedEntityIds()[0];

			for( int i = 0; i != m_propertyReferences.size(); ++i ) {
				PropertyReference &currentProperty = m_propertyReferences[i];
				if( currentProperty.m_type == "string" ) {
					sfg::Entry *field = dynamic_cast<sfg::Entry*>(currentProperty.m_widget.get());
					*boost::any_cast<std::string*>(currentProperty.m_target) = field->GetText();
				} else if ( currentProperty.m_type == "float" ) {
					sfg::SpinButton *spin = dynamic_cast<sfg::SpinButton*>(currentProperty.m_widget.get());
					*boost::any_cast<float*>(currentProperty.m_target) = spin->GetValue();
				} else if ( currentProperty.m_type == "int" ) {
					sfg::SpinButton *spin = dynamic_cast<sfg::SpinButton*>(currentProperty.m_widget.get());
					*boost::any_cast<unsigned char*>(currentProperty.m_target) = (unsigned char)spin->GetValue();
				}
			}

			// Entity knows to re-read properties (change vertex colours etc) when this is set
			m_app->m_entitymanager.GetEntityWithId(m_app->SelectedEntityIds()[0])->PropertyAltered();
		}
	}

	void CreateForm() {

		// Clean slate
		m_propertyReferences.clear();
		m_window->RemoveAll();

		// Box to store all properties (vertically, in rows of horiz. boxes)
		m_propertylist_box = sfg::Box::Create( sfg::Box::VERTICAL, 5.0f );

		if( !m_app->SelectedEntityIds().empty() ) {

			Entity *e = m_app->m_entitymanager.GetEntityWithId(m_app->SelectedEntityIds()[0]);

			// Labels for entity ID, entity type
			m_propertylist_box->Pack( sfg::Label::Create( std::string("ID: ") + boost::lexical_cast<std::string>( m_app->SelectedEntityIds()[0] ) ) );
			m_propertylist_box->Pack( sfg::Label::Create( std::string("Type: ") + std::string(typeid(*e).name()) ) );

			// Now, add widgets for each property
			for(int i = 0; i != e->NamedProperties().size(); ++i) {
				NamedProperty &property = e->NamedProperties()[i];

				// Horiz. box for a property. Label on left, then widget on right.
				sfg::Box::Ptr property_box = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );

				// Label for left
				property_box->Pack(sfg::Label::Create(property.m_name), false, false);

				// Then determine widget type, create.

				// ATM selection by boost::any cast exception: probably not very fast but
				// not hugely fussed. Bit hacky, works, & form creation ain't realtime.

				// Is it a string property? Entry widget.
				try{
					std::string *s = boost::any_cast<std::string*>(property.m_value);
					sfg::Entry::Ptr entry = sfg::Entry::Create( *s );
					PropertyReference pr = {"string", property.m_value, entry };
					m_propertyReferences.push_back(pr);
					property_box->Pack(entry);
				} catch (boost::bad_any_cast &e ) {}

				// Is it a float property? Spinbox widget.
				try {
					float *f = boost::any_cast<float*>(property.m_value);
					sfg::SpinButton::Ptr spin = sfg::SpinButton::Create(-500, 500, 0.01);
					spin->SetValue( *f );
					spin->SetRequisition( sf::Vector2f( 80.f, 0.f ) );
					spin->SetDigits( 2 );
					PropertyReference pr = {"float", property.m_value, spin };
					m_propertyReferences.push_back(pr);
					property_box->Pack(spin);
				} catch (boost::bad_any_cast &e ) {}

				// Is it an int property? Spinbox widget.
				try {
					unsigned char *f = boost::any_cast<unsigned char*>(property.m_value);
					float fv = (float) *f;
					sfg::SpinButton::Ptr spin = sfg::SpinButton::Create(-500, 500, 1);
					spin->SetValue( fv );
					spin->SetRequisition( sf::Vector2f( 80.f, 0.f ) );
					spin->SetDigits( 2 );
					PropertyReference pr = {"int", property.m_value, spin };
					m_propertyReferences.push_back(pr);
					property_box->Pack(spin);
				} catch (boost::bad_any_cast &e ) {}

				// Add the horizontal property to the vertical list of rows
				m_propertylist_box->Pack(property_box);
			}
		} else {
			m_propertylist_box->Pack( sfg::Label::Create( "No entity selected." ) );
		}

		// Finally, add the finished box to the window.
		m_window->Add( m_propertylist_box );
	}

protected:

	sfg::Box::Ptr m_propertylist_box;
	sfg::Window::Ptr m_window;
	unsigned int m_lastid;

	std::vector<PropertyReference> m_propertyReferences;
};