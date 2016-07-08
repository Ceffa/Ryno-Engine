#include "Batches/Batch3DGeometry.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {

	bool Batch3DGeometry::sorting = true;


	void Batch3DGeometry::init(Camera3D* cam) {
		set_camera(cam);
		create_vertex_array();
		m_mesh_manager = MeshManager::get_instance();
		indices.resize(0);
		input_instances = nullptr;

	}
	void Batch3DGeometry::set_camera(Camera3D* camera) {
		m_camera = camera;
	}

	void Batch3DGeometry::begin() {

		m_render_batches.clear();
		free(input_instances);
		m_models.clear();
		shaders.clear();
		create_vertex_array();
	
	}
	void Batch3DGeometry::end() {



		//Sort with provided compare function
		if (sorting)
			std::stable_sort(m_models.begin(), m_models.end(), compare_models);

		//Create batches
		create_render_batches();

	}

	void Batch3DGeometry::draw(Model* mod) {
		for (SubModel& m : mod->sub_models)
			m_models.push_back(&m);
	}




	void Batch3DGeometry::create_render_batches(){

		U32 total_size = 0;
		for (auto mod : m_models){
			total_size += mod->material.shader->attributes_struct_size;
		}

		//Resize the MVP vector at the beginning to avoid reallocations
		input_instances = malloc(total_size);

		U32 temp_size = 0;
		for (auto mod : m_models){
			const auto& m = mod->material;
			U32 curr_size = m.shader->attributes_struct_size;
			std::memcpy((void*)((U64)input_instances + temp_size), m.attribute_memory, curr_size);
			temp_size += curr_size;

		}
		
		//Return if no mesh
		if (m_models.empty())
			return;

		U32 indices_offset = 0;
		U32 vertex_offset = 0;
		U32 instance_offset = 0;
		
		bool first_iter = true;
		SubModel* last_mod = nullptr;
		//For each mesh...
		for (auto new_mod : m_models){
	
			//Checks to see if the new model has different uniforms than the previous one,
			//thus requiring a new draw call (and a new render abtch)

			bool equals_uniform = true;
			
			if (first_iter || new_mod->material.shader != last_mod->material.shader){
				equals_uniform = false;
				shaders.push_back(new_mod->material.shader);
			}
			else if ( new_mod->mesh != last_mod->mesh)
				equals_uniform = false;
			else {

				for (const auto& cnt : new_mod->material.shader->uniforms_data){
					if (0 != memcmp(new_mod->material.uniform_map[cnt.first], last_mod->material.uniform_map[cnt.first], cnt.second.size)){
						equals_uniform = false;
						break;
					}
				}
			}

			last_mod = new_mod;

			if (!equals_uniform){
				if (!first_iter){
					RenderBatchStruct* last_batch = &m_render_batches.back();
					vertex_offset += last_batch->num_vertices;
					indices_offset += last_batch->num_indices;
					instance_offset += last_batch->num_instances;
				}
				

				Mesh* temp_mesh = m_mesh_manager->get_mesh(new_mod->mesh);
				m_render_batches.emplace_back(vertex_offset, temp_mesh->vertices_number, indices_offset, temp_mesh->indices_number, instance_offset, 1, new_mod);
	
			}
			else
			{
				//else, i increment the number of instances
				m_render_batches.back().num_instances ++;
			}

			first_iter = false;

		}
		I32 total_vertices = m_render_batches.back().vertex_offset + m_render_batches.back().num_vertices;

		I32 cv = 0;
		vertices.resize(total_vertices);
		for (const auto& rb : m_render_batches){
			for (Vertex3D v : m_mesh_manager->get_mesh(rb.model->mesh)->vertices){
				vertices[cv++] = v;
			}
		}

		I32 total_indices = m_render_batches.back().indices_offset + m_render_batches.back().num_indices;
		cv = 0;
		indices.resize(total_indices);
		for (const auto& rb : m_render_batches){
			for (U32 v : m_mesh_manager->get_mesh(rb.model->mesh)->indices){
				indices[cv++] = v;
			}
		}

	}


	void Batch3DGeometry::enable_attributes(Shader* s){

		//Bind vertex-vbo and specify vertex attributes
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		
		for (const auto& cnt : s->vertex_3d_locations){
			glVertexAttribPointer(cnt.second.loc, cnt.second.nr, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), cnt.second.offset);
			glEnableVertexAttribArray(cnt.second.loc);
		}

		//bind instance-vbo and specify instance attributes
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
		
		U64 buffer_data_offset = 0;
		for (const auto& rb : m_render_batches){

			auto curr_shader = rb.model->material.shader;
			curr_shader->use();

			U32 old_shader_size = 0;//keep track of the size of the last used shader
			if (old_shader)old_shader_size = old_shader->attributes_struct_size;

			//If the new batch has a different shader than the one before, 
			//enable attributes and send global data
			if (old_shader != curr_shader){
				old_shader = curr_shader;
				enable_attributes(curr_shader);
				U8 curr_samp = 0;
				//Send global shader uniforms
				for (const auto& cnt : curr_shader->global_uniforms_data){
					curr_shader->send_global_uniform_to_shader(cnt.first, cnt.second.value, &curr_samp);
				}
			}

			U8 current_sampler = 0;
			
			for (const auto& cnt : rb.model->material.uniform_map)
			{
				curr_shader->send_material_uniform_to_shader(cnt.first, cnt.second, &current_sampler);
			}
		
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * curr_shader->attributes_struct_size, (void*)((U64)input_instances + buffer_data_offset ), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			buffer_data_offset += rb.num_instances* curr_shader->attributes_struct_size;
			U32 offset = rb.indices_offset * sizeof(U32);

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, rb.num_indices, GL_UNSIGNED_INT, (void*)offset, rb.num_instances, rb.vertex_offset);

			curr_shader->unuse();
		}
	}

	const U8 Batch3DGeometry::compare_models(SubModel* a, SubModel* b){
		const auto& ma = a->material;
		const auto& mb = b->material;

		if (ma.shader != mb.shader)
			return ma.shader < mb.shader;

		if (a->mesh != b->mesh)
			return a->mesh < b->mesh;

		auto ita = ma.uniform_map.begin();
		auto itb = mb.uniform_map.begin();

		while (ita != ma.uniform_map.end()){
			I8 res = memcmp(ita->second, itb->second, ma.shader->uniforms_data[ita->first].size);
			if (res == 0){
				ita++; itb++;
				continue;
			}
			return res < 0 ? true: false;
		}
		return false;
	}


}



