#pragma once
#include "Common.h"
#include "Components.h"

class Entity
{
	friend class EntityManager;

public:

	// component pointers
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespan> cLifespan;


	bool isActive() const;
	const std::string tag() const;
	const size_t id() const;
	void destroy();

private:

	Entity(size_t id, const std::string& tag);


	bool				m_active = true;
	const size_t		m_id = 0;
	const std::string	m_tag = "Default";
};

