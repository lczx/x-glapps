#pragma once

#include <string>
#include <functional>

#include "../engines/GLEngine.h"

struct EngineSpec
{
	std::string name;
	std::function<GLEngine *()> genInstance;
};