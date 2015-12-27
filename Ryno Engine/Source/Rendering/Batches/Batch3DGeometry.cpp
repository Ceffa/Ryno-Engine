#include "Batches/Batch3DGeometry.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {

	
	void Batch3DGeometry::begin() {

		m_render_batches.clear();
		free(input_instances);
		m_game_objects.clear();
		shaders.clear();
		create_vertex_array();
	
	}
	void Batch3DGeometry::end() {

		number_of_models = (I32)m_game_objects.size();

		//Send model attribute to each model
		for (I32 i = 0; i < number_of_models; i++){
			auto m = *m_game_objects[i]->model->material;
			m->set_attribute("in_M", m_game_objects[i]->transform->model_matrix);
		}

		

		//Sort with provided compare function
		std::stable_sort(m_game_objects.begin(), m_game_objects.end(), compare_models);

		//Create batches
		create_render_batches();

	}

	void Batch3DGeometry::draw(GameObject* go) {

		m_game_objects.push_back(go);

	}




	void Batch3DGeometry::create_render_batches(){

		U32 total_size = 0;
		for (auto go : m_game_objects){
			total_size += go->model->material->shader->attributes_struct_size;
		}
		//Resize the MVP vector at the beginning to avoid reallocations
		input_instances = malloc(total_size);

		U32 temp_size = 0;
		for (I32 i = 0; i < number_of_models; i++){
			auto m = *m_game_objects[i]->model->material;
			U32 curr_size = m->shader->attributes_struct_size;
			std::memcpy((void*)((U64)input_instances + temp_size), m->attribute_memory, curr_size);
			temp_size += curr_size;

		}
		
		//Return if no mesh
		if (m_game_objects.empty())
			return;

		U32 indices_offset = 0;
		U32 vertex_offset = 0;
		U32 instance_offset = 0;
		

		//For each mesh...
		for (I32 cg = 0; cg < m_game_objects.size(); cg++){
	
			auto new_model = *m_game_objects[cg]->model;
			//Checks to see if the new model has different uniforms than the previous one,
			//thus requiring a new draw call (and a new render abtch)

			bool equals_uniform = true;
			
			if (cg == 0 || new_model->material->shader != m_game_objects[cg - 1]->model->material->shader){
				equals_uniform = false;
				shaders.push_back(new_model->material->shader);
			}
			else if ( new_model->mesh != m_game_objects[cg-1]->model->mesh)
				equals_uniform = false;
			else {

				for (auto cnt : new_model->material->shader->uniforms_data){
					if (0 != Shader::compare_uniforms(new_model->material->uniform_map[cnt.first], m_game_objects[cg - 1]->model->material->uniform_map[cnt.first])){
						equals_uniform = false;
						break;
					}
				}
			}
			if (!equals_uniform){
				if (cg != 0){
					RenderBatchGeometry* last_batch = &m_render_batches.back();
					vertex_offset += last_batch->num_vertices;
					indices_offset += last_batch->num_indices;
					instance_offset += last_batch->num_instances;
				}

				Mesh* temp_mesh = m_mesh_manager->get_mesh(new_model->mesh);
				m_render_batches.emplace_back(vertex_offset, temp_mesh->vertices_number, indices_offset, temp_mesh->indices_number, instance_offset, 1, new_model);
	
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
			for (Vertex3D v : m_mesh_manager->get_mesh(rb.model->mesh)->vertices){
				vertices[cv++] = v;
			}
		}

		I32 total_indices = m_render_batches.back().indices_offset + m_render_batches.back().num_indices;
		cv = 0;
		indices.resize(total_indices);
		for (RenderBatchGeometry rb : m_render_batches){
			for (U32 v : m_mesh_manager->get_mesh(rb.model->mesh)->indices){
				indices[cv++] = v;
			}
		}
	}


	void Batch3DGeometry::enable_attributes(Shader* s){

		//temporally bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//Tell vbo how to use the data it will receive
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, tangent));
		//Enable all vertex info
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);

		s->setup_vbo_attributes();

	}

	

	void Batch3DGeometry::create_vertex_array(){
		//Create vao
		if (!m_vao){
			glGenVertexArrays(1, &m_vao);
		}

		//Create vbo
		if (!m_vbo)
			glGenBuffers(1, &m_vbo);
	
		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);


		if (!m_index_vbo)
			glGenBuffers(1, &m_index_vbo);
		
		glBindVertexArray(m_vao);

	}

	void Batch3DGeometry::render_batch() {
		
		

		//Vertices data
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), vertices.data(), GL_STATIC_DRAW);
		//Indices data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), indices.data(), GL_STATIC_DRAW);

		Shader* old_shader = nullptr;
		
		
		for (RenderBatchGeometry rb : m_render_batches){

			Shader* curr_shader = rb.model->material->shader;
			curr_shader->use();

			U32 old_shader_size = 0;//keep track of the size of the last used shader

			//If the new batch has a different shader than the one before, 
			//enable attributes and send global data
			if (old_shader != curr_shader){
				if (old_shader)old_shader_size = old_shader->attributes_struct_size;
				old_shader = curr_shader;
				enable_attributes(curr_shader);
				U8 curr_samp = 0;
				//Send global shader uniforms
				for (auto cnt : curr_shader->global_uniforms_data){
					curr_shader->send_global_uniform_to_shader(cnt.first, cnt.second.value, &curr_samp);
				}
			}

			U8 current_sampler = 0;
			
			for (auto cnt : rb.model->material->uniform_map)
			{
				curr_shader->send_material_uniform_to_shader(cnt.first, cnt.second, &current_sampler);
			}
		
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * curr_shader->attributes_struct_size, (void*)((U64)input_instances + rb.instance_offset* old_shader_size), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		
			U32 offset = rb.indices_offset * sizeof(U32);

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, rb.num_indices, GL_UNSIGNED_INT, (void*)offset, rb.num_instances, rb.vertex_offset);

			curr_shader->unuse();
		}
	}

	const U8 Batch3DGeometry::compare_models(GameObject* a, GameObject* b){
		const auto ma = *a->model->material;
		const auto mb = *b->model->material;

		if (ma->shader != mb->shader)
			return ma->shader < mb->shader;

		if (a->model->mesh != b->model->mesh)
			return a->model->mesh < b->model->mesh;

		auto ita = ma->uniform_map.begin();
		auto itb = mb->uniform_map.begin();

		while (ita != ma->uniform_map.end()){
			if (ita->second == itb->second){
				ita++; itb++;
				continue;
			}
			return ita->second < itb->second;
		}
		return false;
	}


}



