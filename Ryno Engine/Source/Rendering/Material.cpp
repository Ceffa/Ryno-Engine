#include "Material.h"



namespace Ryno {

	bool Material::set_shader(Shader * _shader)
	{
		shader = _shader;

		if(attribute_memory)
			free(attribute_memory);
		if (uniform_memory)
			free(uniform_memory);

		attribute_memory = calloc(shader->attributes_struct_size, 1);
		uniform_memory = calloc(shader->uniforms_map_size, 1);
		
		if (!attribute_memory || !uniform_memory){
			std::cout << "Failed to allocate material structs" << std::endl;
			return false;
		}		
		return true;
	}

	

	void Material::send_uniforms_to_shader() const
	{
		U8 sampler_index = 0;
		for (auto& entry : shader->uniforms_map) {
			auto& x = entry.second;
			shader->send_uniform_to_shader(entry.first, (void*)((U64)uniform_memory + x.offset), &sampler_index);
		}
	}

	Material::Material(const Material& cp)
	{
		copy(cp);
	}


	void Material::copy(const Material& cp)
	{
		//Set same shader
		set_shader(cp.shader);
		

		//Copy attributes memory
		std::memcpy(attribute_memory, cp.attribute_memory, shader->attributes_struct_size);
		std::memcpy(uniform_memory, cp.uniform_memory, shader->uniforms_map_size);

	}

}
