#pragma once

#include "Types.h"
#include "Model.h"
#include "MeshManager.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{

	
	class SimpleDrawer{
	public:
		SimpleDrawer(){}
		~SimpleDrawer(){}
		void init();
		void draw(Model* model);
		
	private:

		void create_vertex_array();

		U32 m_vbo;
		U32 m_vao;

		MeshManager& m_mesh_loader = MeshManager::get_instance();


	};





}