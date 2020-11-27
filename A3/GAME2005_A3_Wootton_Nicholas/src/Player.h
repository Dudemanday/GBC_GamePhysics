#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "FacingDirection.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// setters
	void setAnimationState(PlayerAnimationState state);
	void setDebug(bool debug);

	//movement
	void moveLeft();
	void moveRight();
	void jump();
	bool isJumping();
	void zeroXVelocity();
	void setGravity(float gravity);

private:
	void m_buildAnimations();
	bool m_debug = true;

	//movement
	float verticalSpeed = -250.0f; //initial jump velocity
	float horizontalSpeed = 200.0f; //constant hoizontal velocty
	float m_gravity = 9.8; 
	bool m_bJumping = false;
	float groundLevel = 610.0f;

	PlayerAnimationState m_currentAnimationState;
	SDL_RendererFlip m_lastFacingDirection; //facing right or left 
};

#endif /* defined (__PLAYER__) */