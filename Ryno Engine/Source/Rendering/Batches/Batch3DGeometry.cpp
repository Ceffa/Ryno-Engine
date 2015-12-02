#include "Batches/Batch3DGeometry.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"


namespace Ryno {

	
	void Batch3DGeometry::begin() {

		m_render_batches.clear();
		input_instances.clear();
		m_game_objects.clear();
	



	}
	void Batch3DGeometry::end() {

	
		//Sort with provided compare function
		std::stable_sort(m_game_objects.begin(), m_game_objects.end(), compare_models);


		//Create batches
		create_render_batches();
	}

	void Batch3DGeometry::draw(GameObject* go) {

		m_game_objects.push_back(go);

	}




	void Batch3DGeometry::create_render_batches(){

		
		

	
		I32 models_size = (I32) m_game_objects.size();

		//Resize the MVP vector at the beginning to avoid reallocations
		input_instances.resize(models_size);

		//Adds MVP to the final instance array.
		//One for each instance. 
		for (I32 i = 0; i < models_size; i++){
			input_instances[i].color = m_game_objects[i]->model->color;
			input_instances[i].m = m_game_objects[i]->transform->model_matrix;
			input_instances[i].tiling = m_game_objects[i]->model->tiling;

		}
		

		//Return if no mesh
		if (m_game_objects.empty())
			return;

		U32 indices_offset = 0;
		U32 vertex_offset = 0;
		U32 instance_offset = 0;
		

		//For each mesh...
		for (I32 cg = 0; cg < m_game_objects.size(); cg++){

			Model* temp_model = m_game_objects[cg]->model;

			//If a mesh has a different texture or mesh than the one before, i create a new batch
			if (cg == 0
				|| temp_model->texture.id != m_game_objects[cg - 1]->model->texture.id
				|| temp_model->normal_map.id != m_game_objects[cg - 1]->model->normal_map.id
				|| temp_model->mesh != m_game_objects[cg - 1]->model->mesh)
			{
				if (cg != 0){
					RenderBatchGeometry* last_batch = &m_render_batches.back();
					vertex_offset += last_batch->num_vertices;
					indices_offset += last_batch->num_indices;
					instance_offset += last_batch->num_instances;
				}

				Mesh* temp_mesh = m_mesh_manager->get_mesh(temp_model->mesh);
				U16 num_indices = temp_mesh->indices_number;
				U16 num_vertices = temp_mesh->vertices_number;
	
				m_render_batches.emplace_back(vertex_offset, num_vertices, indices_offset, num_indices, instance_offset, 1, temp_model->texture.id, temp_model->normal_map.id, temp_model->mesh);
			
			}
			else
			{
				//else, i increment the number of instances
				m_render_batches.back().num_instances ++;
			}

		
		}
		I32 total_vertices = m_render_batches.back().vertex_offset + m_render_batches.back().num_vertices;
		I32 cv = 0;
		vertices.resize(total_vertices);
		for (RenderBatchGeometry rb : m_render_batches){
			for (Vertex3D v : m_mesh_manager->get_mesh(rb.mesh)->vertices){
				vertices[cv++] = v;
			}
		}

		I32 total_indices = m_render_batches.back().indices_offset + m_render_batches.back().num_indices;
		cv = 0;
		indices.resize(total_indices);
		for (RenderBatchGeometry rb : m_render_batches){
			for (U16 v : m_mesh_manager->get_mesh(rb.mesh)->indices){
				indices[cv++] = v;
			}
		}
	}

	void Batch3DGeometry::enable_attributes(){
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//Enable all vertex info
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		//enable M matrix
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);

		//Enable tiling and color
		glEnableVertexAttribArray(8);
		glEnableVertexAttribArray(9);
	}

	void Batch3DGeometry::create_vertex_array(){
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
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, tangent));

	
		


		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);


		
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		
		
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceGeometry), (void*)offsetof(InputInstanceGeometry, m));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceGeometry), (void*)(offsetof(InputInstanceGeometry, m) + 16));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceGeometry), (void*)(offsetof(InputInstanceGeometry, m) + 32));
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(InputInstanceGeometry), (void*)(offsetof(InputInstanceGeometry, m) + 48));
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(InputInstanceGeometry), (void*)offsetof(InputInstanceGeometry, tiling));
		glVertexAttribPointer(9, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InputInstanceGeometry), (void*)offsetof(InputInstanceGeometry, color));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
		glVertexAttribDivisor(9, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Create vbo
		if (!m_index_vbo)
			glGenBuffers(1, &m_index_vbo);

		



	}







	

	void Batch3DGeometry::render_batch() {

		enable_attributes();

		//Vertices data
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), &vertices[0], GL_STATIC_DRAW);

		//Indices data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U16), &indices[0], GL_STATIC_DRAW);
		for (RenderBatchGeometry rb : m_render_batches){
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, rb.texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, rb.normal_map);
			
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * sizeof(InputInstanceGeometry), &input_instances[rb.instance_offset], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		
			U32 offset = rb.indices_offset * sizeof(U16);
			
			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, rb.num_indices, GL_UNSIGNED_SHORT, (void*)offset, rb.num_instances, rb.vertex_offset);
			
		}
	}

	U8 Batch3DGeometry::compare_models(GameObject* a, GameObject* b){
		Model* ma = a->model;
		Model* mb = b->model;
		if (ma->texture.id == mb->texture.id){
			if (ma->normal_map.id == mb->normal_map.id)
				return ma->mesh < mb->mesh;
			return ma->normal_map.id < mb->normal_map.id;
		}
		return ma->texture.id < mb->texture.id;
	}


}



