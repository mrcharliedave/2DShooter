#include "World.h"

World::World()
{
	//Initialse the visualisation and in turn HAPI
	if (!vis_.initialise())
		return;
}


World::~World()
{
	//Delete pointers
	for (auto p : enemies_)
		delete p.second;
}

void World::Initialise()
{
	//Activate FPS counter
	HAPI.SetShowFPS(true);

	//Load all textures
	LoadAllTextures();

	//Load all sounds
	LoadSounds();

	//Initialise enemies
	for (int i = 0; i < 10; i++)
	{
		EnemyEntity *s = new EnemyEntity();
		
		s->Initialise("enemy", vis_.getRectangle("enemy"), vis_.getRectangle("ammo1"), 0, 0, 3);

		enemies_[i] = s;
	}

	//Initialise player
	player_.Initialise("character", vis_.getRectangle("character"), vis_.getRectangle("ammo1"), 200, 200, speed_);

	//Play background music
	HAPI_TStreamedMediaOptions backopt;
	backopt.loop = true;
	backopt.volume = 0.75f;
	unsigned int soundID = 1;

	//Start background music
	HAPI.PlayStreamedMedia(std::string{ "media\\background.mp3" }, backopt, soundID);

}

void World::LoadAllTextures()
{
	//Load background texture
	std::string background{ "background" };
	if (!vis_.createSprite(1280, 720, background, "media\\background.jpg"))
	{
		HAPI.UserMessage("Couldn't load background texture.", "Error");
		return;
	}

	//Load charater texture
	std::string character{ "character" };
	if (!vis_.createSprite(64, 64, character, "media\\char.png"))
	{
		HAPI.UserMessage("Couldn't load character texture.", "Error");
		return;
	}

	//Load enemy texture
	std::string enemy{ "enemy" };
	if (!vis_.createSprite(64, 64, enemy, "media\\enemy.png"))
	{
		HAPI.UserMessage("Couldn't load enemy texture.", "Error");
		return;
	}

	//Load ammo1 texture
	std::string ammo1{ "ammo1" };
	if (!vis_.createSprite(16, 16, ammo1, "media\\ammo1.png"))
	{
		HAPI.UserMessage("Couldn't load ammo1 texture.", "Error");
		return;
	}

	//Load ammo2 texture
	std::string ammo2{ "ammo2" };
	if (!vis_.createSprite(32, 32, ammo2, "media\\ammo2.png"))
	{
		HAPI.UserMessage("Couldn't load ammo2 texture.", "Error");
		return;
	}

	//Load ammo3 texture
	std::string ammo3{ "ammo3" };
	if (!vis_.createSprite(16, 16, ammo3, "media\\ammo3.png"))
	{
		HAPI.UserMessage("Couldn't load ammo3 texture.", "Error");
		return;
	}
}

void World::LoadSounds()
{
	//Loads all sound effects into memory
	HAPI.LoadSound("media\\p_fire.wav");
	HAPI.LoadSound("media\\s_fire.wav");
	HAPI.LoadSound("media\\e_fire.wav");
	HAPI.LoadSound("media\\background.mp3");
}

void World::EnemySpawn()
{
	//Spawner counter
	if (spawnCount_ == 150)
	{
		//Spawn enemy if it's time
		for (auto p : enemies_)
			if (p.second->isDead())
			{
				p.second->Spawn();
				break;
			}
		spawnCount_ = 0;
	}
	spawnCount_++;
}

void World::Draw()
{
	if (!GAMEOVER_)
	{
		//Blit background to the screen
		vis_.render(0, "background", 0, 0);

		//Alpha blit character using Char Position
		player_.Render(vis_);

		//Render projectiles;
		player_.RenderProjectiles(vis_);
		for (auto p : enemies_)
			p.second->RenderProjectiles(vis_);

		//Render enemeis 
		for (auto p : enemies_)
			if (!p.second->isDead())
				p.second->Render(vis_);

		//Render Score
		HAPI.RenderText(36, 36, HAPI_TColour(), "Player Score: " + std::to_string(score_), 20U);
	}

	if (GAMEOVER_)
	{
		//Blit background to the screen
		vis_.render(0, "background", 0, 0);

		//Render Score
		HAPI.RenderText(400, 100, HAPI_TColour(), "Game Over! The world is destroyed.", 30U);
		HAPI.RenderText(550, 140, HAPI_TColour(), "Final Score: " + std::to_string(score_), 25U);

	}
}

void World::CheckCollisions()
{
	//Check if enemy has collided with player
	for (auto p : enemies_)
		if (!p.second->isDead())
			if (p.second->Collision(player_))
			{
				//EndGame
				p.second->Kill();
				GAMEOVER_ = true;
			}

	//Check if enemy projectile has collided with player
	for(auto a: enemies_)
		for (auto p : a.second->getAmmo() )
			if (!p.second->isDead())
				if (p.second->Collision(player_))
				{
					//EndGame
					p.second->Kill();
					GAMEOVER_ = true;
				}

	//Check if ammo block collides with enemy projectile
	for (auto p : player_.getAmmo(2))
		if (!p.second->isDead())
			for(auto a: enemies_)
				for (auto s : a.second->getAmmo())
				{
					if (s.second->Collision(*p.second))
					{
						//Kill enemy projectile and increase score
						s.second->Kill();
						score_ += 1;
					}

				}

	//Check if player projectile has collided with enemy
	for (auto p : player_.getAmmo(1))
		if (!p.second->isDead())
			for (auto s : enemies_)
			{
				if (s.second->Collision(*p.second))
				{
					//Kill projectile, collided enemy and increase score
					p.second->Kill();
					s.second->Kill();
					score_ += 10;
				}

			}
}

void World::Run()
{

	if (!GAMEOVER_) 
	{
		//Screen Wipe
		vis_.screenWipe(colour_);

		//Handle spawning
		EnemySpawn();

		//Update enemies
		for (auto p : enemies_)
			if (!p.second->isDead())
				p.second->Update();
		//Update enemy projectiles
		for (auto p : enemies_)
			p.second->UpdateProjectiles();

		//Update character entity with Input
		player_.Update();
	}

}
