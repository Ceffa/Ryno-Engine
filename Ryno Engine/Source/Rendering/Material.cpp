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
		for (auto cnt : shader->uniforms_data){
			uniform_map[cnt.first] = nullptr;
		}
		return true;
	}

	

	Material::Material(const Material& copy)
	{
		//Set same shader
		set_shader(copy.shader);
		//Copy entry of uniform map
		for (const auto& entry : copy.uniform_map){
			uniform_map[entry.first] = entry.second;
		}
		//Copy attributes memory
		std::memcpy(attribute_memory, copy.attribute_memory, shader->attributes_struct_size);


	}

}
