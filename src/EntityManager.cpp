#include "EntityManager.h"

#include <SFML/OpenGL.hpp>
#include <iostream>


// All Id Dispensor functions are dumb at the moment
// Awaiting proper implementation
IdDispensor::IdDispensor() 
	: m_currentId(0) {}

unsigned int IdDispensor::GetNewId() {
	return ++m_currentId;
}

void IdDispensor::SetIdUtilized( unsigned int val ) {
	if( val > m_currentId ) m_currentId = val + 1;
}

void IdDispensor::RemoveId( unsigned int val ) {
	// Pop from list or something
}

EntityManager::EntityManager( Application *app )
	: m_app(app) { }

EntityManager::~EntityManager() {
	for( EntityMap::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
		delete it->second;
	}
}

Entity *EntityManager::AddEntity( Entity *ent ) {
	ent->SetId( m_idDispensor.GetNewId() );
	ent->SetApp( m_app );
	m_entities[ent->Id()] = ent;
	ent->Initialize();
	return ent;
}

void EntityManager::RemoveEntity( Entity *ent ) {
	ent->Destroy();
	m_id_removal_list.push_back(ent->Id());
}

void EntityManager::UpdateEntities( float delta ) {
	for( EntityMap::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
		it->second->Update( delta );
	}
}

void EntityManager::DrawEntities() {
	for( EntityMap::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
		if( it->second->Is3D() )
			glLoadName( it->second->Id() );
		it->second->Draw();
	}
}

void EntityManager::DoDeletions() {
	for ( std::vector<unsigned int>::iterator i = m_id_removal_list.begin(); i != m_id_removal_list.end(); ++i ) {
		if( m_entities.find( *i ) != m_entities.end() ) {
			delete m_entities[*i];
			m_entities.erase(*i);
		} else {
			std::cout << "Attempted to delete entity with invalid ID!" << std::endl;
		}
	}
	m_id_removal_list.clear();
}

Entity *EntityManager::GetEntityWithId(unsigned int id ) {
	if( m_entities.find( id ) != m_entities.end() ) {
		return m_entities[id];
	}
	std::cout << "Attempted to get entity with invalid ID!" << std::endl;
	return nullptr;
}

void EntityManager::RemoveEntityById( unsigned int id ) {
	GetEntityWithId(id)->Destroy();
	m_id_removal_list.push_back(id);
}

void EntityManager::RemoveAllEntities() {
	for( EntityMap::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
		RemoveEntity( it->second );
	}
}

void EntityManager::HandleSFMLEvent( sf::Event event ) {
	for( EntityMap::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
		it->second->HandleSFMLEvent( event );
	}
}

