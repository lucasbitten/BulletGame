#include "EntityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

void EntityManager::update()
{
	// - add them to the vector of all entities
	// 	-add them to the vector inside the map, with the tag as a key m_entitiesToAdd)
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->m_tag].push_back(e);
	}

	m_entitiesToAdd.clear();

	// remove dead entities from the vector of all entities 
	removeDeadEntities(m_entities);


	// remove dead entities from each vector in the entity map
	// C++17 way of iterating through [key, value] pairs in a map 
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{

	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	
	m_entitiesToAdd.push_back(entity);

	return entity;
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];

}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	std::erase_if(vec, [](auto& e) {return !e->isActive(); });
}
