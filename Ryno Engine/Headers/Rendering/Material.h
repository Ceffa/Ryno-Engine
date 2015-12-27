#pragma once
#include "Global.h"
#include "Shader.h"
#include "GenericMap.h"
#include <map>


namespace Ryno{

	enum Type { VERTEX, INSTANCE, UNIFORM };



	class Material{
	public:

		bool set_shader(Shader * _shader);
		template <class T>
		bool set_attribute(const std::string& attr, T val){
			auto res = shader->attributes_map.find(attr);
			if (res == shader->attributes_map.end()){
				std::cout << "Attribute " << attr << " not found in shader" << '\n';
				return false;
			}
			*(T*)((U64)attribute_memory + res->second.offset) = val;
		}
		template <class T>
		bool set_uniform(const std::string& attr, T* val){
			auto res = uniform_map.find(attr);
			if (res == uniform_map.end()){
				return false;
			}

			uniform_map[attr] = (void*)val;
			return true;
		}


		~Material(){ free(attribute_memory); }
		void* attribute_memory = nullptr;
		std::map<std::string, void*> uniform_map;
		Shader* shader;



	};
}