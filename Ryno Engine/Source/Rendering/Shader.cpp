#include "Shader.h"
#include "Log.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>



namespace Ryno{


	void Shader::create(const std::string& name, bool vert, bool geom, bool frag){
		is_shader_present[VERT] = vert;
		is_shader_present[GEOM] = geom;
		is_shader_present[FRAG] = frag;
		init();
		load_shaders(name,ENGINE);
		compile_shaders();
		link_shaders();
		get_attributes();
		get_uniforms();
	}
	void Shader::init(){
		m_program_id = 0;
		m_attr_no = 0;
	
		m_shader_ids[VERT] = 0;
		m_shader_ids[GEOM] = 0;
		m_shader_ids[FRAG] = 0;

		m_program_id = glCreateProgram();
	}

	void Shader::destroy(){

		for (U8 i = VERT; i < VERT + 3; i++){
			if (is_shader_present[i]){
				glDetachShader(m_program_id, m_shader_ids[i]);
				glDeleteShader(m_shader_ids[i]);
			}
		}
		glDeleteProgram(m_program_id);

	}

	

	void Shader::load_shaders(const std::string& name, Owner loc){
		shader_name = name;
		static std::string extensions[3]{".vert", ".geom", ".frag"};
		static const std::string middle_path = "Resources/Shaders/";


		for (U8 i = VERT; i < VERT + 3; i++){
			if (is_shader_present[i])
				load_shader((ShadersIndex)i , BASE_PATHS[loc] + middle_path + name + extensions[i-VERT]);
		}
		
	}

	void Shader::load_shader(ShadersIndex index, const std::string& path){

		switch (index){
		case VERT:	m_shader_ids[VERT] = glCreateShader(GL_VERTEX_SHADER); break;
		case GEOM:	m_shader_ids[GEOM] = glCreateShader(GL_GEOMETRY_SHADER); break;
		case FRAG:	m_shader_ids[FRAG] = glCreateShader(GL_FRAGMENT_SHADER); break;
		}

		if (check_create_errors(index) < 0) {
			destroy();
			return;
		}

		const C* content;
		read_file_inside_string(path, &content);

		//send to GL the shader and keep track of id
		glShaderSource(m_shader_ids[index], 1, &content, NULL);

	}



	void Shader::compile_shaders(){
		for (U8 i = VERT; i < VERT + 3; i++){
			if (is_shader_present[i])
				compile_shader((ShadersIndex) i);
		}
	}

	void Shader::compile_shader(ShadersIndex index){

		glCompileShader(m_shader_ids[index]);
		if (check_compile_errors(index) < 0) {
			destroy();
			return;
		}
	}



	
	void Shader::link_shaders(){

		for (U8 i = VERT; i < VERT + 3; i++)
			if (is_shader_present[i])
				glAttachShader(m_program_id, m_shader_ids[i]);

		glLinkProgram(m_program_id);
		if (check_link_errors() < 0) {
			destroy();
			return;
		}

	}

	

	
	GLint Shader::getUniformLocation(const std::string& uniformName){
		GLint location = glGetUniformLocation(m_program_id, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
			std::cout << "Uniform " + uniformName + " not found in shader." << std::endl;
		return location;
	}

	void Shader::use(){
		glUseProgram(m_program_id);
		

	}
	void Shader::unuse(){
		glUseProgram(0);
		
	}

	void Shader::read_file_inside_string(const std::string&  path, const char** c){

		std::ifstream file(path, std::ios::in);
		file.seekg(0, std::ios::end);
		I32 size = (I32)file.tellg();
		
		file.seekg(0, std::ios::beg);
		C* buffer = (C*)calloc(size, 1);
		file.read(buffer, size + 1);
		buffer[size] = '\0';
		if (!buffer)
			std::cout << "Failed to load from file: " + path << std::endl;
		*c = buffer;


	}

	U8 Shader::check_create_errors(ShadersIndex index){

		//simply check if glShaderSource returns a shader id
		if (!m_shader_ids[index]){
		
			std::cout << "Shader Failed to be created." << std::endl;
			return -1;
		}
		return 0;
	}

	U8 Shader::check_compile_errors(ShadersIndex index){

		I32 is_compiled, max_length;
		C* log;

		glGetShaderiv(m_shader_ids[index], GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE)
		{
			glGetShaderiv(m_shader_ids[index], GL_INFO_LOG_LENGTH, &max_length);

			/* The maxLength includes the NULL character */
			log = (C*)malloc(max_length);

			glGetShaderInfoLog(m_shader_ids[index], max_length, &max_length, log);

			std::cout << "Problem compiling shader.\n" + std::string(log) << std::endl;
			free(log);
			return -1;
		}
		return 0;
	}



	U8 Shader::check_link_errors(){
		I32 is_linked, max_length;
		C* log;
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE)
		{
			/* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
			glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &max_length);

			/* The maxLength includes the NULL character */
			log = (C *)malloc(max_length);

			/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
			glGetProgramInfoLog(m_program_id, max_length, &max_length, log);

			std::cout << "Problem linking program.\n" + std::string(log) << std::endl;
			free(log);
			return -1;
		}
		return 0;
	}

	










	void Shader::get_attributes()
	{
		GLint attr_num;
		glGetProgramiv(m_program_id, GL_ACTIVE_ATTRIBUTES, &attr_num);
		//Temp arrays. They contain only instances attributes, not the vertex ones
		U8 max_name_char = 30;
		const int _attr_num = attr_num;
		std::list<attribute*> temp_attribs;
		for (GLuint i = 0; i < attr_num; i++){
			//Temp return values
			GLint temp_size, temp_name_size;
			GLenum temp_type;
			GLchar* temp_name = (GLchar*)malloc(30);

			glGetActiveAttrib(m_program_id, i, 30, &temp_name_size, &temp_size, &temp_type, temp_name);
			GLuint loc = glGetAttribLocation(m_program_id, temp_name);
			//keep only instance data, not vertex attributes.
			//They are the one with the divisor. For now I'll assume they start from 4
			if (loc > 3)
				temp_attribs.push_back(new attribute(loc, 0,  sizeof(U32)*get_size_from_type(temp_type) * temp_size, temp_name));
			else
				free(temp_name);
		}

		std::stable_sort(temp_attribs.begin(), temp_attribs.end(), [](attribute*a, attribute*b){return a->index < b->index; });

		
		for (auto _old : temp_attribs){
			auto _new = &attributes_map[_old->name];
			_new->index = _old->index;
			_new->offset = attributes_struct_size;
			_new->size = _old->size;
			attributes_struct_size += _old->size;
			delete _old;
		}
		temp_attribs.clear();
	}


	void Shader::get_uniforms()
	{
		GLint unif_num;
		glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &unif_num);
		//Temp arrays. They contain only instances attributes, not the vertex ones
		U8 max_name_char = 30;
		for (GLuint i = 0; i < unif_num; i++){
			//Temp return values
			GLint temp_size, temp_name_size;
			GLenum temp_type;
			GLchar* temp_name = (GLchar*)malloc(30);

			glGetActiveUniform(m_program_id, i, 30, &temp_name_size, &temp_size, &temp_type, temp_name);
			GLuint loc = glGetUniformLocation(m_program_id, temp_name);
			//keep only instance data, not vertex attributes
			uniforms_map[temp_name].index = loc;
			uniforms_map[temp_name].size = get_size_from_type(temp_type);
			uniforms_map[temp_name].type = temp_type;

		}

	}


	U8 Shader::get_size_from_type(const GLenum type)
	{

		switch (type)
		{
		case GL_FLOAT:
		case GL_INT:
		case GL_UNSIGNED_INT:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
		case GL_SAMPLER_2D_SHADOW:
			return 1; 
		case GL_FLOAT_VEC2:
		case GL_INT_VEC2:
		case GL_UNSIGNED_INT_VEC2:
			return 2;
		case GL_FLOAT_VEC3:
		case GL_INT_VEC3:
		case GL_UNSIGNED_INT_VEC3:
			return 3;
		case GL_FLOAT_VEC4:
		case GL_INT_VEC4:
		case GL_UNSIGNED_INT_VEC4:
			return 4;
		case GL_FLOAT_MAT4:
			return 16;

		}
		return 0;


	}

	void Shader::setup_vbo_attributes()
	{
		//This does the binding for the vbo.
		
		//For each attribute
		for (auto a : attributes_map){
			U32 temp_size = 0;
			U32 index = a.second.index;

			//If the attribute is a matrix, multiple lines must be added.
			//This requires to autoincrement the index, the offset and so on.
			//Very ugly code but very straightforward
			while (temp_size < a.second.size){
				U32 how_many = a.second.size - temp_size;
				if (how_many > 4*sizeof(U32)) how_many = 4 * sizeof(U32); //Max line size
				glVertexAttribIPointer(index, how_many/sizeof(U32), GL_UNSIGNED_INT, attributes_struct_size, (void*)(how_many*(index - a.second.index) +a.second.offset));
				temp_size += how_many;
				glVertexAttribDivisor(index, 1);
				glEnableVertexAttribArray(index);

				index++;
			}
		}
	}

	

	I8 Shader::compare_uniforms(void* a, void* b)
	{
		U64 first = (U64)a;
		U64 second = (U64)b;
		if (a == b) return 0;
		if (a < b) return -1;
		if (a > b) return 1;

		return 0;
	}

	bool Shader::is_sampler(GLenum type)
	{
		if (type == GL_SAMPLER_2D || type == GL_SAMPLER_3D || type == GL_SAMPLER_CUBE || type == GL_SAMPLER_2D_SHADOW)
			return true;
		return false;
	}

}