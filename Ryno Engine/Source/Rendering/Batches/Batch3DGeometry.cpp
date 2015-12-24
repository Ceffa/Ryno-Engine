#include "Batches/Batch3DGeometry.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {

	Shader* Batch3DGeometry::s;
	
	void Batch3DGeometry::begin() {

		m_render_batches.clear();
		free(input_instances);
		m_game_objects.clear();
		create_vertex_array();
	



	}
	void Batch3DGeometry::end() {

		number_of_models = (I32)m_game_objects.size();

		//Send last uniforms and attributes to the models
		for (I32 i = 0; i < number_of_models; i++){
			auto m = m_game_objects[i]->model->material;
			m->set_attribute("in_M", m_game_objects[i]->transform->model_matrix);
			m->set_uniform("V", m_camera->get_V_matrix());
			m->set_uniform("VP", m_camera->get_VP_matrix());

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


		//Resize the MVP vector at the beginning to avoid reallocations
		input_instances = malloc(number_of_models * s->attributes_struct_size);

		
		for (I32 i = 0; i < number_of_models; i++){
			auto m = m_game_objects[i]->model->material;
			std::memcpy((void*)((U64)input_instances + s->attributes_struct_size*i), m->attribute_memory, s->attributes_struct_size);
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
			//Checks to see if the new model has different uniforms than the previous one,
			//thus requiring a new draw call (and a new render abtch)

			bool equals_uniform = true;
			
			if (cg == 0 || temp_model->mesh != m_game_objects[cg - 1]->model->mesh)
				equals_uniform = false;
			else {
	
				for (auto cnt : s->uniforms_map){
					if (0 != Shader::compare_uniforms(temp_model->material->uniform_map[cnt.first], m_game_objects[cg - 1]->model->material->uniform_map[cnt.first])){
						equals_uniform = false;
						return;
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

				Mesh* temp_mesh = m_mesh_manager->get_mesh(temp_model->mesh);
				U32 num_indices = temp_mesh->indices_number;
				U32 num_vertices = temp_mesh->vertices_number;
				m_render_batches.emplace_back(vertex_offset, num_vertices, indices_offset, num_indices, instance_offset, 1, temp_model->mesh);
				for (auto cnt : s->uniforms_map){
					m_render_batches.back().uniforms.emplace_back(cnt.first, temp_model->material->uniform_map[cnt.first]);

				}
			
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
			for (U32 v : m_mesh_manager->get_mesh(rb.mesh)->indices){
				indices[cv++] = v;
			}
		}
	}


	void Batch3DGeometry::enable_attributes(){
		glBindVertexArray(m_vao);

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

	}

	void Batch3DGeometry::render_batch() {

		enable_attributes();

		//Vertices data
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), vertices.data(), GL_STATIC_DRAW);

		//Indices data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), indices.data(), GL_STATIC_DRAW);
		std::cout << "RB: " << m_render_batches.size() << '\n';
		for (RenderBatchGeometry rb : m_render_batches){

			glUniformMatrix4fv(s->uniforms_map["V"].index, 1, GL_FALSE, &(*((glm::mat4*)rb.uniforms[0].value))[0][0]);
			glUniformMatrix4fv(s->uniforms_map["VP"].index, 1, GL_FALSE, &(*((glm::mat4*)rb.uniforms[1].value))[0][0]);

			U8 current_sampler = 0;
			for (auto cnt : rb.uniforms)
			{
				if (Shader::is_sampler(s->uniforms_map[cnt.name].type)){
					glUniform1i(s->uniforms_map[cnt.name].index, current_sampler++);
					glActiveTexture(GL_TEXTURE0 + current_sampler);
					glBindTexture(GL_TEXTURE_2D, *(U32*)cnt.value);
				}
			}

			
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * s->attributes_struct_size, (void*)((U64)input_instances+rb.instance_offset), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		
			U32 offset = rb.indices_offset * sizeof(U32);
			
			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, rb.num_indices, GL_UNSIGNED_INT, (void*)offset, rb.num_instances, rb.vertex_offset);
			
		}
	}

	const U8 Batch3DGeometry::compare_models(GameObject* a, GameObject* b){
		const Material* ma = a->model->material;
		const Material* mb = b->model->material;

		
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



