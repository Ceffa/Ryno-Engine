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


	void Batch3D::init(Camera3D* camera) {
		m_camera = camera;
		create_vertex_array();

	}




	void Batch3D::begin() {

		m_render_batches.clear();
		
		m_models.clear();



	}
	void Batch3D::end() {

	
		//Sort with provided compare function
		std::stable_sort(m_models.begin(), m_models.end(), compare_texture);

		//generate camera matrix
		m_camera->generate_camera_matrix();

		//Create batches
		create_render_batches();
	}

	void Batch3D::draw(Model* model) {

		m_models.push_back(model);

	}




	void Batch3D::create_render_batches(){

		//Create vertices vector to send to gpu
		std::vector<Vertex3D> vertices;
		std::vector<glm::mat4> mvp_instances;

		I32 size = 0;
		I32 models_size = m_models.size();

		//Resize the vector at the beginning to avoid reallocations
		for (Model* m : m_models)
			size += (I32) m->mesh.vertices.size();
		vertices.resize(size); 
		mvp_instances.resize(models_size);

		//Adds MVP to the final instance array.
		//One for each instance. Needed for instancing only
		for (I32 i = 0; i < models_size; i++){
			mvp_instances[i] = m_camera->camera_matrix *  m_models[i]->model_matrix;
		}
		

		//Return if no mesh
		if (m_models.empty())
			return;

		U32 cv = 0;
		U32 offset = 0;

		//For each mesh...
		for (I32 cg = 0; cg < m_models.size(); cg++){

			I32 mesh_size = (I32) m_models[cg]->mesh.vertices.size();

			//If a mesh has a different texture than the one before, i create a new batch
			if (cg == 0 || m_models[cg]->texture.id != m_models[cg - 1]->texture.id){
				m_render_batches.emplace_back(offset, mesh_size, m_models[cg]->texture.id);
			}
			else
			{
				m_render_batches.back().num_vertices += mesh_size;
			}

			//Adds vertices to the final array.
			//One for each vertex
			for (I32 i = 0; i < mesh_size; i++)
				vertices[cv++] = m_models[cg]->mesh.vertices[i];

			
			



			offset += mesh_size;

		}

		//Once i have the full vector of vertices (unrelated with the batches though)
		//i can bind the vbo, orphan it, pass the new data, and unbind it.
		glActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex3D), vertices.data());
		//Same with instance buffer, but i'm not sure
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		glBufferData(GL_ARRAY_BUFFER, mvp_instances.size() * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mvp_instances.size() * sizeof(glm::mat4), mvp_instances.data());
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

		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);


		
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 4 * 4, (void*)(0));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 4 * 4, (void*)(sizeof(F32) * 4));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 4 * 4, (void*)(sizeof(F32) * 8));
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 4 * 4, (void*)(sizeof(F32) * 12));
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		



	}

	void Batch3D::render_batch() {
		

		for (I32 i = 0; i < m_render_batches.size(); i++){
			
			glBindTexture(GL_TEXTURE_2D, m_render_batches[i].texture);
			//glDrawArrays(GL_TRIANGLES, m_render_batches[i].offset, m_render_batches[i].num_vertices);
			glDrawArraysInstanced(GL_TRIANGLES,0,36,m_render_batches[i].num_vertices/36);
		}


	}

	U8 Batch3D::compare_texture(Model* a, Model* b){
		return a->texture.id < b->texture.id;

	}


}



