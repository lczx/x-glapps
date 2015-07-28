#pragma once

#include "../GLEngine.h"
#include "../../util/MouseFilter.h"

//#define USE_BOOK_INCLUDES

#ifndef USE_BOOK_INCLUDES
#include "../../util/objects/TexturedPlane.h"
#include "../../util/camera/FreeCamera.h"
#else
#include "../../util/_orig_/_TexturedPlane.h"
#include "../../util/_orig_/_FreeCamera.h"

typedef CTexturedPlane TexturedPlane;
typedef CFreeCamera FreeCamera;
#endif

class FreeCamWorld : public GLEngine
{
public:
	FreeCamWorld();

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

	void onMouseDown(int button, int s, int x, int y) override;
	void onMouseMove(int x, int y) override;
	void onKey(unsigned char key, int x, int y) override;

	void onIdle() override;

	// Camera transformation variables
	int state_ = 0, oldX_ = 0, oldY_ = 0;
	float rX_ = 0, rY_ = 0, fov_ = 45;

	// Delta time
	float dt_ = 0;

	// Timing related variables
	float lastTime_ = 0, currentTime_ = 0;

	// Free camera instance
	FreeCamera cam;

	// Mouse filter
	MouseFilter mouseFilter;

	// Floor checker texture ID and checkered plane object
	GLuint checkerTextureID_;
	TexturedPlane *pCheckerPlane_;

};
