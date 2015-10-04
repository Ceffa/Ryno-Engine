#pragma once

#include <string>
#include <vector>
#include "Structures.h"
#include "Global.h"


namespace Ryno{
	
	struct Mesh{
	
		std::vector<Vertex3D> vertices;
		I32 size;
	};

	struct ColliderMesh{
		ColliderMesh() : size(0){
			vertices.clear();
		}
		std::vector<glm::vec3> vertices;
		I32 size;
	};

	class MeshManager{

		
	public:
		
		static MeshManager* get_instance();
		I32 load_mesh(const std::string& name, bool has_uvs, LocationOfResource loc);
		Mesh* get_mesh(I32 mesh_number);
		I32 load_collider_mesh(const std::string& name, LocationOfResource loc);
		ColliderMesh* get_collider_mesh(I32 collider_mesh_number);

	private:
		MeshManager::MeshManager();		
		I32 last_mesh, last_collider_mesh;
		std::vector < Mesh* > meshes;
		std::vector < ColliderMesh* > collider_meshes;
	
	};
}