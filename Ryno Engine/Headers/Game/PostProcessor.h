#include "Global.h"
#include <vector>
#include "Shader.h"

namespace Ryno {

	class PostProcessor {
		
	public: 
		Shader& add_effect(const std::string& name, Owner location);

		std::vector<Shader> effects;
	};

}