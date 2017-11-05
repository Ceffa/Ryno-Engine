#include "Material.h"



namespace Ryno {

	bool Material::set_shader(Shader * _shader)
	{
		shader = _shader;
		free(attribute_memory);
		attribute_memory = calloc(shader->attributes_struct_size,1);
		if (!attribute_memory){
			std::cout << "Failed to allocate material struct" << std::endl;
			return false;
		}
		for (auto& cnt : shader->uniforms_data){
			uniform_map[cnt.first] = calloc (cnt.second.size,1);
		}
		return true;
	}

	

	Material::Material(const Material& cp)
	{
		copy(cp);
	}


	void Material::copy(const Material& cp)
	{
		if (attribute_memory) {
			free(attribute_memory);
			attribute_memory = nullptr;
		}
		uniform_map.clear();
		//Set same shader
		set_shader(cp.shader);
		//Copy entry of uniform map
		for (const auto& entry : cp.uniform_map) {
			if (entry.second == nullptr) continue;
			uniform_map[entry.first] = malloc(shader->uniforms_data[entry.first].size);
			std::memcpy(uniform_map[entry.first], entry.second, shader->uniforms_data[entry.first].size);
		}
		//Copy attributes memory
		std::memcpy(attribute_memory, cp.attribute_memory, shader->attributes_struct_size);

	}

}
