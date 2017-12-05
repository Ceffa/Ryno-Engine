#pragma once
#include "Global.h"
#include "Shader.h"
#include "GenericMap.h"
#include <map>
#include <iostream>


namespace Ryno{

	enum Type { VERTEX, INSTANCE, UNIFORM };



	class Material{
	public:

		bool set_shader(Shader * _shader);
		template <class T>
		bool set_attribute(const std::string& attr, const T& val){
			auto res = shader->attributes_map.find(attr);
			if (res == shader->attributes_map.end()){
				std::cout << "Attribute " << attr << " not found in shader" << '\n';
				return false;
			}
			memcpy((void*)((U64)attribute_memory + res->second.offset), &val, res->second.size);
		}

		void* get_attribute(const std::string& attr) {
			auto& res = shader->attributes_map.find(attr);
			if (res == shader->attributes_map.end()) {
				std::cout << "Attribute " << attr << " not found in shader" << '\n';
				return nullptr;
			}
			return (void*)((U64)attribute_memory + res->second.offset);
		}
	
		template <class T>
		bool set_uniform(const std::string& attr, const T& val){
			auto& res = shader->uniforms_map.find(attr);
			if (res == shader->uniforms_map.end()){
				return false;
			}
			memcpy((void*)((U64)uniform_memory + res->second.offset), &val, res->second.size);
			return true;
		}

		void send_uniforms_to_shader() const;

	
		~Material(){ 
			free(attribute_memory);
			free(uniform_memory);
		}
		Material(const Material& copy);
		void copy(const Material& copy);
		Material() {}
		void* attribute_memory = nullptr;
		void* uniform_memory = nullptr;
		Shader* shader = nullptr;



	};
}