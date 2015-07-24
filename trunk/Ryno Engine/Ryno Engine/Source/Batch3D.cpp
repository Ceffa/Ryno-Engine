#include "Batch3D.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {

	void Batch3D::init(Camera3D* camera) {
		m_camera = camera;
		create_vertex_array();
		m_mesh_manager = MeshManager::get_instance();

	}




	void Batch3D::begin() {

		m_render_batches.clear();
		input_instances.clear();
		m_models.clear();
	



	}
	void Batch3D::end() {

	
		//Sort with provided compare function
		std::stable_sort(m_models.begin(), m_models.end(), compare_texture);


		//Create batches
		create_render_batches();
	}

	void Batch3D::draw(Model* model) {

		m_models.push_back(model);

	}




	void Batch3D::create_render_batches(){

		//Create vertices vector to send to gpu
		std::vector<Vertex3D> vertices;
		

	
		I32 models_size = (I32) m_models.size();

		//Resize the MVP vector at the beginning to avoid reallocations
		input_instances.resize(models_size);

		//Adds MVP to the final instance array.
		//One for each instance. 
		for (I32 i = 0; i < models_size; i++){
			input_instances[i].color = m_models[i]->color;
			input_instances[i].m = m_models[i]->model_matrix;
			input_instances[i].tiling = m_models[i]->tiling.get_vec_2();

		}
		

		//Return if no mesh
		if (m_models.empty())
			return;

		U32 vertex_offset = 0;
		U32 mvp_offset = 0;
		

		//For each mesh...
		for (I32 cg = 0; cg < m_models.size(); cg++){

			I32 mesh_size = (I32)m_mesh_manager->get_mesh(m_models[cg]->mesh)->size;

			//If a mesh has a different texture or mesh than the one before, i create a new batch
			if (cg == 0
				|| m_models[cg]->texture != m_models[cg - 1]->texture
				|| m_models[cg]->normal_map != m_models[cg - 1]->normal_map
				|| m_models[cg]->mesh != m_models[cg - 1]->mesh)
			{
				if (cg != 0){
					vertex_offset += m_render_batches.back().num_vertices;
					mvp_offset += m_render_batches.back().num_instances;
				}
					
				m_render_batches.emplace_back(vertex_offset, mvp_offset, mesh_size, 1, m_models[cg]->texture, m_models[cg]->normal_map, m_models[cg]->mesh);
				
				
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
		for (RenderBatch rb : m_render_batches){
			for (Vertex3D v : m_mesh_manager->get_mesh(rb.mesh)->vertices){
				vertices[cv++] = v;
			}
		}

		//i can bind the vbo, orphan it, pass the new data, and unbind it.
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex3D), vertices.data());
		
	
	}

	void Batch3D::create_vertex_array(){
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

		//Enable the attrib arrays
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);


		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);


		
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		
		
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 19, 0);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 19, (void*)(4 * 4));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 19, (void*)(8 * 4));
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(F32) * 19, (void*)(12 * 4));
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(F32) * 19, (void*)(16 * 4));
		glVertexAttribPointer(9, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(F32) * 19, (void*)(18 * 4));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
		glVertexAttribDivisor(9, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		



	}

	void Batch3D::render_deferred_scene(GLSLProgram* p){
		
		glUniformMatrix4fv(p->getUniformLocation("V"), 1, GL_FALSE, &m_camera->get_view_matrix()[0][0]);
		glUniformMatrix4fv(p->getUniformLocation("VP"), 1, GL_FALSE, &m_camera->get_camera_matrix()[0][0]);

		bind_for_deferred_scene();
		render_batch();
	}

	void Batch3D::render_shadow_scene(){
		bind_for_shadow_scene();
		render_batch();
	}

	void Batch3D::bind_for_deferred_scene(){

		//use texture and normal map
		texture_needed = true;

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

	void Batch3D::bind_for_shadow_scene(){

		//don't use texture and normal map
		texture_needed = false;

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//Enable only vertex position
		glEnableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		//enable M matrix
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);
		
		//Disable tiling and color
		glDisableVertexAttribArray(8);
		glDisableVertexAttribArray(9);
	}

	void Batch3D::render_batch() {
		
		I32 draw_calls = 0;
		bool a = false;
		



		for (RenderBatch rb : m_render_batches){
			
			if (texture_needed){
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, rb.texture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, rb.normal_map);
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * sizeof(InputInstance), nullptr, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, rb.num_instances * sizeof(InputInstance), &input_instances[rb.instance_offset]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			++draw_calls;
			
			glDrawArraysInstanced(GL_TRIANGLES,rb.vertex_offset ,rb.num_vertices,rb.num_instances);
		}

		static U32 count = 0;
		if (count++ == 30){
			count = 0;
			//std::cout << "Draw calls: " << draw_calls << std::endl;
		}
	}

	U8 Batch3D::compare_texture(Model* a, Model* b){
		if (a->texture == b->texture){
			if (a->normal_map == b->normal_map)
				return a->mesh < b->mesh;
			return a->normal_map < b->normal_map;
		}
		return a->texture < b->texture;
	}


}



