#include "Batch3D.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {


	/*Mesh::Mesh(const std::vector <Vertex3D>& verticex, GLuint t)

	{
		texture = t;
		for (I32 i = 0; i< vertices.size(); i++)
			vertices.emplace_back(verticex[i]);
	}*/


	void Batch3D::init() {
		create_vertex_array();

	}




	void Batch3D::begin() {

		m_render_batches.clear();
		for (Model m : m_models)
			m.vertices.clear();
		m_models.clear();



	}
	void Batch3D::end() {

		//Create the vector of mesh pointers
		m_models_pointers.resize(m_models.size());

		//Fill the vector of mesh pointers
		for (I32 i = 0; i < m_models.size(); i++){
			m_models_pointers[i] = &m_models[i];
		}

		//Sort with provided compare function
		std::stable_sort(m_models_pointers.begin(), m_models_pointers.end(), compare_texture);

		//Create batches
		create_render_batches();
	}

	void Batch3D::draw(const Model& model) {

		m_models.push_back(model);

	}




	void Batch3D::create_render_batches(){

		//Create vertices vector to send to gpu
		std::vector<Vertex3D> vertices;
		I32 size = 0;

		//Resize the vector at the beginning to avoid reallocations
		for (Model* m : m_models_pointers)
			size += (I32) m->vertices.size();
		vertices.resize(size); 

		//Return if no mesh
		if (m_models_pointers.empty())
			return;

		U32 cv = 0;
		U32 offset = 0;

		//For each mesh...
		for (I32 cg = 0; cg < m_models.size(); cg++){

			I32 mesh_size = (I32) m_models_pointers[cg]->vertices.size();

			//If a mesh has a different texture than the one before, i create a new batch
			if (cg == 0 || m_models_pointers[cg]->texture.id != m_models_pointers[cg - 1]->texture.id){
				m_render_batches.emplace_back(offset, mesh_size, m_models_pointers[cg]->texture.id);
			}
			else
			{
				m_render_batches.back().num_vertices += mesh_size;
			}

			//Adds vertices to the final array.
			//Not related to the render batches, but we can do it here
			for (I32 i = 0; i < mesh_size; i++)
				vertices[cv++] = m_models_pointers[cg]->vertices[i];

			offset += mesh_size;

		}

		//Once i have the full vector of vertices (unrelated with the batches though)
		//i can bind the vbo, orphan it, pass the new data, and unbind it.
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glActiveTexture(GL_TEXTURE0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex3D), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Batch3D::create_vertex_array(){

		//Create vao
		if (!m_vao)
			glGenVertexArrays(1, &m_vao);

		//Bind vao
		glBindVertexArray(m_vao);

		//Create vbo
		if (!m_vbo)
			glGenBuffers(1, &m_vbo);


		//temporally bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//Tell vbo how to use the data it will receive
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, color));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));

		//Enable the attrib arrays
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		//unbind vao and vbo
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		



	}

	void Batch3D::render_batch() {
		

		for (I32 i = 0; i < m_render_batches.size(); i++){

			glBindTexture(GL_TEXTURE_2D, m_render_batches[i].texture);
			glDrawArrays(GL_TRIANGLES, m_render_batches[i].offset, m_render_batches[i].num_vertices);

		}


	}

	U8 Batch3D::compare_texture(Model* a, Model* b){
		return a->texture.id < b->texture.id;

	}


}



