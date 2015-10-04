#include "MeshManager.h"
#include "Log.h"
#include <iostream>

namespace Ryno{

	MeshManager::MeshManager(){
		last_mesh = 0;
		meshes.resize(0);
		last_collider_mesh = 0;
		collider_meshes.resize(0);
	}

	MeshManager* MeshManager::get_instance(){

		static MeshManager instance;//only at the beginning
		return &instance;
	}

	Mesh* MeshManager::get_mesh(I32 mesh_number){
		return meshes[mesh_number-1];
	}
	ColliderMesh* MeshManager::get_collider_mesh(I32 collider_mesh_number){
		return collider_meshes[collider_mesh_number - 1];
	}

	I32 MeshManager::load_mesh(const std::string& name, bool has_uvs, LocationOfResource loc)
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
		meshes.push_back(mesh);
		mesh->vertices.resize(size);

		

		for (U32 i = 0; i < size; i++){
			U32 vertexIndex = vertexIndices[i];
			mesh->vertices[i].position = temp_vertices[vertexIndex - 1];

			if (has_uvs){
				U32 uvIndex = uvIndices[i];
				mesh->vertices[i].uv = temp_uvs[uvIndex - 1];
			}
			
			U32 normalIndex = normalIndices[i];
			mesh->vertices[i].normal = temp_normals[normalIndex - 1];


		}

		for (U32 i = 0; i < mesh->vertices.size(); i += 3) {
			Vertex3D* v0 = &mesh->vertices[i];
			Vertex3D* v1 = &mesh->vertices[i+1];
			Vertex3D* v2 = &mesh->vertices[i+2];

			if (has_uvs){
				glm::vec3 Edge1 = v1->position - v0->position;
				glm::vec3 Edge2 = v2->position - v0->position;

				F32 DeltaU1 = v1->uv.x - v0->uv.x;
				F32 DeltaV1 = v1->uv.y - v0->uv.y;
				F32 DeltaU2 = v2->uv.x - v0->uv.x;
				F32 DeltaV2 = v2->uv.y - v0->uv.y;

				F32 f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

				glm::vec3 Tangent, Bitangent;

				Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
				Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
				Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

				//Calculated in the fragment shader, but i leave the code here
				//Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
				//Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
				//Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

				v0->tangent += Tangent;
				v1->tangent += Tangent;
				v2->tangent += Tangent;
			}
		}

		if (has_uvs){
			for (U32 i = 0; i < mesh->vertices.size(); i++) {
				mesh->vertices[i].tangent = glm::normalize(mesh->vertices[i].tangent);
			}
		}

		meshes[last_mesh]->size = meshes[last_mesh]->vertices.size(); //One time only

		return ++last_mesh;
	}

	I32 MeshManager::load_collider_mesh(const std::string& name, LocationOfResource loc)
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