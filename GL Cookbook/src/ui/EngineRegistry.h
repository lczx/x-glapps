#pragma once

#include <map>
#include <vector>

#include "EngineSpec.h"

#include "../engines/chapter_1/1_GettingStarted.h"
#include "../engines/chapter_1/2_SimpleTriangle.h"
#include "../engines/chapter_1/3_RippleDeformer.h"
#include "../engines/chapter_1/4_Subdivision.h"
#include "../engines/chapter_1/6_ImageLoader.h"

typedef std::map<std::string, std::vector<EngineSpec>> EngineRegistryData;

class EngineRegistry
{
public:
	static EngineRegistry &getInstance()
	{
		// Instance on first use, will be destroyed at the end
		static EngineRegistry instance;
		return instance;
	}

	static EngineRegistryData &getInstanceData()
	{
		return getInstance().engines_;
	}

private:
	EngineRegistry()
	{
		engines_["Chapter 1"] = {
			{ "Getting Started", [] { return new GettingStartedEngine(); } },
			{ "Simple Triangle", [] { return new SimpleTriangleEngine(); } },
			{ "Ripple Deformer", [] { return new RippleDeformerEngine(); } },
			{ "Plane subdivision", [] { return new SubdivisionEngine(false); } },
			{ "Plane subdivision, instanced", [] { return new SubdivisionEngine(true);  } },
			{ "Image Loader", [] { return new ImageLoaderEngine("assets/puerto_pollo.png"); } }
		};
	}

	EngineRegistry(const EngineRegistry &) = delete; // Remove copy constructor
	void operator=(const EngineRegistry &) = delete; // Remove assignment op.

	EngineRegistryData engines_;

};
