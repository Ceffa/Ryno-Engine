#include "MeshManager.h"
#include "MeshBuilder.h"
#include "Log.h"
#include <iostream>
#include "ObjLoader.h"

namespace Ryno{

	MeshManager::MeshManager(){
		last_temporary_mesh = TEMPORARY_OFFSET;
		last_lifetime_mesh = 0;
		temporary_meshes.resize(0);
	}

	MeshManager* MeshManager::get_instance(){

		static MeshManager instance;//only at the beginning
		return &instance;
	}

	Mesh* MeshManager::get_mesh(I32 mesh_number){
		//if the number is very high, the mesh is for the game,
		//and the offset must be adjusted
		if (mesh_number >= TEMPORARY_OFFSET)
			return temporary_meshes[mesh_number - TEMPORARY_OFFSET - 1];
		else
			return lifetime_meshes[mesh_number - 1];
	}

	void MeshManager::reset()
	{
#if MESH_LOG
		std::cout << "Mesh Manager: number of meshes: " << temporary_meshes.size() << std::endl;
#endif
		temporary_meshes.clear();
		last_temporary_mesh = TEMPORARY_OFFSET;
	}

	I32 MeshManager::load_mesh(const std::string& name, Owner loc)
	{
		static const std::string middle_path = "Resources/Models/";

		const std::string& obj_path = BASE_PATHS[loc] + middle_path + name + ".obj";
		const std::string& mtl_path = BASE_PATHS[loc] + middle_path;

	
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		tinyobj::LoadObj(shapes, materials, err, obj_path.c_str(), mtl_path.c_str());

		if (!err.empty()) {
			std::cerr << err << std::endl;
			exit(1);
		}

		
		Mesh* mesh = new Mesh();
		mesh->indices = shapes[0].mesh.indices;
		mesh->indices_number = shapes[0].mesh.indices.size(); 
		mesh->vertices = shapes[0].mesh.vertices;
		mesh->vertices_number = shapes[0].mesh.vertices.size(); 

		if (shapes[0].mesh.has_uvs && !shapes[0].mesh.has_normals) 
			MeshBuilder::calculate_normals(mesh);
			
		MeshBuilder::calculate_tangents(mesh);
		

		if (loc == Owner::ENGINE)
			lifetime_meshes.push_back(mesh);
		else
			temporary_meshes.push_back(mesh);


		if (loc == Owner::ENGINE)
			return ++last_lifetime_mesh;
		else
			return ++last_temporary_mesh;
	}





	I32 MeshManager::create_empty_mesh(Owner loc)
	{
		Mesh* mesh = new Mesh();
		if (loc == GAME){
			temporary_meshes.push_back(mesh);
			return ++last_temporary_mesh;
		}
		lifetime_meshes.push_back(mesh);
		return ++last_lifetime_mesh;
	}
}