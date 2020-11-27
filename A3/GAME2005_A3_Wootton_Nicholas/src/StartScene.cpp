#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(SCENE_1);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(SCENE_2);
	}
}

void StartScene::start()
{
	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pStartLabel = new Label("START SCENE", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Press 1 for scene 1, 2 for scene 2", 
		"Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_pTeamNamesLabel = new Label("Team: Nicholas Wootton", "Consolas", 30, blue, glm::vec2(400.0f, 160.0f));
	m_pTeamNamesLabel->setParent(this);
	addChild(m_pTeamNamesLabel);
	
	//m_pShip = new Ship();
	//m_pShip->getTransform()->position = glm::vec2(400.0f, 300.0f); 
	//addChild(m_pShip); 

	// Scene 1 Button
	m_pScene1Button = new Button();
	m_pScene1Button->getTransform()->position = glm::vec2(300.0f, 400.0f); 

	m_pScene1Button->addEventListener(CLICK, [&]()-> void
	{
		m_pScene1Button->setActive(false);
		TheGame::Instance()->changeSceneState(SCENE_1);
	});
	
	m_pScene1Button->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pScene1Button->setAlpha(128);
	});

	m_pScene1Button->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pScene1Button->setAlpha(255);
	});
	addChild(m_pScene1Button);

	// Scene 2 Button
	m_pScene2Button = new Button();
	m_pScene2Button->getTransform()->position = glm::vec2(500.0f, 400.0f);

	m_pScene2Button->addEventListener(CLICK, [&]()-> void
		{
			m_pScene2Button->setActive(false);
			TheGame::Instance()->changeSceneState(SCENE_2);
		});

	m_pScene2Button->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pScene2Button->setAlpha(128);
		});

	m_pScene2Button->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pScene2Button->setAlpha(255);
		});
	addChild(m_pScene2Button);
	
}

