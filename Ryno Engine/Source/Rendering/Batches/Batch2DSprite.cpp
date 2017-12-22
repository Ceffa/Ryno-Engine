#include "Batches/Batch2DSprite.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include <GL/glew.h>

namespace Ryno {

	void Batch2DSprite::init() {
	
		create_vertex_array();

	}
	void Batch2DSprite::begin() {

		m_render_batches.clear();
		input_instances.clear();
		m_sprites.clear();
	}


	void Batch2DSprite::end() {
	
		//Sort with provided compare function
		std::stable_sort(m_sprites.begin(), m_sprites.end(), compare_models);


		//Create batches
		create_render_batches();
	}

	void Batch2DSprite::draw(Sprite* sprite) {

		if (sprite->active)
			m_sprites.push_back(sprite);

	}




	void Batch2DSprite::create_render_batches(){

		I32 sprites_number = (I32) m_sprites.size();

		//Resize the instance vector at the beginning to avoid reallocations
		input_instances.resize(sprites_number);

		//Adds model matrix and other stuff to the final instance array.
		//One for each instance. 
		for (I32 i = 0; i < sprites_number; i++){
			input_instances[i].color = m_sprites[i]->get_color();
			input_instances[i].m = m_sprites[i]->model_matrix;
			input_instances[i].tiling = m_sprites[i]->get_tiling();
			input_instances[i].depth = m_sprites[i]->depth;


		}
		

		//Return if no mesh
		if (m_sprites.empty())
			return;

		U32 instance_offset = 0;
		

		//For each mesh...
		for (I32 cg = 0; cg < m_sprites.size(); cg++){


			//If a mesh has a different texture or mesh than the one before, i create a new batch
			if (cg == 0	|| m_sprites[cg]->get_texture_id() != m_sprites[cg - 1]->get_texture_id())
				
			{
				if (cg != 0){
					instance_offset += m_render_batches.back().num_instances;
				}
					
				m_render_batches.emplace_back(1, m_sprites[cg]->get_texture_id());
				
				
			}
			else
			{
				//else, i increment the number of instances
				m_render_batches.back().num_instances ++;
			}

		
		}	
	}

	void Batch2DSprite::enable_attributes(){
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//Enable all vertex info
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		//enable M matrix
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		//Enable tiling, color and depth
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);

	}

	void Batch2DSprite::create_vertex_array(){
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

		//Tell vbo how to use the data it will receive.
		//In this case 4 bytes: 2 for position, 2 for uvs.
		//They are always normalized quads, because they are sprites,
		//so a byte is enough
		glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, 4, (void*)0);
		glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, 4, (void*)2);
		
		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		
		
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceValues), (void*)offsetof(InstanceValues,m));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceValues), (void*)(offsetof(InstanceValues, m) + 12));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceValues), (void*)(offsetof(InstanceValues, m) + 24));
		glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceValues), (void*)offsetof(InstanceValues, tiling));
		glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceValues), (void*)offsetof(InstanceValues, color));
		glVertexAttribPointer(7, 1, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceValues), (void*)offsetof(InstanceValues, depth));

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (!m_indices_vbo)
			glGenBuffers(1, &m_indices_vbo);

		


	}







	

	void Batch2DSprite::render_batch() {

		const I8 quad_vertices[16] {-1, -1, 0, 1, -1, 1, 0, 0, 1, 1, 1, 0, 1, -1, 1, 1};	//A mapped quad
		const U8 quad_indices[6]{ 0, 1, 2, 0, 2, 3 };										//Quad indices	

		enable_attributes();

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(I8), quad_vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(U8), quad_indices, GL_STATIC_DRAW);


		U32 current_instance = 0;
		U32 number_of_instances = 0;
		for (RenderBatch2D rb : m_render_batches){
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, rb.texture);
			
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * sizeof(InstanceValues), &input_instances[number_of_instances], GL_STATIC_DRAW);
			
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0, rb.num_instances);
			current_instance++;
			number_of_instances += rb.num_instances;
		}

	}

	const bool Batch2DSprite::compare_models(const Sprite* a, const Sprite* b){
	
		return a->get_texture_id() < b->get_texture_id();
	}


}



