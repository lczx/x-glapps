#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../util/glassert.h"

#define GLE_REG_RENDER		0x01
#define GLE_REG_RESIZE		0x02
#define GLE_REG_SHUTDOWN	0x04

#define GLE_REG_MDOWN		0x08
#define GLE_REG_MMOVE		0x10
#define GLE_REG_KEY			0x20

#define GLE_REG_IDLE		0x40

#define GLE_REGISTER_DISPLAY	GLE_REG_RENDER | GLE_REG_RESIZE | GLE_REG_SHUTDOWN
#define GLE_REGISTER_INPUTS		GLE_REG_MDOWN | GLE_REG_MMOVE | GLE_REG_KEY

#define GLE_REGISTER_DEFAULT	GLE_REGISTER_DISPLAY | GLE_REG_IDLE
#define GLE_REGISTER_ALL		GLE_REGISTER_DISPLAY | GLE_REGISTER_INPUTS | GLE_REG_IDLE

class GLEngine
{
public:
	/** Construct with no parameters, register all event types by default */
	GLEngine() : GLEngine(GLE_REGISTER_ALL) { }

	/** Construct with specified event types */
	GLEngine(DWORD dwRegFlags) : regEvtTypes_(dwRegFlags)
	{
		if (instance_)
			throw std::logic_error("Only a single engine instance allowed, blame GLUT.");
		instance_ = this;
	}

	virtual ~GLEngine()
	{
		instance_ = nullptr;
		//remove(instances.begin(), instances.end(), this);
	}

#ifndef _WIN64 // C4316: alignment on 32bit platform (on 64bit, 16-byte aligned by default)
	void *operator new(std::size_t count) { return _aligned_malloc(count, 16); }

	void operator delete(void *ptr) { _aligned_free(ptr); }
#endif

	/** Registers all the event handlers declared in this class to GLUT */
	void registerHooks() {
		if (regEvtTypes_ & GLE_REG_RENDER) glutDisplayFunc(cbOnRender);
		if (regEvtTypes_ & GLE_REG_RESIZE) glutReshapeFunc(cbOnResize);
		if (regEvtTypes_ & GLE_REG_SHUTDOWN) glutCloseFunc(cbOnShutdown);

		if (regEvtTypes_ & GLE_REG_MDOWN) glutMouseFunc(cbOnMouseDown);
		if (regEvtTypes_ & GLE_REG_MMOVE) glutMotionFunc(cbOnMouseMove);
		if (regEvtTypes_ & GLE_REG_KEY) glutKeyboardFunc(cbOnKey);

		if (regEvtTypes_ & GLE_REG_IDLE) glutIdleFunc(cbOnIdle);
	};

	/** DEPRECATED: Initialization function */
	virtual void onInit() { /* Not pure virtual anymore! */ };

private:
	// Static callbacks
	static void cbOnRender() { instance_->onRender(); }
	static void cbOnResize(int nw, int nh) { instance_->onResize(nw, nh); }
	static void cbOnShutdown() { instance_->onShutdown(); }

	static void cbOnMouseDown(int button, int s, int x, int y) { instance_->onMouseDown(button, s, x, y); }
	static void cbOnMouseMove(int x, int y) { instance_->onMouseMove(x, y); }
	static void cbOnKey(unsigned char key, int x, int y) { instance_->onKey(key, x, y); }

	static void cbOnIdle() { instance_->onIdle(); }

	// Effective thiscall event handlers
	// Effective handlers
	virtual void onRender() { }
	virtual void onResize(int w, int h) { }
	virtual void onShutdown() { }

	virtual void onMouseDown(int button, int s, int x, int y) { }
	virtual void onMouseMove(int x, int y) { }
	virtual void onKey(unsigned char key, int x, int y) { }

	virtual void onIdle() { }

	// Hmm... maybe the value returned by 'glutCreateWindow' and 'glutSetWindow'
	// can be used to hold multiple instances...

	// Engine singleton instance
	static GLEngine *instance_;

	// Registered event types bit map
	DWORD regEvtTypes_;

};
