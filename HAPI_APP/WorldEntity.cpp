#include "WorldEntity.h"

#pragma region baseEntity
WorldEntity::WorldEntity()
{
}


WorldEntity::~WorldEntity()
{
}

void WorldEntity::Render(Visualisation & vis)
{
	//render current object
	vis.render(1, spriteName_, posX_, posY_);
}

bool WorldEntity::Collision(WorldEntity & other)
{
	//Check collision box
	if (	(box_.Get(1) < other.box_.Get(0) || box_.Get(0) > other.box_.Get(1)) ||
			(box_.Get(3) < other.box_.Get(2) || box_.Get(2) > other.box_.Get(3))	)
		return false;
	else
		return true;
}

const bool WorldEntity::isDead()
{
	//Checks if current object is dead or not
	return isDead_;
}

void WorldEntity::Kill()
{
	//Sets object as dead and moves it out of play area
	isDead_ = true;
	box_.Translate(-posX_, -posY_);
	posX_ = 0;
	posY_ = 0;
}

void WorldEntity::Live(int posX, int posY)
{
	//Sets as alive and moves back into play area
	posX_ = posX;
	posY_ = posY;
	box_.Translate(posX_, posY_);

	isDead_ = false;
}

#pragma endregion

#pragma region projectile
void ProjectileEntity::Update()
{
	//Move projectile in X position
	posX_ += speed_;
	box_.Translate(speed_, 0);

	//If out of bounds, destroy projectile
	if (posX_ > 1300 || posX_ < -100)
		Kill();
}

void ProjectileEntity::Initialise(const std::string spriteName, Rectangle box, int posX, int posY, int speed)
{
	//Initialise member variables
	posX_ = posX;
	posY_ = posY;
	speed_ = speed;
	spriteName_ = { spriteName };
	isDead_ = true;
	box_ = box;

}

#pragma endregion

#pragma region player

PlayerEntity::~PlayerEntity()
{
	//Delete primary projectile pointers
	for (auto p : primaryProjectiles_)
		delete p.second;
	//Delete secondary projectile pointers
	for (auto p : secondaryProjectiles_)
		delete p.second;
}

void PlayerEntity::Update()
{
	//Manipulate collision box
	box_.Translate(-posX_,-posY_);
	//Update position
	playerInput_.Update(posX_, posY_, speed_);
	//Replace collision box
	box_.Translate(posX_, posY_);

	//If it's time to fire
	if (fireTick_ >= 12)
	{
		//Set fire check bool
		fireReady_ = true;
		fireTick_ = 0;
	}

	//If it's time to fire
	if (fireReady_)
	{
		//Fire projectile
		Fire();
	}

	//Update primary projectiles
	for (auto p : primaryProjectiles_)
		if (!p.second->isDead())
			p.second->Update();
	//Update secondary projectiles
	for (auto p : secondaryProjectiles_)
		if (!p.second->isDead())
			p.second->Update();

	//Increase the fire wait tick
	fireTick_++;
}

void PlayerEntity::Initialise(const std::string spriteName, Rectangle box, Rectangle ammoBox, int posX, int posY, int speed)
{
	//Initialise member variables
	posX_ = posX;
	posY_ = posY;
	speed_ = speed;
	spriteName_ = { spriteName };
	isDead_ = false;
	box_ = box;
	box_.Translate(posX_, posY_);

	//Initialise ammo1
	for (int i = 0; i < 20; i++)
	{
		ProjectileEntity *s = new ProjectileEntity();
		s->Initialise("ammo3", ammoBox, 0, 0, 7);
		primaryProjectiles_[i] = s;
	}

	//Initialise ammo2
	for (int i = 0; i < 5; i++)
	{
		ProjectileEntity *s = new ProjectileEntity();
		s->Initialise("ammo2", ammoBox, 0, 0, 5);
		secondaryProjectiles_[i] = s;
	}
}

void PlayerEntity::RenderProjectiles(Visualisation & vis)
{

	//Render primary projectiles
	for (auto p : primaryProjectiles_)
		if(!p.second->isDead())
			p.second->Render(vis);
	//Render secondary projectiles
	for (auto p : secondaryProjectiles_)
		if (!p.second->isDead())
			p.second->Render(vis);
}

void PlayerEntity::Fire()
{
	//Get which type of projectile to fire
	int fire = playerInput_.Firing();

	//Case of what ammo type
	switch (fire)
	{
	case 0: break;
	case 1:
	{
		//Fire primary projectile
		for (auto p : primaryProjectiles_)
			if (p.second->isDead())
			{
				//If the projectile is dead, spawn it
				p.second->Live(posX_+24, posY_+24);
				//Play firing sound
				HAPI.PlaySound("media\\p_fire.wav",HAPI_TSoundOptions(0.75f));
				fireReady_ = false;
				break;
			}
		break;
	}
	case 2:
	{
		//Fire secondary projectile
		for (auto p : secondaryProjectiles_)
			if (p.second->isDead())
			{
				//If the projectile is dead, spawn it
				p.second->Live(posX_+8, posY_+16);
				//Play firing sound
				HAPI.PlaySound("media\\s_fire.wav",HAPI_TSoundOptions(0.25f));
				fireReady_ = false;
				break;
			}
		break;
	}
	}
}

std::map<int, ProjectileEntity*>& PlayerEntity::getAmmo(int type)
{
	//Return a pointer to projectiles for collision detection
	switch (type)
	{
	case 1:
		return primaryProjectiles_;
	case 2:
		return secondaryProjectiles_;
	}

	return primaryProjectiles_;
}

#pragma endregion

#pragma region enemy
EnemyEntity::~EnemyEntity()
{
	//Delete all projectile pointers
	for (auto p : projectiles_)
		delete p.second;
}

void EnemyEntity::Update()
{
	//Check if firing
	Fire();

	//Check bounry and flip speed if necessary
	if (posY_ >= 800)
		currentSpeed_ = -speed_;
	if (posY_ <= -50)
		currentSpeed_ = speed_;

	//Update location
	posY_ += currentSpeed_;
	box_.Translate(0, currentSpeed_);

}

void EnemyEntity::UpdateProjectiles()
{
	//update projectiles
	for (auto p : projectiles_)
		if (!p.second->isDead())
			p.second->Update();
}

void EnemyEntity::Initialise(const std::string spriteName, Rectangle box, Rectangle ammoBox, int posX, int posY, int speed)
{
	//Initialise member variables
	posX_ = posX;
	posY_ = posY;
	speed_ = speed;
	currentSpeed_ = speed;
	spriteName_ = { spriteName };
	isDead_ = true;
	box_ = box;

	//Update projectiles
	for (int i = 0; i < 20; i++)
	{
		ProjectileEntity *s = new ProjectileEntity();

		s->Initialise("ammo1", ammoBox, -50, -50, -5);

		projectiles_[i] = s;
	}

}

void EnemyEntity::RenderProjectiles(Visualisation & vis)
{
	//Loop projectiles and render
	for (auto p: projectiles_)
		if (!p.second->isDead())
			p.second->Render(vis);
}

void EnemyEntity::Fire()
{
	//Check to fire
	int shot = rand() % 30;

	//If it's time to fire, search and spawn
	if (shot == 10)
		for (auto p : projectiles_)
			if (p.second->isDead())
			{
				p.second->Live(posX_, posY_+24);
				HAPI.PlaySound("media\\e_fire.wav", HAPI_TSoundOptions(0.1f));
				break;
			}
}

void EnemyEntity::Spawn()
{
	//Spawn random possition on enemy side of screen
	int x = (rand() % 600) + 600;
	int y = (rand() % 600) + 60;

	//Spawn enemy
	Live(x, y);
}

#pragma endregion