#pragma once

#include <GLM/glm.hpp>
#include "Structures.h"
#include "Text.h"
#include <vector>

namespace Ryno{


	struct InstanceValuesFont {
		glm::mat3 m;
		ColorRGBA color;
		U8 depth;
	};
	class RenderBatch2DFont{
	public:
		RenderBatch2DFont(U32 num_ins, U32 text, glm::vec4 uvs) :  num_instances(num_ins), texture(text), uv(uvs){}
		U32 num_instances;
		U32 texture;
		glm::vec4 uv;
	};


	class Batch2DFont{
	public:
		
		void init();
		void begin();
		void end();
		
		void draw_font(Text* sprite_font);
		void draw_glyph(FontGlyph* font_glyph);

		void render_batch();
		
	protected:

		std::vector<FontGlyph*> m_font_glyphs;
		U32 m_vbo;
		U32 m_i_vbo;//instancing vbo
		U32 m_vao;

		std::vector<InstanceValuesFont> input_instances;
		std::vector<RenderBatch2DFont> m_render_batches;
		
		static U8 compare_models(FontGlyph* a, FontGlyph* b) ;

		void create_render_batches();
		void create_vertex_array();
		void enable_attributes();
		void set_uvs(Vertex2D* quad_vertices, glm::vec4* uvs);
		

		
	};





}