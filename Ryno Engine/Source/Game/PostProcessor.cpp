#include "PostProcessor.h"

namespace Ryno{
	Shader& PostProcessor::add_effect(const std::string & name, Owner location)
	{
		effects.emplace_back(name, location);
		return effects.back();
	}
}

