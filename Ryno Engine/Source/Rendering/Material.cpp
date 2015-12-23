#include "Material.h"
#include "Mallocator.h"


namespace Ryno {

	bool Material::set_shader(Shader * _shader)
	{
		shader = _shader;
		free(attribute_memory);
		attribute_memory = malloc(shader->attributes_struct_size);
		if (!attribute_memory){
			std::cout << "Failed to allocate material struct" << std::endl;
			return false;
		}
		for (auto cnt : shader->uniforms_map){
			uniform_map[cnt.first] = nullptr;
		}
		return true;
	}

	

}
