#include "Batches/Batch2DFont.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include <GL/glew.h>




namespace Ryno {

	U8 almost_equals(F32 a, F32 b){
		if (abs(a - b) < 0.0001f)
			return true;
		return false;
	}

	U8 are_uvs_equals(glm::vec4* a, glm::vec4* b){
		if (almost_equals(a->x, b->x) && almost_equals(a->y, b->y) && almost_equals(a->z, b->z) && almost_equals(a->w, b->w))
			return true;
		return false;
	}

	U8 compare_uvs(glm::vec4* a, glm::vec4* b){
		if (almost_equals(a->x,b->x))
			if (almost_equals(a->y,b->y))
				if (almost_equals(a->z,b->z))
					return a->w < b->w;
				else return a->z < b->z;
			else return a->y < b->y;
		else return a->x < b->x;
	}

	void Batch2DFont::init() {
	
		create_vertex_array();

	}
	void Batch2DFont::begin() {

		m_render_batches.clear();
		m_font_glyphs.clear();
		input_instances.clear();
		
		Text::preallocate_memory_for_glyphs();

	}


	void Batch2DFont::end() {
	
		//Sort with provided compare function
		std::stable_sort(m_font_glyphs.begin(), m_font_glyphs.end(), compare_models);

		FontGlyph::current_glyph = 0;

		//Create batches
		create_render_batches();
	}


	void Batch2DFont::draw_font(Text* sprite_font) {
		if (sprite_font->active)
			sprite_font->font->draw(this, sprite_font);

	}

	void Batch2DFont::draw_glyph(FontGlyph* font_glyph) {
		m_font_glyphs.push_back(font_glyph);

	}




	void Batch2DFont::create_render_batches(){

		I32 sprites_number = (I32) m_font_glyphs.size();

		//Resize the instance vector at the beginning to avoid reallocations
		input_instances.resize(sprites_number);

		//Adds model matrix and other stuff to the final instance array.
		//One for each instance. 
		for (I32 i = 0; i < sprites_number; i++){
			input_instances[i].color = m_font_glyphs[i]->color;
			input_instances[i].m = m_font_glyphs[i]->model_matrix;
			input_instances[i].depth= m_font_glyphs[i]->depth;

		}
		

		//Return if no mesh
		if (m_font_glyphs.empty())
			return;

		U32 instance_offset = 0;
		

		//For each mesh...
		for (I32 cg = 0; cg < m_font_glyphs.size(); cg++){

		
			//If a mesh has a different texture or mesh than the one before, i create a new batch
			if (cg == 0	
				|| m_font_glyphs[cg]->get_texture_id() != m_font_glyphs[cg - 1]->get_texture_id()
				|| !are_uvs_equals(&m_font_glyphs[cg]->uv,&m_font_glyphs[cg - 1]->uv))
				
			{
				if (cg != 0){
					instance_offset += m_render_batches.back().num_instances;
				}
					
				m_render_batches.emplace_back(1, m_font_glyphs[cg]->get_texture_id(), m_font_glyphs[cg]->uv);
				
				
			}
			else
			{
				//else, i increment the number of instances
				m_render_batches.back().num_instances ++;
			}

		
		}	
		
	}

	void Batch2DFont::enable_attributes(){
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

		//Enable color and depth
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

	}


	

	void Batch2DFont::create_vertex_array(){
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
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

		
		//Create instanced vbo
		if (!m_i_vbo)
			glGenBuffers(1, &m_i_vbo);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
		
		
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceValuesFont), (void*)offsetof(InstanceValuesFont,m));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceValuesFont), (void*)(offsetof(InstanceValuesFont, m) + 12));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceValuesFont), (void*)(offsetof(InstanceValuesFont, m) + 24));
		glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceValuesFont), (void*)offsetof(InstanceValuesFont, color));
		glVertexAttribPointer(6, 1, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceValuesFont), (void*)offsetof(InstanceValuesFont, depth));


		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		if (!m_indices_vbo)
			glGenBuffers(1, &m_indices_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}







	void Batch2DFont::set_uvs(Vertex2D* quad_vertices, glm::vec4* uvs)
{
		
		quad_vertices[0].uv = glm::vec2(uvs->x, uvs->y );
		quad_vertices[1].uv = glm::vec2(uvs->x, uvs->y - uvs->w);
		quad_vertices[2].uv = glm::vec2(uvs->x + uvs->z, uvs->y - uvs->w);
		quad_vertices[3].uv = glm::vec2(uvs->x + uvs->z, uvs->y);

	}

	void Batch2DFont::render_batch() {

		 static Vertex2D quad_vertices[4] {
			Vertex2D(0, -1,0,1),
			Vertex2D(0, 1, 0,0),
			Vertex2D(2, 1, 1,0),
			Vertex2D(2, -1, 1, 1)
		};
		const U8 quad_indices[6]{ 0, 1, 2,0,2,3 };										//Quad indices	

		enable_attributes();

		U32 current_instance = 0;
		U32 number_of_instances = 0;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(U8), quad_indices, GL_STATIC_DRAW);

		for (RenderBatch2DFont rb : m_render_batches){

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, rb.texture);

			//Change UVs depending on letter glyph.
			//They are batched, so many identical letters share the same font uvs
			set_uvs(quad_vertices, &rb.uv);
							
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2D), quad_vertices, GL_STATIC_DRAW);
			
			glBindBuffer(GL_ARRAY_BUFFER, m_i_vbo);
			glBufferData(GL_ARRAY_BUFFER, rb.num_instances * sizeof(InstanceValuesFont), &input_instances[number_of_instances], GL_STATIC_DRAW);
				
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0, rb.num_instances);

			current_instance++;
			number_of_instances += rb.num_instances;
		}
		
	
	}

	U8 Batch2DFont::compare_models(FontGlyph* a, FontGlyph* b){
		if (a->get_texture_id() == b->get_texture_id())
			return compare_uvs(&a->uv, &b->uv);
		return a->get_texture_id() < b->get_texture_id();
	}

	


}



