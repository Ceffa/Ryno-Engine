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
		bool set_attribute(const std::string& attr, const T& val){
			auto res = shader->attributes_map.find(attr);
			if (res == shader->attributes_map.end()){
				std::cout << "Attribute " << attr << " not found in shader" << '\n';
				return false;
			}
			*(T*)((U64)attribute_memory + res->second.offset) = val;
		}

		void* get_attribute(const std::string& attr) {
			auto res = shader->attributes_map.find(attr);
			if (res == shader->attributes_map.end()) {
				std::cout << "Attribute " << attr << " not found in shader" << '\n';
				return nullptr;
			}
			return (void*)((U64)attribute_memory + res->second.offset);
		}
	
		template <class T>
		bool set_uniform(const std::string& attr, const T& val){
			auto res = uniform_map.find(attr);
			if (res == uniform_map.end()){
				return false;
			}
			if (uniform_map[attr] == nullptr){
				uniform_map[attr] = malloc(sizeof(T));
			}
			*(T*)uniform_map[attr] = val;
			return true;
		}
	
		~Material(){ 
			free(attribute_memory);
			for (auto& entry : uniform_map){
				delete entry.second;
			}
			uniform_map.clear();
		}
		Material(const Material& copy);
		void copy(const Material& copy);
		Material(){ attribute_memory = nullptr; }
		void* attribute_memory = nullptr;
		std::map<std::string, void*> uniform_map;
		Shader* shader = nullptr;



	};
}