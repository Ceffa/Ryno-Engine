#pragma once

#include "Global.h"
#include "Model.h"
#include "MeshManager.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{

	
	class SimpleDrawer{
	public:
		
		static SimpleDrawer* get_instance();
		void draw(Model* model);
		
	private:
		SimpleDrawer();
		void create_vertex_array();

		U32 m_vbo;
		U32 m_vao;

		MeshManager* m_mesh_manager;


	};





}