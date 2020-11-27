#include "BulletPool.h"
#include "CollisionManager.h"
#include "Renderer.h"
#include <iostream>
using namespace std;

BulletPool::BulletPool(int size)
{
	for (int i = 0; i < size; i++)
	{
		inactive.push_back(new Bullet());
	}
	bulletTimerStart = SDL_GetTicks();
}

BulletPool::~BulletPool() {}

void BulletPool::draw()
{
	for (std::vector<Bullet*>::iterator myIter = active.begin();
		myIter != active.end(); myIter++)
	{
		Bullet* bullet = *myIter;
		bullet->draw();
	}
}

void BulletPool::update()
{
	if (m_bSimulationActive)
	{
		//attempt to spawn new bullet
		if (SDL_GetTicks() - bulletTimerStart >= bulletSpawnTime)
		{
			spawnBullet();
		}

		//update bullets
		for (std::vector<Bullet*>::iterator myIter = active.begin();
			myIter != active.end(); myIter++)
		{
			Bullet* bullet = *myIter;
			bullet->update();
		}

		//check for inactive bullets in the active bullet vector
		for (std::vector<Bullet*>::iterator myIter = active.begin();
			myIter != active.end(); myIter++)
		{
			Bullet* bullet = *myIter;
			if (!bullet->getActive())
			{
				active.erase(myIter);
				inactive.push_back(bullet);
				break;
			}
		}
		//checking for bullets that have hit the ground so they can be despawned properly
		for (std::vector<Bullet*>::iterator myIter = active.begin();
			myIter != active.end(); myIter++)
		{
			Bullet* bullet = *myIter;
			if (bullet->getTransform()->position.y >= groundLevel)
			{
				if (bullet->m_explosionTimer >= bullet->maxExplosionTime)
				{
					despawn(bullet);
					break;
				}
			}
		}
		//check collision
		for (std::vector<Bullet*>::iterator myIter = active.begin();
			myIter != active.end(); myIter++)
		{
			Bullet* bullet = *myIter;
			if (checkCollision(bullet))
			{
				break;
			}
		}
	}
}

bool BulletPool::checkCollision(Bullet* bullet)
{
	float ax = bullet->getTransform()->position.x; //center x
	float ay = bullet->getTransform()->position.y; //center y
	float aw = bullet->getWidth();

	float bw = m_pPlayerRef->getWidth();
	float bh = m_pPlayerRef->getHeight();
	float bx = m_pPlayerRef->getTransform()->position.x; //top left x
	float by = m_pPlayerRef->getTransform()->position.y; //top left y

	SDL_Rect rect1 = { ax, ay, aw, 0 };
	SDL_Rect rect2 = { bx, by, bw, bh };
	bool temp = CollisionManager::circleBoxCheck(rect1, rect2);

	if (temp)
	{
		bullet->explode();
	}
	return temp;
}

void BulletPool::clean() {}

void BulletPool::spawnBullet()
{
	Bullet* bullet = NULL;
	if (inactive.size() > 0)
	{
		bullet = inactive.back();
		inactive.pop_back();
		active.push_back(bullet);
		if (bullet)
		{
			bullet->spawn();
		}
	}
	bulletTimerStart = SDL_GetTicks();
}

void BulletPool::despawn(Bullet* bullet)
{
	inactive.push_back(bullet);
	for (std::vector<Bullet*>::iterator myIter = active.begin(); myIter != active.end(); myIter++)
	{
		if (*myIter == bullet)
		{
			bullet->despawn();
			active.erase(myIter);
			return;
		}
	}
}

void BulletPool::resetAll()
{
	m_bSimulationActive = false;

	for (std::vector<Bullet*>::iterator myIter = active.begin(); myIter != active.end(); myIter++)
	{
		Bullet* bullet = *myIter;
		bullet->despawn();
		inactive.push_back(bullet);
	}
	active.clear();
}

void BulletPool::setGravity(float grav)
{
	for (std::vector<Bullet*>::iterator myIter = active.begin(); myIter != active.end(); myIter++)
	{
		(*myIter)->setGravity(grav);
	}
	for (std::vector<Bullet*>::iterator myIter = inactive.begin(); myIter != inactive.end(); myIter++)
	{
		(*myIter)->setGravity(grav);
	}
}

void BulletPool::setSimulation(bool active)
{
	m_bSimulationActive = active;
}

void BulletPool::setDebug(bool debug)
{
	for (std::vector<Bullet*>::iterator myIter = active.begin();
		myIter != active.end(); myIter++)
	{
		Bullet* bullet = *myIter;
		bullet->setDebug(debug);
	}
	for (std::vector<Bullet*>::iterator myIter = inactive.begin();
		myIter != inactive.end(); myIter++)
	{
		Bullet* bullet = *myIter;
		bullet->setDebug(debug);
	}
}

void BulletPool::setPlayerRef(Player* player)
{
	m_pPlayerRef = player;
}

glm::vec2 BulletPool::getBulletPoolsSize()
{
	return glm::vec2(active.size(), inactive.size());
}
