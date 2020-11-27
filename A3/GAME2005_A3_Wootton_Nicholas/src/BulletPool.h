#pragma once
#ifndef __BULLET_POOL__
#define __BULLET_POOL__

#include "Bullet.h"
#include "Player.h"

class BulletPool : public DisplayObject
{
public:
	BulletPool(int size);
	~BulletPool();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void despawn(Bullet* bullet);
	void resetAll();
	void setGravity(float grav);
	void setSimulation(bool active);
	void setDebug(bool debug);

	void setPlayerRef(Player* player);
	glm::vec2 getBulletPoolsSize();

	std::vector<Bullet*> active;
	std::vector<Bullet*> inactive;

	const float bulletSpawnTime = 500.0f;
	float bulletTimerStart;

private:
	void spawnBullet();
	bool checkCollision(Bullet* bullet);

	bool m_bSimulationActive;
	Player* m_pPlayerRef;

	float groundLevel = 600.0f;
};

#endif /* defined (__BULLET_POOL__) */