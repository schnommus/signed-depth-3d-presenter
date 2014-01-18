#pragma once

#include "EntityManager.h"
#include "UIEntity.h"

class UIEntityManager : public EntityManager {
public:
	UIEntityManager::UIEntityManager( Application *app )
		: EntityManager(app) { }

	bool GUI_InUse() {
		for( EntityMap::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
			if(dynamic_cast<UIEntity*>(it->second)->m_mouseInside) return true;
		}
		return false;
	}
};