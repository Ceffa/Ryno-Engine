#include "Batches/Batch3DGeometry.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"
#include "CPUProfiler.h"
#include "TextureManager.h"

namespace Ryno {

	void Batch3DGeometry::init(Camera3D* cam) {
		set_camera(cam);
		create_vertex_array();
		m_mesh_manager = MeshManager::get_instance();
		indices.resize(0);
		input_instances = nullptr;
		dither_tex = TextureManager::get_instance()->load_png("dither.png", ENGINE).id;


	}
	void Batch3DGeometry::set_camera(Camera3D* camera) {
		m_camera = camera;
	}

	void Batch3DGeometry::begin() {

		m_render_batches.clear();
		free(input_instances);
		input_instances = nullptr;
		m_models.clear();
		shaders.clear();
		create_vertex_array();
	
	}
	void Batch3DGeometry::end() {

		//Sort with provided compare function
		std::stable_sort(m_models.begin(), m_models.end(), compare_models);

		//Create batches
		create_render_batches();

	}

	void Batch3DGeometry::draw(Model* mod) {
		U32 start_size = m_models.size();
		U32 total_size = mod->sub_models.size() + start_size;
		m_models.resize(total_size);

		for (I32 i = start_size; i < total_size; i++)
			m_models[i] = &mod->sub_models[i - start_size];
	}




	void Batch3DGeometry::create_render_batches(){

		U32 total_size = 0;
		for (const auto mod : m_models){
			total_size += mod->material.shader->attributes_struct_size;
		}

		//Resize the MVP vector at the beginning to avoid reallocations
		input_instances = malloc(total_size);

		U32 temp_size = 0;
		for (const auto mod : m_models){
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
		
		for (const auto new_mod : m_models){
	
			//Checks to see if the new model has different uniforms than the previous one,
			//thus requiring a new draw call (and a new render abtch)

			bool equals_uniform = true;
			auto& a = new_mod->material;
			auto& b = last_mod->material;

			if (first_iter || a.shader != b.shader){
				equals_uniform = false;
				shaders.push_back(a.shader);
			}
			else {
				equals_uniform = new_mod->mesh == last_mod->mesh && 0 == memcmp(a.uniform_memory, b.uniform_memory, a.shader->uniforms_map_size);
			}

			last_mod = new_mod;

			if (!equals_uniform){
				if (!first_iter){
					RenderBatchStruct* last_batch = &m_render_batches.back();
					vertex_offset += last_batch->num_vertices;
					indices_offset += last_batch->num_indices;
					instance_offset += last_batch->num_instances;
				}
				

				auto temp_mesh = m_mesh_manager->get_mesh(new_mod->mesh);
				m_render_batches.emplace_back(vertex_offset, temp_mesh->vertices.size(), indices_offset, temp_mesh->indices.size(), instance_offset, 1, new_mod);
				
			}
			else
			{
				//else, i increment the number of instances
				m_render_batches.back().num_instances ++;
			}

			first_iter = false;

		}
		

		total_vertices = m_render_batches.back().vertex_offset + m_render_batches.back().num_vertices;
		I32 total_indices = m_render_batches.back().indices_offset + m_render_batches.back().num_indices;
		vertices.resize(total_vertices);
		indices.resize(total_indices);

		
		for (const auto& rb : m_render_batches){
			const auto m = m_mesh_manager->get_mesh(rb.model->mesh);
			std::memcpy((void*)((U64)vertices.data() + rb.vertex_offset * sizeof(Vertex3D)), m->vertices.data(), m->vertices.size() * sizeof(Vertex3D));
			std::memcpy((void*)((U64)indices.data() + rb.indices_offset * sizeof(U32)), m->indices.data(), m->indices.size() * sizeof(U32));
		}	
	
		draw_calls = m_render_batches.size();

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
		//Create 

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

		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		for (const auto& rb : m_render_batches){

			auto curr_shader = rb.model->material.shader;

			DeferredRenderer::bind_global_ubo(*curr_shader);
			curr_shader->use();

			U32 old_shader_size = 0;//keep track of the size of the last used shader
			if (old_shader)old_shader_size = old_shader->attributes_struct_size;

			//If the new batch has a different shader than the one before, 
			//enable attributes and send glob data
			if (old_shader != curr_shader){
				old_shader = curr_shader;
				enable_attributes(curr_shader);
				U8 curr_samp = 0;
			}

		
			rb.model->material.set_uniform("dither_tex", dither_tex);
			rb.model->material.send_uniforms_to_shader();
			

			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * curr_shader->attributes_struct_size, (void*)((U64)input_instances + buffer_data_offset ), GL_STATIC_DRAW);
			buffer_data_offset += rb.num_instances* curr_shader->attributes_struct_size;
			U32 offset = rb.indices_offset * sizeof(U32);

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, rb.num_indices, GL_UNSIGNED_INT, (void*)offset, rb.num_instances, rb.vertex_offset);
			curr_shader->unuse();

		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	const bool Batch3DGeometry::compare_models(const SubModel* a, const SubModel* b) {
		const auto& ma = a->material;
		const auto& mb = b->material;

		return (ma.shader != mb.shader) ? ma.shader < mb.shader : (
			(a->mesh != b->mesh) ? a->mesh < b->mesh :
			memcmp(ma.uniform_memory, mb.uniform_memory, ma.shader->uniforms_map_size) < 0);
	}
	


}



