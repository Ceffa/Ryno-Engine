#include "Batches/Batch3DAbstract.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"

namespace Ryno {

	void Batch3DAbstract::init(Camera3D* camera) {
		m_camera = camera;
		create_vertex_array();
		m_mesh_manager = MeshManager::get_instance();

	}
}



