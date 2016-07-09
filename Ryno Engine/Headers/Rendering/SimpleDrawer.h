#pragma once

#include "Global.h"
#include "Model.h"
#include "MeshManager.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace RynoEngine{

	
	class SimpleDrawer{
	public:
		
		static SimpleDrawer* get_instance();
		void draw(Model* model);
		void draw(SubModel* model);

	private:
		SimpleDrawer();
		void create_vertex_array();
		void enable_attributes_and_uniforms(const Material& s);

		U32 m_vbo;
		U32 m_vao;
		U32 m_index_vbo;

		MeshManager* m_mesh_manager;


	};





}