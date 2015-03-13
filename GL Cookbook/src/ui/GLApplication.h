#pragma once

#include <memory>
#include <functional>

#include "../engines/GLEngine.h"

struct EngineSpec
{
	std::string name;
	std::function<GLEngine *()> genInstance;
};

class GLApplication
{
public:
	/** GL Context initialization */
	static void initializeGLContext(int *argcp, char **argv, int glMajorVersion, int glMinorVersion);

	/** Construct with RHS Engine pointer */
	explicit GLApplication(GLEngine *&&pEngine) : hookClass_(pEngine) { }

	/** Construct with unique Engine pointer, move */
	explicit GLApplication(std::unique_ptr<GLEngine> &&pEngine) : hookClass_(std::move(pEngine)) { }

	/** Construct with meta Engine specification */
	explicit GLApplication(EngineSpec &spec) : engineName_(spec.name), hookClass_(spec.genInstance()) { }

	/** Create viewport window */
	void createWindow(int width, int height);

	/** Show device and version info to stdout */
	void glewShowInfo();

	/** Enter the main message loop */
	void loop();

private:
	void glewInitialize_();

	static int glMajorVersion, glMinorVersion;

	std::string engineName_;
	std::unique_ptr<GLEngine> hookClass_;

};
