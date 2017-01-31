#pragma once
#include "Visualisation.h"
#include "Input.h"
#include <string>
#include <map>

//Base world entity
class WorldEntity
{
public:
	WorldEntity();
	virtual ~WorldEntity();

	virtual void Update() = 0;

	void Render(Visualisation& vis);
	bool Collision(WorldEntity& other);

	void Kill();
	void Live(int posX, int posY);

	const bool isDead();
	Rectangle getBox() const { return box_; };

protected:
	int posX_;
	int posY_;
	int speed_;
	bool isDead_;

	std::string spriteName_;
	Rectangle box_;

};

//Projectile 
class ProjectileEntity : public WorldEntity
{
public:
	void Update() override final;
	void Initialise(const std::string spriteName, Rectangle box, int posX, int posY, int speed);
};

//Player
class PlayerEntity : public WorldEntity
{
public:
	~PlayerEntity();
	void Update() override final;
	void Initialise(const std::string spriteName, Rectangle box, Rectangle ammoBox, int posX, int posY, int speed);
	void RenderProjectiles(Visualisation& vis);
	std::map<int, ProjectileEntity*>& getAmmo(int type);

	void Fire();
private:
	Input playerInput_;

	int fireTick_{ 0 };
	bool fireReady_ = false;

	std::map<int, ProjectileEntity*> primaryProjectiles_;
	std::map<int, ProjectileEntity*> secondaryProjectiles_;
};

//Enemy
class EnemyEntity : public WorldEntity
{
public:
	~EnemyEntity();
	void Update() override final;
	void UpdateProjectiles();
	void Initialise(const std::string spriteName, Rectangle box, Rectangle ammoBox, int posX, int posY, int speed);
	void RenderProjectiles(Visualisation & vis);

	std::map<int, ProjectileEntity*>& getAmmo() { return projectiles_; };

	void Fire();
	void Spawn();

private:
	std::map<int, ProjectileEntity*> projectiles_;
	int currentSpeed_ = 5;


};

