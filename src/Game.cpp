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
			//parse Enemy cfg
		}
		else if (configValue == "Bullet")
		{
			//parse Bullet cfg
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
			sUserInput();
		}


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
	// TODO: Finish adding all properties of the player with the correct values from the config
	
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

	// Since we want this Entity to be our playwe, set our Game's player variable to be this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
	m_player = entity;

}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
	// TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
	//		the enemy must be spawned completely within the bounds of the window
	
	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes 
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	// TODO: spawn small enemies at the location of the input enemy e
	// 
	// when we create the smaller enemy, we have to read the values of the original enemy
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - small enem*/ies are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
		// TODO: implement the spawning of a bullet which travels toward target 
		//		- bullet speed is given as a scalar speed
		//		- you must set the velocity by using formula in notes

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	//TODO: implement your own special weapon
}



void Game::sMovement()
{
	// TODO: implement all entity movement in this function
	//		 you should read the m_player->cInput component to determine if the player is moving

	// implement player movement

	// sample movement speed update
	//m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	//m_player->cTransform->pos.y += m_player->cTransform->velocity.y;


}

void Game::sLifespan()
{
	// TODO: implement all lifespan functionality
	//
	// for all entities
	//		if entity has no lifespan component, skip it
	//		if entity has > 0 remaining lifespan, subtract 1
	//		if it has lifespan and is alive
	//			scale its alpha channel properly
	//		if it has lifespan and its time is up
	//			destroy entity

}

void Game::sCollision()
{
	//todo: implement collision
	// be sure to use collision radius, not the shape radius

}

void Game::sEnemySpanwer()
{
	//todo: implement enemy spawning
	// use m_currentFrame - m_lastEnemySpawnTime to determine
	// how long it has been since the last enemy spawned

}


void Game::sRender()
{
	// TODO: change the code below to draw ALL of the entities
	//sample drawing of the player Entity that we have created
	m_window.clear();

	for (auto& e : m_entities.getEntities())
	{
		// set the position of the shape based on the entity's transform->pos
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		// set the rotation of the shape based on the entity's transform->angle
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		// draw the entity's sf::CircleShape 
		m_window.draw(e->cShape->circle);

	}
	m_window.display();
}

void Game::sUserInput()
{
	// TODO: handle user input here
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
}

