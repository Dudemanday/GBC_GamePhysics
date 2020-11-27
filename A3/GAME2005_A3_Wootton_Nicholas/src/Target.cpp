#include "Target.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Util.h"
#include <iostream>
using namespace std;

Target::Target()
{
	//textures
	TextureManager::Instance()->load("../Assets/textures/spaceMeteors_002.png","meteor");
	const auto size = TextureManager::Instance()->getTextureSize("meteor");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	m_debug = false;
	m_simulationActive = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	//draw
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->draw("meteor", x, y, 0, 255, true);

	//debug
	if (m_debug)
	{
		SDL_Renderer* renderer = Renderer::Instance()->getRenderer();
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		for (int angle = 0; angle < 360; angle++)
		{
			float endX = cos(angle) * getWidth() / 2 + getTransform()->position.x;
			float endY = sin(angle) * getWidth() / 2 + getTransform()->position.y;
			SDL_RenderDrawPoint(renderer, endX, endY);
		}
	}
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean() {}

void Target::m_move()
{
	//energy loss
	getRigidBody()->velocity *= energyLossMultiplier;

	//zero out velocity when slow enough
	if (getRigidBody()->velocity.x < 0.1 && getRigidBody()->velocity.x > -0.1)
	{
		getRigidBody()->velocity.x = 0;
	}
	else if (getRigidBody()->velocity.y < 0.1 && getRigidBody()->velocity.y > -0.1)
	{
		getRigidBody()->velocity.y = 0;
	}

	//move
	float deltaTime = 1.0f / 60.0f;
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * deltaTime;
}

//check if target has hit the edge of the screen
//  if it has, then 'bounce' off the edge of said screen
void Target::m_checkBounds()
{
	//starting vars
	glm::vec2 pos = getTransform()->position;
	glm::vec2 vel = getRigidBody()->velocity;
	glm::vec2 bottomRightCorner = { 800.0f - getWidth() / 2, 600.0f - getHeight() / 2};
	glm::vec2 topLeftCorner = { getWidth() / 2 , getHeight() / 2 };

	//inverts velocity to bounce and sets pos to the edge of the screen 
	if (pos.x > bottomRightCorner.x)
	{
		pos.x = bottomRightCorner.x;
		vel.x = -vel.x;
	}
	if (pos.x < topLeftCorner.x)
	{
		pos.x = topLeftCorner.x;
		vel.x = -vel.x;
	}
	if (pos.y > bottomRightCorner.y)
	{
		pos.y = bottomRightCorner.y;
		vel.y = -vel.y;
	}
	if (pos.y < topLeftCorner.y)
	{
		pos.y = topLeftCorner.y;
		vel.y = -vel.y;
	}
	//set new vel/pos
	getTransform()->position = pos;
	getRigidBody()->velocity = vel;
}

void Target::spawn()
{
	getTransform()->position = glm::vec2(50 + rand() % 700, 50 + rand() % 500);
}

void Target::collide(glm::vec2 objPos)
{
	getRigidBody()->velocity = (getTransform()->position - objPos) * colVelMultiplier * energyLossMultiplier;
}

void Target::reset() {}

void Target::setDebug(bool debug)
{
	m_debug = debug;
}

void Target::setSimulationActive(bool active)
{
	m_simulationActive = active;
}
