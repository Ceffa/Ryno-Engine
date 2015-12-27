#pragma once

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Global.h"
#include <array>
#include <map>

namespace Ryno{

	//These are used to build the struct in the material class.
	//Basically each material creates it based on these parameters.
	struct attributes{
		U32 index;		//Index of the attribute
		U32 offset;		//Memory offset of the attribute in the struct
		U32 size;		//Size of the current attribute
	};

	//These are used to build the struct in the material class.
	//Basically each material creates it based on these parameters.
	struct uniforms{
		U32 index;		//Index of the attribute
		U32 type;
	};
	struct global_uniforms{
		U32 index;		//Index of the attribute
		U32 type;
		void* value;
	};

	class Shader
	{

	public:

		Shader(){}
		~Shader(){}

		/**
		This function create a new GLSLProgram, it loads the shaders and everything else.
		After this call, the GLSLProgram could be used.
		Use init, load_shaders, compile_shaders and link_shaders to
		separate these steps.
		@Param name of the shader
		*/
		void create(const std::string& name, bool geom, bool vert, bool frag);


		/**
		Initialize the GLSLProgram
		*/
		void init();

		/**
		Load both shaders
		*/
		void load_shaders(const std::string& name, Owner b);

		/**
		Compile both Vertex and Fragment shader.
		*/
		void compile_shaders();


		/**
		Attach shaders to this program, and then link it
		*/
		void link_shaders();

		/**
		Get info about attributes. It ignores the vertex attributes 
		for the time being, it only gets instance one, maybe this will change
		*/
		void get_attributes();

		/**
		Get uniforms from the gpu shader
		*/
		void get_uniforms();

		/**
		Get uniform location used to pass uniforms to the shader
		*/
		I32 getUniformLocation(const std::string& uniformName);

		/**
		Use the current program, call this befor starting to draw
		*/
		void use();

		/**
		Call this after drawing
		*/
		void unuse();

		/**
		Destroy this program, along with the loaded shaders. Call it at the end, not between game cycles
		*/
		void destroy();


		//THE FOLLOWING STUFF IS FOR PARSING AND AUTOMATICALLY USE ATTRIBUTES
		/**
		Bind all his attributes to the vbo. Must be called by the batch class
		*/
		void setup_vbo_attributes();

		//Size of the struct of attribused used by materials
		U32 attributes_struct_size = 0;

		//Instance attributes data:
		std::map<std::string, attributes> attributes_map;

		//Uniforms data: (each model has its own)
		std::map<std::string, uniforms> uniforms_data;
		//Global uniforms data: (sent one and for only by the shader itself)
		std::map<std::string, global_uniforms> global_uniforms_data;

		template <class T>
		bool set_global_uniform(const std::string& attr, T* value){
			auto res = global_uniforms_data.find(attr);
			//If the value is present and its not global (starts with "c_")
			if (res == global_uniforms_data.end()){
				return false;
			}
			global_uniforms_data[attr].value = (void*)value;
			return true;
		}

		//Check if two uniforms are equals (just check the pointers).
		//Be sure to NOT use two copies of a value, or they are identified as different
		static I8 compare_uniforms(void* a, void* b);


		//Check if a uniform is a sampler
		static bool is_sampler(GLenum type);

		//send uniform tto shader. Depending on its type (matrix, int etc)
		//a different function is used
		void send_material_uniform_to_shader(const std::string& name, void* value, U8* sampler_index);
		void send_global_uniform_to_shader(const std::string& name, void* value, U8* sampler_index);


		U8 get_size_from_type(const GLenum type);
	private:

		//local enum to differentiate the shader type
		enum  ShadersIndex{ VERT, GEOM, FRAG };

		bool is_shader_present[3];

		//local enum to identify the different operations on a shader
		enum  OperationIndex{ CREATE, COMPILE, LINK };

		//number of attributes input to vert shader
		U32 m_attr_no;
		//id of the glsl program
		U32 m_program_id;
		//the ids of the shaders
		U32 m_shader_ids[3];
		
		std::string shader_name;

		/**
		Load a single shader.
		@Param index -> Type of the shader
		@Param path -> Path of the shader
		*/
		void load_shader(ShadersIndex index, const std::string& path);

		/**
		Compile a previously loaded shader.
		@Param index -> shader id
		*/
		void compile_shader(ShadersIndex index);

		/**
		Function to load a shader inside a buffer
		*/
		void read_file_inside_string(const std::string& path, const char** c);

		/**
		Check for error during shader creation
		*/
		U8 check_create_errors(ShadersIndex index);

		/**
		Check for error during shader compilation
		*/
		U8 check_compile_errors(ShadersIndex index);

		/**
		Check for error during program linking
		*/
		U8 check_link_errors();


		//THE FOLLOWING STUFF IS FOR GETTING ATTRIBUTES FROM THE GPU PROGRAM

	
		//Temporary struct to sort
		static struct attribute{
			attribute(U32 _i, U32 _o, U32 _s, C* _n) : index(_i), offset(_o), size(_s), name(_n){}
			U32 index, offset, size;
			C* name;
		};


	};
}