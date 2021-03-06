#pragma once

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Global.h"
#include <array>
#include <map>
#include <vector>

namespace Ryno{

	//Info about a vertex 3D (static)
	struct static_vertex_attr{
		static_vertex_attr(){}
		static_vertex_attr(U32 _nr, void* off) : nr(_nr), offset(off){}
		U32 nr;
		void* offset;
	};

	//Info about a vertex 3D (of the single shader)
	struct vertex_attr{
		vertex_attr() : loc(-1){}
		vertex_attr(I32 loc, U32 _nr, void* off) : nr(_nr), offset(off){}
		I32 loc = -1;
		U32 nr;
		void* offset;
	};

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
		U32 index;
		U32 offset;		
		U32 type;
		U32 size;
	};
	
	class Shader
	{

	public:

		Shader() {}
		Shader(const std::string& name, Owner location) { create(name, location); }
		~Shader(){ destroy(); }

		/**
		This function create a new GLSLProgram, it loads the shaders and everything else.
		After this call, the GLSLProgram could be used.
		Use init, load_shaders, compile_shaders and link_shaders to
		separate these steps.
		@Param name of the shader
		The last vector provides optional include libraries
		*/
		static std::vector<std::pair<std::string, Owner>> empty_list;
		void create(const std::string& name, Owner location);


		

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
		std::map<std::string, uniforms> uniforms_map;
		U32 uniforms_map_size = 0;
		
		
		//This holds info about the vertex 3d struct.
		//It is manually created, and it is shared among the shaders.
		//Each shader will then keep track of the attributes it has and their locations
		//with the vertex_3d_locations array below.
		static std::map<std::string, static_vertex_attr> vertex_3d_map;
		std::map<std::string, vertex_attr> vertex_3d_locations;
		bool is_vertex_attribute(const std::string& name);


		//Check if a uniform is a sampler
		static bool is_sampler(GLenum type, I32* type_of_texture, bool* computeTexture);

		//send uniform to shader. Depending on its type (matrix, int etc)
		//the template function send_uniform_to_shader is called with a different argument
		void send_uniform_to_shader(const std::string& name, void* value, U8* sampler_index);


		U8 get_size_from_type(const GLenum type);

		U32 get_id()const { return m_program_id; }
		std::string name;
	private:

		//local enum to differentiate the shader type
		enum  ShadersIndex{ VERT, GEOM, FRAG, COMP };

		bool is_shader_present[4];

		//local enum to identify the different operations on a shader
		enum  OperationIndex{ CREATE, COMPILE, LINK };

		//number of attributes input to vert shader
		U32 m_attr_no = 0;
		//id of the glsl program
		U32 m_program_id = 0;
		//the ids of the shaders
		U32 m_shader_ids[4];
		

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
		std::string read_file_inside_string(const std::string& path);
		std::string read_file_inside_string_recursive(const std::string& path);

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