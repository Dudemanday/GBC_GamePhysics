#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void reset();
	void setDebug(bool debug);
	void setSimulationActive(bool active);
	void spawn();

	void collide(glm::vec2 objPos);

private:
	void m_move();
	void m_checkBounds();

	bool m_debug;
	bool m_simulationActive;
	float colVelMultiplier = 20;
	float energyLossMultiplier = 0.9;
};

#endif /* defined (__TARGET__) */