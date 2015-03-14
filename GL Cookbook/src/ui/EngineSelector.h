#pragma once

#include <vector>

#include "GLApplication.h"

class EngineSelector
{
public:
	/** Displays a console user interface to let the user select an engine. */
	const EngineSpec &selectSpec();

	/** Selects an engine from a specific identification code */
	const EngineSpec &selectSpec(const std::string &id);

	/** Registers the list of implemented engines */
	void registerEngines();

private:
	// Returns 'true' if an error occurred
	bool clearConsole();

	void exitApplication();

	std::vector<EngineSpec> c1Engines_;
	
};
