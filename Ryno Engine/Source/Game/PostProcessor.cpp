#include "PostProcessor.h"

namespace Ryno{

	//Shader is created inside the post processor, and it will be deleted by it.
	//The scene doesn't need to do it
	Material& PostProcessor::add_effect(const std::string & name, Owner location)
	{
		effects.emplace_back();
		auto& m = effects.back();
		m.set_shader(new Shader(name, location));
		return m;
	}
}

