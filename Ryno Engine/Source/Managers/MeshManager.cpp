#include "MeshManager.h"
#include "MeshBuilder.h"
#include "Log.h"
#include <iostream>

namespace Ryno{

	MeshManager::MeshManager(){
		last_game_mesh = GAME_OFFSET;
		last_engine_mesh = 0;
		game_meshes.resize(0);
		last_collider_mesh = 0;
		collider_meshes.resize(0);
	}

	MeshManager* MeshManager::get_instance(){

		static MeshManager instance;//only at the beginning
		return &instance;
	}

	Mesh* MeshManager::get_mesh(I32 mesh_number){
		//if the number is very high, the mesh is for the game,
		//and the offset must be adjusted
		if (mesh_number >= GAME_OFFSET)
			return game_meshes[mesh_number - GAME_OFFSET - 1];
		else
			return engine_meshes[mesh_number - 1];
	}
	ColliderMesh* MeshManager::get_collider_mesh(I32 collider_mesh_number){
		return collider_meshes[collider_mesh_number - 1];
	}


	

	void MeshManager::reset()
	{
		game_meshes.clear();
		last_game_mesh = GAME_OFFSET;
		collider_meshes.clear();
		last_collider_mesh = 0;
	}

	I32 MeshManager::load_mesh(const std::string& name, bool has_uvs, Owner loc)
	{
		static const std::string middle_path = "Resources/Models/";

		const std::string& path = BASE_PATHS[loc] + middle_path + name + ".obj";

		std::vector< U32 > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		FILE * file = fopen(path.c_str(), "r");
		if (!file){
			printf("Impossible to open the file !\n");
			return -1;
		}

		while (1){
			printf("");
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.
			// else : parse lineHeader
			if (strcmp(lineHeader, "v") == 0){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);

			}
			else if (strcmp(lineHeader, "vt") == 0){
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);

			}
			else if (strcmp(lineHeader, "vn") == 0){
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);

			}
			else if (strcmp(lineHeader, "f") == 0){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				
				fgetc(file);
				char buffer[100];
				fgets(buffer, 100, file);
			
				int matches;
				if (has_uvs){
					matches = sscanf(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					if (matches != 9){
						printf("File can't be read by our simple parser : ( Try exporting with other options\n");
						return -1;
					}
				}
				else{

					matches = sscanf(buffer, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
					if (matches != 6){
						printf("File can't be read by our simple parser : ( Try exporting with other options\n");
						return -1;
					}
				}
				
				

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				if (has_uvs){
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
				}
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				

			}
		}

		U32 size = (U32)vertexIndices.size();
		Mesh* mesh = new Mesh();
		if (loc == Owner::ENGINE)
			engine_meshes.push_back(mesh);
		else
			game_meshes.push_back(mesh);
		std::vector <Vertex3D> vertices;
		vertices.resize(size);

		

		for (U32 i = 0; i < size; i++){
			U32 vertexIndex = vertexIndices[i];
			vertices[i].position = temp_vertices[vertexIndex - 1];

			if (has_uvs){
				U32 uvIndex = uvIndices[i];
				vertices[i].uv = temp_uvs[uvIndex - 1];
			}
			
			U32 normalIndex = normalIndices[i];
			vertices[i].normal = temp_normals[normalIndex - 1];


		}
		
		bool add = true;

		for (int i = 0; i < vertices.size(); i++){
			Vertex3D a = vertices[i];
			add = true;
			for (int j = 0; j < mesh->vertices.size(); j++){
				if (Vertex3D::Compare(a, mesh->vertices[j])){
					mesh->indices.push_back(j);
					add = false;
					break;
				}
			}
			if (add){
				mesh->indices.push_back(mesh->vertices.size());
				mesh->vertices.push_back(a);
			}
		}
		if (loc == Owner::GAME){
			I32 last = last_game_mesh - GAME_OFFSET;
			game_meshes[last]->vertices_number = game_meshes[last]->vertices.size(); //One time only
			game_meshes[last]->indices_number = game_meshes[last]->indices.size(); //One time only
		}
		else{
			engine_meshes[last_engine_mesh]->vertices_number = engine_meshes[last_engine_mesh]->vertices.size(); //One time only
			engine_meshes[last_engine_mesh]->indices_number = engine_meshes[last_engine_mesh]->indices.size(); //One time only

		}
		if (has_uvs)
			MeshBuilder::calculate_tangents(mesh);

		if (loc == Owner::ENGINE)
			return ++last_engine_mesh;
		else
			return ++last_game_mesh;
	}



	

	I32 MeshManager::create_empty_mesh()
	{
		Mesh* mesh = new Mesh();
		game_meshes.push_back(mesh);
		return ++last_game_mesh;
	}

	I32 MeshManager::load_collider_mesh(const std::string& name, Owner loc)
	{
		static const std::string middle_path = "Resources/Models/Colliders/";

		const std::string& path = BASE_PATHS[loc] + middle_path + name + ".obj";

		ColliderMesh* coll_mesh = new ColliderMesh();
		collider_meshes.push_back(coll_mesh);

	
		FILE * file = fopen(path.c_str(), "r");
		if (!file){
			printf("Impossible to open the file !\n");
			return -1;
		}

		while (1){
			printf("");
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.
			// else : parse lineHeader
			if (strcmp(lineHeader, "v") == 0){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				coll_mesh->vertices.push_back(vertex);
				coll_mesh->size++;

			}

		}



		return ++last_collider_mesh;
	}
}