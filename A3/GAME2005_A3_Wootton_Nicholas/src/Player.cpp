#include "Player.h"
#include "TextureManager.h"
#include "Renderer.h"

Player::Player(): m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));

	// set frame width / height
	setWidth(53);
	setHeight(58);

	getTransform()->position = glm::vec2(400.0f, groundLevel - getHeight());
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	setType(PLAYER);
	m_currentAnimationState = PLAYER_IDLE;
	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	
	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("idle"),
			x, y, 0.25f, 0, 255, true, m_lastFacingDirection);
		break;
	case PLAYER_RUN_VERTICAL:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, m_lastFacingDirection);
		break;
	case PLAYER_RUN_RIGHT:
		m_lastFacingDirection = SDL_FLIP_NONE;
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, m_lastFacingDirection);
		break;
	case PLAYER_RUN_LEFT:
		m_lastFacingDirection = SDL_FLIP_HORIZONTAL;
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, m_lastFacingDirection);
		break;
	default:
		break;
	}

	if (m_debug)
	{
		SDL_Renderer* renderer = Renderer::Instance()->getRenderer();
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect rect = { getTransform()->position.x - getWidth() / 2, 
			getTransform()->position.y - getHeight() / 2, getWidth(), getHeight() };
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-3"));

	setAnimation(runAnimation);
}

void Player::update()
{
	float deltaTime = 1.0f / 60.0f;
	getRigidBody()->velocity += getRigidBody()->acceleration;
	getTransform()->position += getRigidBody()->velocity * deltaTime;

	if (getTransform()->position.y >= groundLevel - getHeight() && m_bJumping)
	{
		m_bJumping = false;
		getRigidBody()->acceleration.y = 0;
		getRigidBody()->velocity.y = 0;
		getTransform()->position.y = groundLevel - getHeight();
	}
}

void Player::moveLeft()
{
	getRigidBody()->velocity = glm::vec2(-horizontalSpeed, getRigidBody()->velocity.y);
	m_currentAnimationState = PLAYER_RUN_LEFT;
}

void Player::moveRight()
{
	getRigidBody()->velocity = glm::vec2(horizontalSpeed, getRigidBody()->velocity.y);
	m_currentAnimationState = PLAYER_RUN_RIGHT;
}

void Player::jump()
{
	getRigidBody()->velocity.y = verticalSpeed;
	getRigidBody()->acceleration.y = m_gravity;
	m_bJumping = true;
}

void Player::clean() {}

void Player::setAnimationState(PlayerAnimationState state)
{
	m_currentAnimationState = state;
}

void Player::setDebug(bool debug)
{
	m_debug = debug;
}

bool Player::isJumping()
{
	return m_bJumping;
}

void Player::zeroXVelocity()
{
	getRigidBody()->velocity = glm::vec2(0.0f, getRigidBody()->velocity.y);
}

void Player::setGravity(float gravity)
{
	m_gravity = gravity;
}
