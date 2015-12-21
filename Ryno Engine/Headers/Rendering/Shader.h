#pragma once

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Global.h"
#include <array>
#include <unordered_map>

namespace Ryno{

	//These are used to build the struct in the material class.
	//Basically each material creates it based on these parameters
	struct instance_attributes{
		U32 index;		//Index of the attribute
		U32 offset;		//Memory offset of the attribute in the struct
		U32 size;		//Size of the current attribute
		U32 type;
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

		void generate_attributes();

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

		//Instance attributes data:
		std::unordered_map<std::string,instance_attributes> attributes;


		//Temporary struct to sort
		static struct attribute{
			attribute(U32 _i, U32 _o, U32 _s, GLenum _t, C* _n) : index(_i), offset(_o), size(_s), type(_t), name(_n){}
			U32 index, offset, size;
			GLenum type;
			C* name;
		};

		U8 get_attribute_size(const C* name);

	};
}