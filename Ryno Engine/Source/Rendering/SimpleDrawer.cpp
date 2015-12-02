#include "SimpleDrawer.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {

	SimpleDrawer::SimpleDrawer(){
		m_vao = 0;
		m_vbo = 0;
		m_mesh_manager = MeshManager::get_instance();
		create_vertex_array();
	}

	SimpleDrawer* SimpleDrawer::get_instance(){

		static SimpleDrawer instance;//only at the beginning
		return &instance;
	}
	

	void SimpleDrawer::draw(Model* model) {
		Mesh* m = m_mesh_manager->get_mesh(model->mesh);
		glBindVertexArray(m_vao);

		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m->vertices_number * sizeof(Vertex3D), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m->vertices_number * sizeof(Vertex3D), m_mesh_manager->get_mesh(model->mesh)->vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->indices_number * sizeof(U16), &m->indices[0], GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, m->indices_number, GL_UNSIGNED_SHORT, (void*)0);
	
	}

	void SimpleDrawer::create_vertex_array(){

		//Create vao
		if (!m_vao){
			glGenVertexArrays(1, &m_vao);
		}
		//Bind vao
		glBindVertexArray(m_vao);

		//Create vbo
		if (!m_vbo)
			glGenBuffers(1, &m_vbo);


		//temporally bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//Tell vbo how to use the data it will receive
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
	

		//Enable the attrib arrays
		glEnableVertexAttribArray(0);

		if (!m_index_vbo)
			glGenBuffers(1, &m_index_vbo);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		


	}

	

}



