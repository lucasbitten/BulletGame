#include "Game.h"


Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& config)
{
	std::ifstream myFile(config);

	if (myFile.fail())
	{
		std::cerr << "Could not load file from " << config << "!" << std::endl;
	}

	std::string configLine, configValue, configName;
	std::istringstream myStream;

	std::vector<int> values;
	int a;
	int frameRateLimit = 60;
	Vec2 dimensions = Vec2(800,600);

	while (std::getline(myFile, configLine))
	{
		myStream.clear();
		myStream.str(configLine);

		myStream >> configValue;
		std::getline(myStream, configName, ' ');
		if (configValue == "Window")
		{
			std::cout << "Window config: " << configLine << std::endl;

			int height, width;
			std::string configName;

			std::istringstream myStream;
			myStream.clear();
			myStream.str(configLine);

			while (myStream >> configName >> width >> height)
			{
				dimensions = Vec2(width, height);
			}
		}
		else if (configValue == "Font")
		{
			std::cout << "Font config: " << configLine << std::endl;

			std::string configName, fontPath;
			int fontSize, r, g, b;

			std::istringstream myStream;
			myStream.clear();
			myStream.str(configLine);

			while (myStream >> configName >> fontPath >> fontSize >> r >> g >> b)
			{

				// attempt to load the font from a file
				if (!m_font.loadFromFile(fontPath))
				{
					//if we can't load the font, print an error to the error console and exit;
					std::cerr << "Could not load font!" << std::endl;
					exit(-1);
				}

				m_fontColor = sf::Color(r, g, b);
				m_fontSize = fontSize;
			}

			m_text = new sf::Text("Score: 0", m_font, m_fontSize);
			m_text->setFillColor(m_fontColor);
		}
		else if (configValue == "Player")
		{
			std::cout << "Player config: " << configLine << std::endl;

			std::string configName, shapeName;
			float xPos, yPos, xSpeed, ySpeed, r, g, b, radius;

			std::istringstream myStream;
			myStream.clear();
			myStream.str(configLine);

			while (myStream >> configName >> m_playerConfig.ShapeRadius >> m_playerConfig.CollisionRadius >> 
				m_playerConfig.Speed 
				>> m_playerConfig.FillColorRed >> m_playerConfig.FillColorGreen >> m_playerConfig.FillColorBlue 
				>> m_playerConfig.OutlineColorRed >> m_playerConfig.OutlineColorGreen >> m_playerConfig.OutlineColorBlue 
				>> m_playerConfig.OutlineThickness >> m_playerConfig.Vertices)
			{


			}
		}
		else if (configValue == "Enemy")
		{
			std::cout << "Enemy config: " << configLine << std::endl;

			std::string configName, shapeName;
			float xPos, yPos, xSpeed, ySpeed, r, g, b, radius;

			std::istringstream myStream;
			myStream.clear();
			myStream.str(configLine);

			while (myStream >> configName >> m_enemyConfig.ShapeRadius >> m_enemyConfig.CollisionRadius >>
				m_enemyConfig.SpeedMin >> m_enemyConfig.SpeedMax
				>> m_enemyConfig.OutlineColorRed >> m_enemyConfig.OutlineColorGreen >> m_enemyConfig.OutlineColorBlue
				>> m_enemyConfig.OutlineThickness >> m_enemyConfig.VerticesMin >> m_enemyConfig.VerticesMax >> m_enemyConfig.SmallLifespan 
				>> m_enemyConfig.SpawnInterval
				)
			{


			}

		}
		else if (configValue == "Bullet")
		{
			std::cout << "Bullet config: " << configLine << std::endl;

			std::string configName, shapeName;
			float xPos, yPos, xSpeed, ySpeed, r, g, b, radius;

			std::istringstream myStream;
			myStream.clear();
			myStream.str(configLine);

			while (myStream >> configName >> m_bulletConfig.ShapeRadius >> m_bulletConfig.CollisionRadius >>
				m_bulletConfig.Speed
				>> m_bulletConfig.FillColorRed >> m_bulletConfig.FillColorGreen >> m_bulletConfig.FillColorBlue
				>> m_bulletConfig.OutlineColorRed >> m_bulletConfig.OutlineColorGreen >> m_bulletConfig.OutlineColorBlue
				>> m_bulletConfig.OutlineThickness >> m_bulletConfig.Vertices >> m_bulletConfig.Lifespan)
			{


			}
		}
	}

	m_window.create(sf::VideoMode(dimensions.x, dimensions.y), "Assignment 2");
	m_window.setFramerateLimit(frameRateLimit);

	spawnPlayer();	

}

void Game::run()
{
	//TODO: add pause functionallity here
	//		some systems should function while paused (rendering)
	//		some systems shouldn't (movement / input)

	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sEnemySpanwer();
			sMovement();
			sCollision();
			sLifespan();

		}

		sUserInput();
		sRender();

		// increment the current frame
		// may need to be moved when paused implemented
		if (!m_paused)
		{
			m_currentFrame++;
		}


	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}


// respawn the player in the middle of the screen
void Game::spawnPlayer()
{

	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	Vec2 dimensions(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);

	// Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform> (dimensions, Vec2(m_playerConfig.Speed, m_playerConfig.Speed), 0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(m_playerConfig.ShapeRadius, m_playerConfig.Vertices, 
		sf::Color(m_playerConfig.FillColorRed, m_playerConfig.FillColorGreen, m_playerConfig.FillColorBlue), 
		sf::Color(m_playerConfig.OutlineColorRed, m_playerConfig.OutlineColorGreen, m_playerConfig.OutlineColorBlue), 
		m_playerConfig.OutlineThickness);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CollisionRadius);

	// Since we want this Entity to be our playwe, set our Game's player variable to be this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
	m_player = entity;

}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");

	Vec2 dimensions(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);

	auto vertices = RandomRange(m_enemyConfig.VerticesMin, m_enemyConfig.VerticesMax);
	auto speedX = RandomRange(m_enemyConfig.SpeedMin, m_enemyConfig.SpeedMax);
	auto speedY = RandomRange(m_enemyConfig.SpeedMin, m_enemyConfig.SpeedMax);

	entity->cShape = std::make_shared<CShape>(m_enemyConfig.ShapeRadius, vertices,
		sf::Color(RandomRange(0,255), RandomRange(0, 255), RandomRange(0, 255)),
		sf::Color(m_enemyConfig.OutlineColorRed, m_enemyConfig.OutlineColorGreen, m_enemyConfig.OutlineColorBlue),
		m_enemyConfig.OutlineThickness);

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(RandomRange(m_enemyConfig.CollisionRadius, dimensions.x - m_enemyConfig.CollisionRadius), RandomRange(m_enemyConfig.CollisionRadius, dimensions.y - m_enemyConfig.CollisionRadius)),
		Vec2(speedX, speedY), 0.0f);
	
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CollisionRadius);
	entity->cScore = std::make_shared<CScore>(vertices*100);
	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes 
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	auto circle = e->cShape->circle;
	auto angle = (3.14f / 180) * 360.0f / circle.getPointCount();
	// TODO: spawn small enemies at the location of the input enemy e
	for (size_t i = 0; i < circle.getPointCount(); i++)
	{
		auto entity = m_entities.addEntity("smallEnemy");

		Vec2 dimensions(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);

		auto vertices = circle.getPointCount();
		auto speedX = RandomRange(m_enemyConfig.SpeedMin, m_enemyConfig.SpeedMax);
		auto speedY = RandomRange(m_enemyConfig.SpeedMin, m_enemyConfig.SpeedMax);

		entity->cShape = std::make_shared<CShape>(circle.getRadius() * 0.5f, vertices,
			sf::Color(circle.getFillColor()),
			sf::Color(circle.getOutlineColor()),
			circle.getOutlineThickness());

		Vec2 dir = Vec2(cos(angle * i), sin(angle * i));
		dir.normalize();

		entity->cTransform = std::make_shared<CTransform>(
			Vec2(e->cTransform->pos),
			Vec2(speedX * dir.x, speedY * dir.y), 0.0f);

		entity->cCollision = std::make_shared<CCollision>(e->cCollision->radius * 0.5f);
		entity->cScore = std::make_shared<CScore>(e->cScore->score * 2);
	}
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	if (m_player->cInput->shoot)
	{
		auto entity = m_entities.addEntity("bullet");
		Vec2 playerPos = m_player->cTransform->pos;
		Vec2 dir = target - playerPos;
		dir.normalize();
		Vec2 velocity = dir * m_bulletConfig.Speed;

		entity->cTransform = std::make_shared<CTransform>(playerPos, velocity, 0.0f);

		entity->cShape = std::make_shared<CShape>(m_bulletConfig.ShapeRadius, m_bulletConfig.Vertices,
			sf::Color(m_bulletConfig.FillColorRed, m_bulletConfig.FillColorGreen, m_bulletConfig.FillColorBlue),
			sf::Color(m_bulletConfig.OutlineColorRed, m_bulletConfig.OutlineColorGreen, m_bulletConfig.OutlineColorBlue),
			m_bulletConfig.OutlineThickness);

		entity->cCollision = std::make_shared<CCollision>(m_bulletConfig.CollisionRadius);

		entity->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.Lifespan);
	}
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	for (size_t i = 0; i < 10; i++)
	{

	}

}



void Game::sMovement()
{
	Vec2 dir;

	if (m_player->cInput->right)
	{
		dir.x = 1;
	}

	if (m_player->cInput->left)
	{
		dir.x = -1;
	}


	if (m_player->cInput->up)
	{
		dir.y = -1;
	}

	if (m_player->cInput->down)
	{
		dir.y = 1;
	}

	dir.normalize();
	auto windowSize = m_window.getSize();
	
	m_player->cTransform->velocity = dir * m_playerConfig.Speed;

	float radius = m_player->cShape->circle.getRadius();

	if (m_player->cTransform->pos.x > windowSize.x - radius && dir.x > 0)
	{
		m_player->cTransform->velocity.x = 0;
	}

	if (m_player->cTransform->pos.x < radius && dir.x < 0)
	{
		m_player->cTransform->velocity.x = 0;
	}


	if (m_player->cTransform->pos.y > windowSize.y - radius && dir.y > 0)
	{
		m_player->cTransform->velocity.y = 0;
	}

	if (m_player->cTransform->pos.y < radius && dir.y < 0)
	{
		m_player->cTransform->velocity.y = 0;
	}

	m_player->cTransform->pos += m_player->cTransform->velocity;

	updateEnemyPositionByTag("enemy");
	updateEnemyPositionByTag("smallEnemy");

	for (auto& e : m_entities.getEntities("bullet"))
	{
		e->cTransform->pos += e->cTransform->velocity;
	}

}

void Game::updateEnemyPositionByTag(const std::string& tag)
{
	auto windowSize = m_window.getSize();

	for (auto& e : m_entities.getEntities(tag))
	{
		e->cTransform->pos += e->cTransform->velocity;
		if (e->cTransform->pos.x > windowSize.x - e->cShape->circle.getRadius() || e->cTransform->pos.x < e->cShape->circle.getRadius())
		{
			e->cTransform->velocity.x = -e->cTransform->velocity.x;
		}

		if (e->cTransform->pos.y > windowSize.y - e->cShape->circle.getRadius() || e->cTransform->pos.y < e->cShape->circle.getRadius())
		{
			e->cTransform->velocity.y = -e->cTransform->velocity.y;
		}
	}
}




void Game::sLifespan()
{
	for (auto& e : m_entities.getEntities())
	{
		if (e->cLifespan == nullptr)
		{
			continue;
		}

		if (e->cLifespan->remaining > 0)
		{
			e->cLifespan->remaining--;
		}

		if (e->isActive())
		{
			auto fillColor = e->cShape->circle.getFillColor();
			auto outlineColor = e->cShape->circle.getOutlineColor();

			e->cShape->circle.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, 255 * static_cast<float> (e->cLifespan->remaining) / e->cLifespan->total));
			e->cShape->circle.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, 255 * static_cast<float> (e->cLifespan->remaining) / e->cLifespan->total));
		}

		if (e->cLifespan->remaining <= 0)
		{
			e->destroy();
		}
	}

}

void Game::sCollision()
{
	handleEnemyCollisionByTag("enemy", true);
	handleEnemyCollisionByTag("smallEnemy", false);

}

void Game::handleEnemyCollisionByTag(const std::string& tag, bool instantiateSmallEnemies)
{
	//todo: implement collision
	// be sure to use collision radius, not the shape radius

	for (auto& e : m_entities.getEntities(tag))
	{
		auto dist = e->cTransform->pos.distSq(m_player->cTransform->pos);
		auto radiusSum = m_player->cCollision->radius + e->cCollision->radius;
		if (dist <= radiusSum * radiusSum)
		{
			std::cout << "Player got hit" << std::endl;
			e->destroy();
			continue;
		}

		for (auto& bullet : m_entities.getEntities("bullet"))
		{
			dist = e->cTransform->pos.distSq(bullet->cTransform->pos);
			radiusSum = bullet->cCollision->radius + e->cCollision->radius;
			if (dist <= radiusSum * radiusSum)
			{
				std::cout << "Enemy got hit by bullet" << std::endl;

				m_score += e->cScore->score;
				m_text->setString("Score: " + std::to_string(m_score));
				if (instantiateSmallEnemies)
				{
					spawnSmallEnemies(e);
				}
				e->destroy();
				bullet->destroy();
				continue;

			}
		}
	}
}


void Game::sEnemySpanwer()
{
	// use m_currentFrame - m_lastEnemySpawnTime to determine
	// how long it has been since the last enemy spawned
	if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SpawnInterval)
	{
		spawnEnemy();
	}
}


void Game::sRender()
{
	//sample drawing of the player Entity that we have created
	m_window.clear();

	m_window.draw(*m_text);


	for (auto& e : m_entities.getEntities())
	{
		if (e->cShape && e->cTransform)
		{
			// set the position of the shape based on the entity's transform->pos
			e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

			// set the rotation of the shape based on the entity's transform->angle
			e->cTransform->angle += 1.0f;
			e->cShape->circle.setRotation(e->cTransform->angle);

			// draw the entity's sf::CircleShape 
			m_window.draw(e->cShape->circle);
		}
	}
	m_window.display();
}

void Game::sUserInput()
{
	//		 note that you should only be setting the player's input component variables here
	//		 you should not implement the player's movement logic here
	//		 the movement system will read the variables you set in this function

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// this event triggers when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (!m_paused)
		{
			// this event is triggered when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::W:

					m_player->cInput->up = true;
					break;
				case sf::Keyboard::S:

					m_player->cInput->down = true;
					break;
				case sf::Keyboard::A:

					m_player->cInput->left = true;
					break;
				case sf::Keyboard::D:
					m_player->cInput->right = true;
					break;
				default:
					break;
				}
			}

			// this event is triggered when a key is released
			if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::W:

					m_player->cInput->up = false;
					break;
				case sf::Keyboard::S:

					m_player->cInput->down = false;
					break;
				case sf::Keyboard::A:

					m_player->cInput->left = false;
					break;
				case sf::Keyboard::D:

					m_player->cInput->right = false;
					break;
				default:
					break;
				}
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				switch (event.key.code)
				{
				case sf::Mouse::Left:
					m_player->cInput->shoot = true;
					spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));

					break;
				case sf::Mouse::Right:
					spawnSpecialWeapon(m_player);
					break;
				default:
					break;
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				switch (event.key.code)
				{
				case sf::Mouse::Left:
					m_player->cInput->shoot = false;

					break;
				default:
					break;
				}
			}
		}

		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::E:
				spawnEnemy();
				break;
			case sf::Keyboard::P:
				setPaused(!m_paused);
				break;
			case sf::Keyboard::Escape:
				exit(0);
				break;

			default:
				break;
			}
		}
	}
}

