#pragma once

#include "../GLEngine.h"

class GettingStartedEngine : public GLEngine
{
public:
	GettingStartedEngine() : GLEngine(GLE_REGISTER_DISPLAY) { }
	void onInit() override;

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

};
