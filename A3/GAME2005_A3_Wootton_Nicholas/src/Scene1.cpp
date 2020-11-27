#include "Scene1.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"

#include "FacingDirection.h"
#include "Renderer.h"
#include <iostream>
Scene1::Scene1()
{
	Scene1::start();
}

Scene1::~Scene1()
= default;

void Scene1::draw()
{
	m_pBackground->draw();
	GUI_Function();
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void Scene1::update()
{
	if (!m_pause)
	{
		updateDisplayList();
	}
}

void Scene1::clean()
{
	removeAllChildren();
}

void Scene1::handleEvents()
{
	EventManager::Instance().update();
	
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	//scenes
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(SCENE_2);
	}

	if (!m_pause)
	{
		//player movement

		m_pPlayer->setAnimationState(PLAYER_IDLE); //reset anim state

		if (!m_pPlayer->isJumping())
		{
			m_pPlayer->zeroXVelocity();
			if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
			{
				m_pPlayer->jump();
			}
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->moveLeft();
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->moveRight();
		}
	}
}

void Scene1::start()
{
	SoundManager::Instance().setAllVolume(2);
	// Set GUI Title
	m_guiTitle = "Play Scene";

	//background
	m_pBackground = new Background(SCENE1);

	//Player 
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_pPlayer->setDebug(m_debug);

	//bullet pool
	m_pBulletsPool = new BulletPool(bulletPoolSize);
	addChild(m_pBulletsPool);
	m_pBulletsPool->setDebug(m_debug);
	m_pBulletsPool->setPlayerRef(m_pPlayer);
}

void Scene1::GUI_Function()
{
	//game screen
	//width: 800, height: 600
	//16m x 12m

	//imgui
	static float buttonWidth = 150.0f;
	static float textWidth = 80.0f;

	//default values
	static float gravity = { 0.98 };
	static float friction = { 0.42f };
	static float pos[2] = { 1.0f, 11.0f };
	static float rampSize[2] = { 4.0f, 3.0f };

	// Always open with a NewFrame
	ImGui::NewFrame();

	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	//Scene selector
	ImGui::Text("Scene Selector");
	if (ImGui::Button("Start Scene"))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}
	ImGui::SameLine(buttonWidth * 0.6 + 10);
	if (ImGui::Button("Scene 1"))
	{
		TheGame::Instance()->changeSceneState(SCENE_1);
	}
	ImGui::SameLine(buttonWidth + 15);
	if (ImGui::Button("Scene 2"))
	{
		TheGame::Instance()->changeSceneState(SCENE_2);
	}
	ImGui::Separator();


	//Physics Input
	ImGui::Text("Physics Input");
	if (ImGui::Button("Start Simulation"))
	{
		m_bSimulationActive = true;
		m_pBulletsPool->setSimulation(true);
	}
	ImGui::SameLine(buttonWidth + 10);
	if (ImGui::Button("Reset"))
	{
		if (m_bSimulationActive)
		{
			m_bSimulationActive = false;
			m_pBulletsPool->resetAll();

			//sliders and inputs from imgui can be changed during simulation,
			//  but they are not passed to the objects until the simulation stops
			m_pBulletsPool->setGravity(gravity);
			m_pBulletsPool->setDebug(m_debug);
			m_pPlayer->setDebug(m_debug);
		}
	}

	//gravity input
	if (ImGui::SliderFloat("Gravity (m/s)", &gravity, 0.0f, 5.0f))
	{
		if (!m_bSimulationActive)
		{
			m_pBulletsPool->setGravity(gravity);
			m_pPlayer->setGravity(gravity);
		}
	}
	ImGui::Separator();

	//debug and pause buttons
	if (ImGui::Checkbox("Debug Mode", &m_debug))
	{
		m_pBulletsPool->setDebug(m_debug);
		m_pPlayer->setDebug(m_debug);
	}
	ImGui::SameLine(buttonWidth);
	ImGui::Checkbox("Pause", &m_pause);
	ImGui::Separator();

	//------Statistics output------
	//gets bullet vector sizes
	glm::vec2 bullets = m_pBulletsPool->getBulletPoolsSize(); //vec2(active bullets, inactive bullets)
	ImGui::Text("Bullets");

	//active
	char activeBulletsText[16] = "";
	strcat(activeBulletsText, NameAndFloatToChar("Active:", bullets.x));
	ImGui::Text(activeBulletsText);
	//inactive
	char inactiveBulletsText[16] = "";
	strcat(inactiveBulletsText, NameAndFloatToChar(" | Inactive:", bullets.y));
	ImGui::SameLine(textWidth + 10);
	ImGui::Text(inactiveBulletsText);

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

char* Scene1::NameAndFloatToChar(const char* text, float f)
{
	if (sizeof(text) > 0) //checks if numbers are valid
	{
		if (f == f) //checks if text is valid
		{
			char val[16];
			sprintf(val, "%G", floorf((f / m_scale) * 100) / 100); //float to 2 decimals, then float->char
			char temp[32] = "";
			strcat(temp, text);
			strcat(temp, val);
			return temp;
			}
		else
		{
			//std::cout << "invalid float" << std::endl;
			return "invalid float";
		}
	}
	else
	{
		//std::cout << "invalid text" << std::endl;
		return "invalid text";
	}
	return "";
}


