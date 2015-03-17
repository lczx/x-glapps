#pragma once

#include <vector>

#include "EngineSpec.h"

class UISpecSelector
{
public:
	/** Displays a console user interface to let the user select an engine. */
	const EngineSpec &selectSpec();

	/** Selects an engine from a specific identification code */
	const EngineSpec &selectSpec(const std::string &id);

private:
	// Returns 'true' if an error occurred
	bool clearConsole();

	void exitApplication();
	
};
