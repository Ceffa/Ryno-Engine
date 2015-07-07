#pragma once

#include <string>
#include <vector>
#include "Structures.h"
#include "Types.h"


namespace Ryno{
	struct Vertex3D{
		Vertex3D(){}
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 tangent;
	};
	struct Mesh{
	
		std::vector<Vertex3D> vertices;
		I32 size;
	};

	class MeshManager{

		
	public:
		
		static MeshManager* get_instance();
		I32 load_mesh(const std::string& name);
		Mesh* get_mesh(I32 mesh_number);

	private:
		MeshManager::MeshManager();		
		I32 last_mesh;
		std::vector < Mesh* > meshes;
	
	};
}