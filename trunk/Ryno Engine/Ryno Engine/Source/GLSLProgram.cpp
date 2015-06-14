#include "GLSLProgram.h"
#include "ErrorManager.h"
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>


void GLSLProgram::init(){
	m_program_id=0;
	m_attr_no=0;
	m_shader_ids[VERT] = 0;
	m_shader_ids[FRAG] = 0;
	m_program_id = glCreateProgram();
}

void GLSLProgram::destroy(){
	glDetachShader(m_program_id,m_shader_ids[VERT]);
	glDetachShader(m_program_id, m_shader_ids[FRAG]);
	glDeleteProgram(m_program_id);
	glDeleteShader(m_shader_ids[VERT]);
	glDeleteShader(m_shader_ids[FRAG]);
}
void GLSLProgram::load_shaders(const std::string& name){
	shader_name = name;
	load_shader(VERT, "Resources/Shaders/" + name + ".vert");
	load_shader(FRAG, "Resources/Shaders/"+ name + ".frag");
}

void GLSLProgram::load_shader(ShadersIndex index, const std::string& path){

	m_shader_ids[index] = glCreateShader(index == VERT ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

	if (check_create_errors(index) < 0) {
		destroy();
		return;
	}

	const C* content;
	read_file_inside_string(path, &content);

	//send to GL the shader and keep track of id
	glShaderSource(m_shader_ids[index], 1, &content, NULL);

}



void GLSLProgram::compile_shaders(){
	compile_shader(VERT);
	compile_shader(FRAG);
}

void GLSLProgram::compile_shader(ShadersIndex index){

	glCompileShader(m_shader_ids[index]);
	if (check_compile_errors(index) < 0) {
		destroy();
		return;
	}
}

void GLSLProgram::bind_attributes(){
	std::string path = "Resources/Shaders/" + shader_name + ".meta";

	std::ifstream meta_file;
	meta_file.open(path,std::ifstream::in);

	std::string s;
	while (1){
		std::getline(meta_file, s);
		if (s.length()== 0)
			break;
		
		add_attribute(s.c_str());
	}
	meta_file.close();
}

void GLSLProgram::add_attribute(const C* attributeName){

	glBindAttribLocation(m_program_id, m_attr_no++, attributeName);
}
void GLSLProgram::link_shaders(){

	glAttachShader(m_program_id, m_shader_ids[VERT]);
	glAttachShader(m_program_id, m_shader_ids[FRAG]);
	glLinkProgram(m_program_id);
	if(check_link_errors() < 0) {
		destroy();
		return;
	}

}



GLint GLSLProgram::getUniformLocation(const std::string& uniformName){
	GLint location = glGetUniformLocation(m_program_id, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
		ErrorManager::Error("Uniform " + uniformName + " not found in shader.");
	return location;
}

void GLSLProgram::use(){
	glUseProgram(m_program_id);
	for (int i = 0; i< m_attr_no; i++){
		glEnableVertexAttribArray(i);
	}
}
void GLSLProgram::unuse(){
	glUseProgram(0);
	for (int i = 0; i< m_attr_no; i++){
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::read_file_inside_string(const std::string&  path, const char** c){

	std::ifstream file(path, std::ios::in);
	file.seekg(0, std::ios::end);
	I32 size = file.tellg();
	file.seekg(0, std::ios::beg);
	C* buffer = (C*) calloc(size,1);
	file.read(buffer, size+1);
	buffer[size] = '\0';
	if (!buffer)
		ErrorManager::Error("Failed to load from file: " + path);
	*c = buffer;

	
}

U8 GLSLProgram::check_create_errors(ShadersIndex index){

	//simply check if glShaderSource returns a shader id
	if (!m_shader_ids[index]){
		std::string fatal_error_mex = (index == FRAG ? "Fragment" : "Vertex");
		fatal_error_mex += " Shader Failed to be created.";
		ErrorManager::Error(fatal_error_mex.c_str());
		return -1;
	}
	return 0;
}

U8 GLSLProgram::check_compile_errors(ShadersIndex index){

	I32 is_compiled, max_length;
	C* log;

	glGetShaderiv(m_shader_ids[index], GL_COMPILE_STATUS, &is_compiled);
	if (is_compiled == GL_FALSE)
	{
		glGetShaderiv(m_shader_ids[index], GL_INFO_LOG_LENGTH, &max_length);

		/* The maxLength includes the NULL character */
		log = (C*)malloc(max_length);

		glGetShaderInfoLog(m_shader_ids[index], max_length, &max_length, log);

		ErrorManager::Error("Problem compiling " + (index == VERT) ? "Vertex shader\n" : "Fragment shader\n",log);
		free(log);
		return -1;
	}
	return 0;
}



U8 GLSLProgram::check_link_errors(){
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

		ErrorManager::Error("Problem linking program\n" ,log);
		free(log);
		return -1;
	}
	return 0;
}
