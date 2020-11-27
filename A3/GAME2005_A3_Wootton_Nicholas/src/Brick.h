#pragma once
#ifndef __Brick__
#define __Brick__

#include "DisplayObject.h"
#include "TextureManager.h"
#include "Target.h"

class Brick final : public DisplayObject
{
public:
	Brick();
	~Brick();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void reset();
	void setDebug(bool debug);
	void setSimulationActive(bool active);
	void setMovement(glm::vec2 movement);

	void setTargetRef(Target* ref);

private:
	void move();
	void m_checkCollision();

	float velocityMultiplier = 200.0f;
	bool m_debug;
	bool m_simulationActive;

	Target* m_pTargetRef;
};


#endif /* defined (__Brick__) */

