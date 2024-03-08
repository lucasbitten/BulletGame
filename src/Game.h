#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Components.h"

struct PlayerConfig {
	int ShapeRadius;
	int CollisionRadius;
	int FillColorRed;
	int FillColorGreen;
	int FillColorBlue;
	int OutlineColorRed;
	int OutlineColorGreen;
	int OutlineColorBlue;
	int OutlineThickness;
	int Vertices;
	float Speed;
};

struct EnemyConfig {
	int ShapeRadius;
	int CollisionRadius;
	int OutlineColorRed;
	int OutlineColorGreen;
	int OutlineColorBlue;
	int OutlineThickness;
	int VerticesMax;
	int VerticesMin;
	int SmallLifespan;
	int SpawnInterval;
	float SpeedMax;
	float SpeedMin;
};

struct BulletConfig {
	int ShapeRadius;
	int CollisionRadius;
	int FillColorRed;
	int FillColorGreen;
	int FillColorBlue;
	int OutlineColorRed;
	int OutlineColorGreen;
	int OutlineColorBlue;
	int OutlineThickness;
	int Vertices;
	int Lifespan; 
	float Speed;
};


class Game
{
private:

	sf::RenderWindow		m_window;					// the window we will draw to
	EntityManager			m_entities;					// vector of entities to maintain
	sf::Font				m_font;						// the font we will use to draw
	sf::Text*				m_text;						// the score text to be drawn to the screen
	PlayerConfig			m_playerConfig;
	EnemyConfig				m_enemyConfig;
	BulletConfig			m_bulletConfig;
	int						m_score = 0;
	int						m_currentFrame = 0;
	int						m_lastEnemySpawnTime = 0;
	bool					m_paused = false;			// whether we update game logic
	bool					m_running = true;			// whether the game is running
	sf::Color				m_fontColor;
	int						m_fontSize;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config);	//initialize the GameState with a config file patyh
	void setPaused(bool paused);			//pause the game

	void sMovement();						//System: Entity position / movement update
	void sUserInput();						//System: User Input
	void sLifespan();						//System: Lifespan
	void sRender();							//System: Render / Drawing
	void sEnemySpanwer();					//System: Spawns Enemies
	void sCollision();						//System: Collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& moousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
	void updateEnemyPositionByTag(const std::string& tag);
	void handleEnemyCollisionByTag(const std::string& tag, bool instantiateSmallEnemies);


public:

	Game(const std::string& config);  //constructor, takes in game config
	void run();

};