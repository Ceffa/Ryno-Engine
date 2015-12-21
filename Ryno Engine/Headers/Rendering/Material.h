#pragma once
#include "Global.h"
#include "Shader.h"
#include "GenericMap.h"
#include <map>


namespace Ryno{

	enum Type {VERTEX, INSTANCE, UNIFORM};

	struct info{
		U32 index; //number of attribute
		U32 quantity; //how many
		U32 type; //like GL_FLOAT
		U32 normal; //like GL_TRUE
	};
	class Material{
	public:
		/*Shader* program;
		GenericMap maps[3];
		std::list<info> meta[3];
		U8 sizes[3];
		void* vertex;
		void* instance;
		U8 indices;
		

		void add_f32_3(const std::string& name, const glm::vec3& f32_3, Type type){
			meta[type].emplace_back(indices++, 3, GL_FLOAT, GL_FALSE);
			maps[type].add(name, f32_3);
			sizes[type] += sizeof(sizeof(F32) * 3);

		}
		void add_f32_4(const std::string& name, const glm::vec4& f32_4, Type type){
			meta[type].emplace_back(indices++,4, GL_FLOAT, GL_FALSE);
			maps[type].add(name, f32_4);
			sizes[type] += sizeof(sizeof(F32) * 4);
		}
		void add_u8_4(const std::string& name, const ColorRGBA& u8_4, Type type){
			meta[type].emplace_back(indices++,4, GL_UNSIGNED_BYTE, GL_TRUE);
			maps[type].add(name, u8_4);
			sizes[type] += sizeof(sizeof(U8) * 4);

		}
		void add_f32_1(const std::string& name, float f32_1, Type type){
			meta[type].emplace_back(indices++, 1, GL_FLOAT, GL_FALSE);
			maps[type].add(name, f32_1);
			sizes[type] += sizeof(sizeof(F32));
		}


		void send_vertices(){

		}*/


	};





}