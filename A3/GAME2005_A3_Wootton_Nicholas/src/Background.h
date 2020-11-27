#pragma once
#include "DisplayObject.h"
#include "BackgroundType.h"

class Background : public DisplayObject
{
public:
	Background(BackgroundType bg);
	~Background();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void newBackground(BackgroundType bg);

private:
	BackgroundType m_backgroundType;
};

