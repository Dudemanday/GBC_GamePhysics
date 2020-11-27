#include "Scene2.h"
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "Renderer.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"

Scene2::Scene2()
{
	Scene2::start();
}

Scene2::~Scene2()
= default;

void Scene2::draw()
{
	m_pBackground->draw(); //IMGUI refuses to work right unless its drawn first
	GUI_Function();
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void Scene2::update()
{
	if (!m_pause)
	{
		updateDisplayList();
	}
}

void Scene2::clean()
{
	removeAllChildren();
}

void Scene2::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
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
		glm::vec2 move = { 0,0 };
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			move = { -1, move.y };
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			move = { 1, move.y };
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			move = { move.x, -1 };
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			move = { move.x, 1 };
		}
		m_pBrick->setMovement(move);
	}
}

void Scene2::start()
{
	m_pBackground = new Background(SCENE2);
	
	//objects
	m_pTarget = new Target();
	addChild(m_pTarget);
	m_pTarget->spawn();

	m_pBrick = new Brick();
	addChild(m_pBrick);
	m_pBrick->setTargetRef(m_pTarget);
}

void Scene2::GUI_Function()
{
	static float buttonWidth = 150.0f;
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

	//checkboxes
	if (ImGui::Checkbox("Debug Mode", &m_debug))
	{
		m_pTarget->setDebug(m_debug);
		m_pBrick->setDebug(m_debug);
	}
	ImGui::SameLine(buttonWidth);
	ImGui::Checkbox("Pause", &m_pause);

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}