#include "Shader.h"
#include "Log.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>


namespace Ryno {

	bool file_exists(const std::string& fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}

	std::vector<std::pair<std::string, Owner>> Shader::empty_list;


	//Holds info about the vertex 3D data
	std::map<std::string, static_vertex_attr> Shader::vertex_3d_map{
			{ "in_Position", static_vertex_attr(3, (void*)offsetof(Vertex3D, position)) },
			{ "in_Uv", static_vertex_attr(2, (void*)offsetof(Vertex3D, uv)) },
			{ "in_Tangent", static_vertex_attr(3, (void*)offsetof(Vertex3D, tangent)) },
			{ "in_Normal", static_vertex_attr(3, (void*)offsetof(Vertex3D, normal)) }
	};
	I32 vertex_3d_locations[4];

	bool Shader::is_vertex_attribute(const std::string& name)
	{
		//Check if an attribute is a vertex one by scanning the
		//map that holds vertex 3d informations
		if (vertex_3d_map.find(name) != vertex_3d_map.end())
			return true;
		return false;
	}


	void Shader::create(const std::string & name, Owner location)
	{
		init();
		load_shaders(name, location);
		compile_shaders();
		link_shaders();
		get_attributes();
		get_uniforms();
	}
	void Shader::init() {
		m_program_id = 0;
		m_attr_no = 0;

		m_shader_ids[VERT] = 0;
		m_shader_ids[GEOM] = 0;
		m_shader_ids[FRAG] = 0;
		m_shader_ids[COMP] = 0;

		m_program_id = glCreateProgram();

	}

	void Shader::destroy() {

		for (U8 i = VERT; i < VERT + 4; i++) {
			if (is_shader_present[i]) {
				glDetachShader(m_program_id, m_shader_ids[i]);
				glDeleteShader(m_shader_ids[i]);
			}
		}
		glDeleteProgram(m_program_id);
	}





	void Shader::load_shaders(const std::string& name, Owner loc) {
		shader_name = name;
		static std::string extensions[4]{ ".vert", ".geom", ".frag", ".comp" };
		static const std::string middle_path = "Resources/Shaders/";

			
		for (U8 i = VERT; i < VERT + 4; i++){
			std::string path_and_name = BASE_PATHS[loc] + middle_path + name + extensions[i - VERT];
			is_shader_present[i] = file_exists(path_and_name);
			if (is_shader_present[i])
				load_shader((ShadersIndex)i , path_and_name);
		}
		
	}


	void Shader::load_shader(ShadersIndex index, const std::string& path){

		switch (index){
		case VERT:	m_shader_ids[VERT] = glCreateShader(GL_VERTEX_SHADER); break;
		case GEOM:	m_shader_ids[GEOM] = glCreateShader(GL_GEOMETRY_SHADER); break;
		case FRAG:	m_shader_ids[FRAG] = glCreateShader(GL_FRAGMENT_SHADER); break;
		case COMP:	m_shader_ids[COMP] = glCreateShader(GL_COMPUTE_SHADER); break;
		}

		if (check_create_errors(index) < 0) {
			destroy();
			return;
		}

		//Read file
		std::string content = read_file_inside_string(path);
		U32 size = content.size();

		//Parser of preprocessor includes
		static std::string include_extension = ".glinc";
		static const std::string middle_path = "Resources/Shaders/";
		enum States{SEARCHING, READING, BREAK};
		States state = SEARCHING;
		std::vector<std::string> includes;
		int startPath = 0;
		int startFile = 0;
		Owner owner;
		

		for (int i = 0; i < size; ++i) {
			if (state == BREAK)
				break;
			char cc = content.at(i);
			switch (state) {
			case SEARCHING:
				if (i + 2 >= size) {
					state = BREAK;
					break;
				}
				if ((cc == 'E' || cc == 'G') && content.at(i+1) == '(')
				{
					owner = cc == 'E' ? ENGINE : GAME;
					startPath = i + 2;
					state = READING;
				}
				else if (cc != ' ' && cc != '\n' && cc!= '\t' && cc != '\r') {
					state = BREAK;
				}
				break;
		
			case READING:
				if (content.at(i) == ')') {
					includes.push_back(BASE_PATHS[owner] + middle_path + content.substr(startPath,i-startPath) + include_extension);
					state = SEARCHING;
					startFile = i + 1;
				}
				break;
			}
		}

		std::string finalProgram;
		for (int i = 0; i < includes.size(); i++) {
			finalProgram += read_file_inside_string(includes[i]) + "\n";
		};

		finalProgram += content.substr(startFile);
		const char* cstr = finalProgram.c_str();
		
	
		//send to GL the shader and keep track of id
		glShaderSource(m_shader_ids[index], 1, &cstr, NULL);

	}



	void Shader::compile_shaders(){
		for (U8 i = VERT; i < VERT + 4; i++){
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

		for (U8 i = VERT; i < VERT + 4; i++)
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

	std::string Shader::read_file_inside_string(const std::string&  path){

		std::ifstream file(path, std::ios::in);
		file.seekg(0, std::ios::end);
		I32 size = (I32)file.tellg();
		
		file.seekg(0, std::ios::beg);
		C* buffer = (C*)calloc(size, 1);
		file.read(buffer, size + 1);
		buffer[size] = '\0';
		if (!buffer)
			std::cout << "Failed to load from file: " + path << std::endl;

		//Detect and fix BOM
		if (buffer[0] == '\xEF' && buffer[1] == '\xBB' && buffer[2] == '\xBF') {
			buffer[0] = ' '; buffer[1] = ' '; buffer[2] = ' ';
		}
		return std::string(buffer);


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
		U8 max_name_char = 50;
		const int _attr_num = attr_num;
		std::list<attribute*> temp_attribs;
		for (GLuint i = 0; i < attr_num; i++){
			//Temp return values
			GLint temp_size, temp_name_size;
			GLenum temp_type;
			GLchar* temp_name = (GLchar*)malloc(max_name_char);

			glGetActiveAttrib(m_program_id, i, 50, &temp_name_size, &temp_size, &temp_type, temp_name);
			GLuint loc = glGetAttribLocation(m_program_id, temp_name);
			//keep only instance data, not vertex attributes.
			//They are the one with the divisor. For now I'll assume they start from 4
			if (is_vertex_attribute(temp_name)){
				vertex_3d_locations[temp_name].loc = loc;
				vertex_3d_locations[temp_name].nr = vertex_3d_map[temp_name].nr;
				vertex_3d_locations[temp_name].offset = vertex_3d_map[temp_name].offset;
			}
			else
				temp_attribs.push_back(new attribute(loc, 0, sizeof(U32)*get_size_from_type(temp_type) * temp_size, temp_name));

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
		const U8 max_name_char = 50;
		U32 offset = 0;
		for (GLuint i = 0; i < unif_num; i++){
			//Temp return values
			GLint temp_size, temp_name_size;
			GLenum temp_type;
			GLchar* temp_name = (GLchar*)malloc(max_name_char);

			glGetActiveUniform(m_program_id, i, max_name_char, &temp_name_size, &temp_size, &temp_type, temp_name);
			int loc = glGetUniformLocation(m_program_id, temp_name);
			if (loc < 0)
				continue;
			uniforms_map[temp_name].index = loc;
			uniforms_map[temp_name].offset = offset;
			uniforms_map[temp_name].type = temp_type;
			auto actualSize = sizeof(U32) * get_size_from_type(temp_type) * temp_size;
			uniforms_map[temp_name].size = actualSize;
			offset += actualSize;	
		}
		uniforms_map_size = offset;

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

	

	

	bool Shader::is_sampler(GLenum type, I32* type_of_texture, bool* computeTexture)
	{
		if (type == GL_IMAGE_2D) {
			*computeTexture = true;
		}
		else {
			*computeTexture = false;
		}
		
		if (type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_SHADOW || type == GL_IMAGE_2D){
			*type_of_texture = GL_TEXTURE_2D;
			return true;
		}
		if (type == GL_SAMPLER_CUBE || type == GL_SAMPLER_CUBE_SHADOW){
			*type_of_texture = GL_TEXTURE_CUBE_MAP;
			return true;
		}
		if (type == GL_SAMPLER_3D){
			*type_of_texture = GL_TEXTURE_3D;
			return true;
		}
		return false;
	}

	

	void Shader::send_uniform_to_shader(const std::string& name, void* value,U8* sampler_index)
	{
		if (value == nullptr) {
			std::cout << "Shader: value required by shader not found in local map: " << name << std::endl;
			//exit(-1);
		}
		I32 type_of_texture;
		bool isComputeTex;
		auto& map = uniforms_map[name];
		if (Shader::is_sampler(map.type, &type_of_texture, &isComputeTex)) {
			glActiveTexture(GL_TEXTURE0 + *sampler_index);

			glBindTexture(type_of_texture, *(U32*)value);

			if (isComputeTex) {
				glBindImageTexture(0, *(U32*)value, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
			}
			else {
				glUniform1i(map.index, *sampler_index);
				*sampler_index = *sampler_index + 1;
			}
			return;
		}

		switch (map.type) {
		case GL_INT:
			glUniform1i(map.index, *(I32*)value);
			break;
		case GL_UNSIGNED_INT:
			glUniform1ui(map.index, *(U32*)value);
			break;
		case GL_FLOAT:
			glUniform1f(map.index, *(F32*)value);
			break;
			//NEEDS A REFACTORING, USE glUniform3iv 
		case GL_INT_VEC3:
			glUniform3iv(map.index, 1, (I32*)value);
			break;
		case GL_UNSIGNED_INT_VEC3:
			glUniform3uiv(map.index, 1, (U32*)value);
			break;
		case GL_FLOAT_VEC3:
			glUniform3fv(map.index, 1, (F32*)value);
			break;

		case GL_INT_VEC4:
			glUniform4iv(map.index, 1, (I32*)value);
			break;
		case GL_UNSIGNED_INT_VEC4:
			glUniform4uiv(map.index, 1, (U32*)value);
			break;
		case GL_FLOAT_VEC4:
			glUniform4fv(map.index, 1, (F32*)value);
			break;

		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(map.index, 1, GL_FALSE, (F32*)value);
			break;
		default:
			std::cout << "Shader " << name << ": uniform type not found. ";
			std::cout << "If possible add it to the switch in the \"send_uniform_to_shader\" function in the shader class" << std::endl;
			//exit(-1);
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
		case GL_IMAGE_2D:
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
}