#include "Brick.h"
#include "glm/gtx/string_cast.hpp"
#include "TextureManager.h"
#include "Renderer.h"
#include "Util.h"
#include "CollisionManager.h"
#include <iostream>
using namespace std;

Brick::Brick()
{
	TextureManager::Instance()->load("../Assets/textures/spaceBuilding_005.png","Brick");

	auto size = TextureManager::Instance()->getTextureSize("Brick");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	m_debug = false;
	m_simulationActive = false;
	setType(BRICK);
}

Brick::~Brick()
= default;

void Brick::draw()
{
	//draw
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->draw("Brick", x, y, 90, 255, true);
	
	//debug
	if (m_debug)
	{
		SDL_Renderer* renderer = Renderer::Instance()->getRenderer();
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect rect = { x - getHeight() / 2, y - getWidth() / 2, 
			getHeight(), getWidth() }; //width & height swapped to rotate 90 degrees
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void Brick::update()
{
	move();
	m_checkCollision();
}

void Brick::move()
{
	float deltaTime = 1.0f / 60.0f;
	getTransform()->position += getRigidBody()->velocity * deltaTime;
}

void Brick::m_checkCollision()
{
	float ax = m_pTargetRef->getTransform()->position.x; //center x
	float ay = m_pTargetRef->getTransform()->position.y; //center y
	float aw = m_pTargetRef->getWidth();

	float bw = getHeight();
	float bh = getWidth();
	float bx = getTransform()->position.x; //top left x
	float by = getTransform()->position.y; //top left y

	SDL_Rect rect1 = { ax, ay, aw, 0 };
	SDL_Rect rect2 = { bx, by, bw, bh };

	if (CollisionManager::circleBoxCheck(rect1, rect2))
	{
		m_pTargetRef->collide(getTransform()->position);
	}
}

void Brick::reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void Brick::clean() {}

void Brick::setMovement(glm::vec2 movement)
{
	getRigidBody()->velocity = movement * velocityMultiplier;
}

void Brick::setTargetRef(Target* ref)
{
	m_pTargetRef = ref;
}

void Brick::setDebug(bool debug)
{
	m_debug = debug;
}

void Brick::setSimulationActive(bool active)
{
	m_simulationActive = active;
}
