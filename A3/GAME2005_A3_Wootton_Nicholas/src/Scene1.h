#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Projectile.h"
#include "BulletPool.h"
#include "Background.h"

class Scene1 : public Scene
{
public:
	Scene1();
	~Scene1();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	
private:
	// IMGUI Function
	void GUI_Function();
	char* NameAndFloatToChar(const char* text, float f);
	std::string m_guiTitle;

	//glm::vec2 m_mousePosition;

	//Objects
	Player* m_pPlayer;
	Background* m_pBackground;
	int bulletPoolSize = 10;
	BulletPool* m_pBulletsPool;
	
	//physics
	bool m_bSimulationActive = false; //prevents input from imgui during simulation
	float m_scale = 1.0f;
	bool m_debug = false;
	bool m_pause = false; //pauses updates

	// UI Items
	//Button* m_pBackButton;
	//Button* m_pNextButton;
};

#endif /* defined (__PLAY_SCENE__) */