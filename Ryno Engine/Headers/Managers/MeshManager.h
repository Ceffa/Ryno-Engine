#pragma once

#include <string>
#include <vector>
#include "Structures.h"
#include "Model.h"
#include "Global.h"

#define TEMPORARY_OFFSET 1073741823
#define MESH_LOG 0
namespace Ryno{

	struct Mesh{

		std::vector<Vertex3D> vertices;
		std::vector<U32> indices;
		F32 min_X, min_Y, min_Z, max_X, max_Y, max_Z;
		void calculate_AABB();
	};

	class MeshManager{


	public:

		static MeshManager* get_instance();
		I32 load_mesh(const std::string& name, Owner loc);
		Model* load_model(const std::string& name, Owner loc, Shader& shader);
		I32 create_empty_mesh(Owner loc);
		Mesh* get_mesh(I32 mesh_number);

		void reset();//preserve engine meshes

	private:
		MeshManager::MeshManager();
		I32 last_temporary_mesh, last_lifetime_mesh;
		std::vector < Mesh* > temporary_meshes;
		std::vector < Mesh* > lifetime_meshes;

	};
}