#include "Global.h"
#include <vector>
#include "Material.h"

namespace Ryno {

	class PostProcessor {
		
	public:
		~PostProcessor() { }
		Material& add_effect(const std::string& name, Owner location);

		std::vector<Material> effects{};
	};

}