#pragma once

#include <string>
#include <vector>
#include "Structures.h"
#include "Types.h"


namespace Ryno{
	
	struct Mesh{
	
		std::vector<Vertex3D> vertices;
		I32 size;
	};

	class MeshLoader{

		
	public:
		
		static MeshLoader& get_instance();
		I32 load_mesh(const std::string& name, ColorRGBA color = ColorRGBA::ColorRGBA(1, 1, 1, 1));
		Mesh* get_mesh(I32 mesh_number);

	private:
		MeshLoader::MeshLoader();		
		I32 last_mesh;
		std::vector < Mesh* > meshes;
	
	};
}