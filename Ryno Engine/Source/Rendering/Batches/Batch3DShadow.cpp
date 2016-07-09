#include "Batches/Batch3DShadow.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace RynoEngine {

	bool Batch3DShadow::sorting = true;

	void Batch3DShadow::init(Camera3D* camera) {
		set_camera(camera);
		create_vertex_array();
		m_mesh_manager = MeshManager::get_instance();
		indices.resize(0);

	}
	void Batch3DShadow::set_camera(Camera3D* camera) {
		m_camera = camera;
	}

	void Batch3DShadow::begin() {

		m_render_batches.clear();
		models_matrices.clear();
		m_models.clear();
	
	}
	void Batch3DShadow::end() {

	
		//Sort with provided compare function
		if (sorting)
			std::stable_sort(m_models.begin(), m_models.end(), compare_models);


		//Create batches
		create_render_batches();
	}

	void Batch3DShadow::draw(Model* m) {

		for (auto& s : m->sub_models) {
			//discard light-emitting models
			if (s.cast_shadows) {
				m_models.emplace_back(&s, &m->game_object->transform.model_matrix);
			}
		}
	}




	void Batch3DShadow::create_render_batches(){

	
		I32 models_size = (I32) m_models.size();

		//Return if no mesh
		if (models_size==0)
			return;

		//Resize the MVP vector at the beginning to avoid reallocations
		models_matrices.resize(models_size);

		//Adds MVP to the final instance array.
		//One for each instance. 
		for (I32 i = 0; i < models_size; i++){

			models_matrices[i] = *m_models[i].matrix;


		}

		U32 indices_offset = 0;
		U32 vertex_offset = 0;
		U32 instance_offset = 0;
		

		//For each mesh...
		for (I32 cg = 0; cg < m_models.size(); cg++){

			auto* new_model = m_models[cg].model;

			//If a mesh has a different texture or mesh than the one before, i create a new batch
			if (cg == 0
				|| new_model->mesh != m_models[cg - 1].model->mesh)
			{
				if (cg != 0){
					indices_offset += m_render_batches.back().num_indices;
					vertex_offset += m_render_batches.back().num_vertices;
					instance_offset += m_render_batches.back().num_instances;
				}
				Mesh* temp_mesh = m_mesh_manager->get_mesh(new_model->mesh);
				I32 num_indices = temp_mesh->indices_number;
				I32 num_vertices = temp_mesh->vertices_number;


				m_render_batches.emplace_back(vertex_offset, num_vertices, indices_offset, num_indices, instance_offset, 1, new_model);

				
			}
			else
			{
				//else, i increment the number of instances
				m_render_batches.back().num_instances ++;
			}

		
		}
		I32 total_vertices = m_render_batches.back().vertex_offset + m_render_batches.back().num_vertices;
		I32 cv = 0;
		vertices_positions.resize(total_vertices);
		for (RenderBatchShadow rb : m_render_batches){
			for (Vertex3D v : m_mesh_manager->get_mesh(rb.model->mesh)->vertices){
				vertices_positions[cv++] = v.position;
			}
		}

		I32 total_indices = m_render_batches.back().indices_offset + m_render_batches.back().num_indices;
		cv = 0;
		indices.resize(total_indices);
		for (RenderBatchShadow rb : m_render_batches){
			for (U16 v : m_mesh_manager->get_mesh(rb.model->mesh)->indices){
				indices[cv++] = v;
			}
		}
		

	}

	void Batch3DShadow::create_vertex_array(){
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);


		
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		
		auto size = sizeof(glm::mat4);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, size, 0);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, size,(void*) (size/4));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, size, (void*)(2 * size/4));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, size, (void*)(3 * size/4));
		

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
	

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		

		//Create vbo
		if (!m_index_vbo)
			glGenBuffers(1, &m_index_vbo);



	}



	

	void Batch3DShadow::enable_attributes(){

	

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//Enable vertex position
		glEnableVertexAttribArray(0);
	
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);

		//enable M matrix
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

	}

	void Batch3DShadow::render_batch() {
		
		enable_attributes();

		//i can bind the vbo, orphan it, pass the new data, and unbind it.
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices_positions.size() * sizeof(glm::vec3), vertices_positions.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), indices.data(), GL_STATIC_DRAW);

		for (RenderBatchShadow rb : m_render_batches){
			
			
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, rb.num_instances * sizeof(glm::mat4), &models_matrices[rb.instance_offset]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			U32 offset = rb.indices_offset * sizeof(U32);

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, rb.num_indices, GL_UNSIGNED_INT, (void*)offset, rb.num_instances, rb.vertex_offset);

		}


	}

	U8 Batch3DShadow::compare_models(model_and_matrix a, model_and_matrix b){
	
		return a.model->mesh < b.model->mesh;

	}


}



