#include "Bullet.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Renderer.h"


Bullet::Bullet()
{
	//textues
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png",
		"spritesheet");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));
	TextureManager::Instance()->load("../Assets/textures/mine.png", "bomb");

	auto size = TextureManager::Instance()->getTextureSize("bomb");
	setWidth(size.x);
	setHeight(size.y);

	//sounds
	SoundManager::Instance().load("../Assets/audio/death.wav", "death", SOUND_SFX);

	getTransform()->position = glm::vec2(-50.0f, -50.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->acceleration = glm::vec2(0, m_gravity);
	getRigidBody()->isColliding = false;

	rotationSpeed = 1;
	m_explosionTimer = 0;
	m_gravity = 0.98;
	m_active = false;
	m_debug = true;
	setType(BULLET);

	m_buildAnimations();
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	if (m_active)
	{
		//falling through air normally -> draws bomb
		if (m_explosionTimer == 0)
		{
			TextureManager::Instance()->draw("bomb", x, y, rotation, 255, true);
		}
		//is currently exloding -> draw explosions spritesheet
		else if(m_explosionTimer <= maxExplosionTime)
		{
			TextureManager::Instance()->playAnimation("spritesheet", getAnimation("explosion"),
				x, y, explosionSpeed, 0, 255, true);
		}
		//debug lines
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
}

void Bullet::update()
{
	m_move();

	//checks if it is done exploding
	if (m_explosionTimer > maxExplosionTime)
	{
		despawn();
	}
	//if it is currently exloding then continue to explode
	else if (m_explosionTimer > 0)
	{
		m_explosionTimer++;
	}
	
	//rotation things
	rotation += rotationSpeed;
	if (rotation >= 180) rotation -= 360;

	//checks if the bullet has hit the ground
	if (getTransform()->position.y >= groundLevel && m_explosionTimer == 0)
	{
		explode();
	} 
}

void Bullet::clean() {}

void Bullet::m_move()
{
	float deltaTime = 1.0f / 60.0f;
	getRigidBody()->velocity += getRigidBody()->acceleration;
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * deltaTime;
}

void Bullet::spawn()
{
	getTransform()->position = glm::vec2(50 + rand() % 700, -50);
	getRigidBody()->acceleration = glm::vec2(0, m_gravity);
	rotationSpeed = rand() % 6 - 3;
	m_explosionTimer = 0;
	m_active = true;
}

void Bullet::explode()
{
	if (m_explosionTimer == 0)
	{
		getRigidBody()->velocity = glm::vec2(0, 0);
		getRigidBody()->acceleration = glm::vec2(0, 0);
		m_explosionTimer = 1;
		SoundManager::Instance().playSound("death", 0, -1);
	}
}

void Bullet::despawn()
{
	getTransform()->position = glm::vec2(-50.0f, -50.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->acceleration = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	m_explosionTimer = 0;
	m_active = false;
}

void Bullet::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "explosion";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion3"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion4"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion5"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion6"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion7"));

	setAnimation(idleAnimation);
}

void Bullet::setGravity(float grav)
{
	m_gravity = grav;
}

void Bullet::setActive(float active)
{
	m_active = active;
}

bool Bullet::getActive()
{
	return m_active;
}

void Bullet::setDebug(bool debug)
{
	m_debug = debug;
}
