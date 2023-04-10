#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Entity.h"
#include <map>

typedef std::vector <std::shared_ptr<Entity>> EntityVec;
typedef std::map	<std::string, EntityVec> EntityMap;

class EntityManager 
{
public:
	EntityManager();
	~EntityManager();

	void update();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);

private:

	EntityVec	m_entitiesToAdd;
	EntityVec	m_entities;
	EntityMap	m_entityMap;
	size_t		m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);

};




