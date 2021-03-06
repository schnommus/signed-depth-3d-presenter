#pragma once

#include "Entity.h"
#include <map>
#include <vector>
#include <SFML/System.hpp>

class Application;

class IdDispensor {
public:
	IdDispensor();

	unsigned int GetNewId();

	void SetIdUtilized( unsigned int val );
	void RemoveId( unsigned int val );
private:
	unsigned int m_currentId;
};

class EntityManager {
public:
	EntityManager(Application *app);
	virtual ~EntityManager();

	void HandleSFMLEvent( sf::Event event );

	Entity *AddEntity( Entity *ent );

	void RemoveEntity( Entity *ent );

	void RemoveAllEntities();

	void UpdateEntities( float delta );

	void DrawEntities();

	void DoDeletions();

	Entity *GetEntityWithId( unsigned int id );

	void RemoveEntityById ( unsigned int id );

protected:
	Application *m_app;

	IdDispensor m_idDispensor;

	typedef std::map< unsigned int, Entity* > EntityMap;

	EntityMap m_entities;

	std::vector< unsigned int > m_id_removal_list;
};