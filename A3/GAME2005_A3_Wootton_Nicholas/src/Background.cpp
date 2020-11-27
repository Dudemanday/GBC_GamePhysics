#include "Background.h"
#include "TextureManager.h"
#include "Renderer.h"
#include <iostream>
using namespace std;

Background::Background(BackgroundType bg)
{
	TextureManager::Instance()->load("../Assets/textures/industrial-background.jpg", "scene1");
	TextureManager::Instance()->load("../Assets/textures/spaceBg2.png", "scene2");

	setWidth(800.0f);
	setHeight(600.0f);

	getTransform()->position = glm::vec2(getWidth() / 2, getHeight() / 2);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);

	m_backgroundType = bg;
}

Background::~Background() {}

void Background::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	if (m_backgroundType == SCENE1)
	{
		TextureManager::Instance()->draw("scene1", x, y, 0, 255, true);
	}
	else if(m_backgroundType == SCENE2)
	{
		TextureManager::Instance()->draw("scene2", x, y, 0, 255, true);
	}
}

void Background::update() {}

void Background::clean() {}

void Background::newBackground(BackgroundType bg)
{
	m_backgroundType = bg;
}
