#pragma once
#ifndef __END_SCENE__
#define __END_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "Target.h"
#include "Brick.h"
#include "Background.h"

class Scene2 final : public Scene
{
public:
	Scene2();
	~Scene2();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	//objects
	Brick* m_pBrick;
	Target* m_pTarget;
	Background* m_pBackground;

	//physics
	bool m_bSimulationActive = false; //prevents input from imgui during simulation
	float m_scale = 1.0f;
	bool m_debug = false;
	bool m_pause = false; //pauses updates
};

#endif /* defined (__END_SCENE__) */