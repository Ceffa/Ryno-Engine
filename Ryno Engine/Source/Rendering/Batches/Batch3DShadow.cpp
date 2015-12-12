#include "Batches/Batch3DShadow.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {

	



	void Batch3DShadow::begin() {

		m_render_batches.clear();
		input_instances.clear();
		m_game_objects.clear();
	



	}
	void Batch3DShadow::end() {

	
		//Sort with provided compare function
		std::stable_sort(m_game_objects.begin(), m_game_objects.end(), compare_models);


		//Create batches
		create_render_batches();
	}

	void Batch3DShadow::draw(GameObject* go) {

		//discard light-emitting models
		if (go->model->cast_shadows)
			m_game_objects.push_back(go);

	}




	void Batch3DShadow::create_render_batches(){

		
	
		I32 models_size = (I32) m_game_objects.size();

		//Resize the MVP vector at the beginning to avoid reallocations
		input_instances.resize(models_size);

		//Adds MVP to the final instance array.
		//One for each instance. 
		for (I32 i = 0; i < models_size; i++){
		
			input_instances[i].m = m_game_objects[i]->transform->model_matrix;
		

		}
		

		//Return if no mesh
		if (m_game_objects.empty())
			return;

		U32 indices_offset = 0;
		U32 vertex_offset = 0;
		U32 instance_offset = 0;
		

		//For each mesh...
		for (I32 cg = 0; cg < m_game_objects.size(); cg++){

			Model* temp_model = *m_game_objects[cg]->model;

			//If a mesh has a different texture or mesh than the one before, i create a new batch
			if (cg == 0
				|| m_game_objects[cg]->model->mesh != m_game_objects[cg - 1]->model->mesh)
			{
				if (cg != 0){
					indices_offset += m_render_batches.back().num_indices;
					vertex_offset += m_render_batches.back().num_vertices;
					instance_offset += m_render_batches.back().num_instances;
				}
				Mesh* temp_mesh = m_mesh_manager->get_mesh(temp_model->mesh);
				I32 num_indices = temp_mesh->indices_number;
				I32 num_vertices = temp_mesh->vertices_number;


				m_render_batches.emplace_back(vertex_offset, num_vertices, indices_offset, num_indices, instance_offset, 1, temp_model->mesh);

				
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
			for (Vertex3D v : m_mesh_manager->get_mesh(rb.mesh)->vertices){
				vertices_positions[cv++] = v.position;
			}
		}

		I32 total_indices = m_render_batches.back().indices_offset + m_render_batches.back().num_indices;
		cv = 0;
		indices.resize(total_indices);
		for (RenderBatchShadow rb : m_render_batches){
			for (U16 v : m_mesh_manager->get_mesh(rb.mesh)->indices){
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);


		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);


		
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		
		
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceShadow),(void*)offsetof(InputInstanceShadow, m));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceShadow), (void*)(offsetof(InputInstanceShadow, m) + 16));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceShadow), (void*)(offsetof(InputInstanceShadow, m) + 32));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceShadow), (void*)(offsetof(InputInstanceShadow, m) + 48));
		

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), &indices[0], GL_STATIC_DRAW);

		for (RenderBatchShadow rb : m_render_batches){
			
			
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * sizeof(InputInstanceShadow), nullptr, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, rb.num_instances * sizeof(InputInstanceShadow), &input_instances[rb.instance_offset]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			U32 offset = rb.indices_offset * sizeof(U32);

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, rb.num_indices, GL_UNSIGNED_INT, (void*)offset, rb.num_instances, rb.vertex_offset);

			//glDrawArraysInstanced(GL_TRIANGLES,rb.vertex_offset ,rb.num_vertices,rb.num_instances);
		}

		

	}

	U8 Batch3DShadow::compare_models(GameObject* a, GameObject* b){
	
		return a->model->mesh < b->model->mesh;
		
	}


}



