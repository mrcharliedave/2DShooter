#pragma once
#include "Visualisation.h"
#include "Input.h"
#include "WorldEntity.h"
#include <iostream>
using namespace HAPISPACE;

class World
{
public:
	World();
	~World();

	void Initialise();
	void Run();
	void Draw();
	void CheckCollisions();

	void LoadAllTextures();
	void LoadSounds();

	void EnemySpawn();

private:

	Visualisation vis_;
	Input input_;
	PlayerEntity player_;

	std::map<int, EnemyEntity*> enemies_;

	int score_ = 0;
	int spawnCount_{ 0 };
	unsigned int speed_ = 6;
	bool GAMEOVER_ = false;
	bool playedMusic_ = false;
	HAPI_TColour colour_ = HAPI_TColour((HAPISPACE::BYTE)10.f, (HAPISPACE::BYTE)10.f, (HAPISPACE::BYTE)10.f);
};

