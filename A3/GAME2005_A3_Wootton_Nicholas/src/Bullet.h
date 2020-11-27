#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "DisplayObject.h"
#include "Sprite.h"

class Bullet : public Sprite
{
public:
	Bullet();
	~Bullet();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void spawn();
	void despawn();
	void explode();
	void setGravity(float grav);
	void setActive(float active);
	bool getActive();
	void setDebug(bool debug);

	int m_explosionTimer;
	int maxExplosionTime = 18;

private:
	void m_move();
	void m_buildAnimations();

	bool m_active;
	float m_gravity;
	float explosionSpeed = 1.0f;
	float groundLevel = 600.0f;
	bool m_debug;
	float rotation = 0;
	float rotationSpeed;
};


#endif /* defined (__BULLET__) */