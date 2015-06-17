#include "Model.h"
#include "Log.h"
#include <iostream>
#include <stdio.h>
#include "GLM/gtx/string_cast.hpp"

namespace Ryno{
	Model* Model::load_model(const std::string& name, ColorRGBA color)
	{
		const std::string& path = "Resources/Models/" + name + ".obj";
		Model* model = new Model();
		std::vector< U32 > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		FILE * file = fopen(path.c_str(), "r");
		if (!file){
			printf("Impossible to open the file !\n");
			return nullptr;
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
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

			}
		}

		U32 size = (U32)vertexIndices.size();
		model->vertices = std::vector<Vertex3D>(size);

		for (U32 i = 0; i < size; i++){
			U32 vertexIndex = vertexIndices[i];
			U32 uvIndex = uvIndices[i];
			U32 normalIndex = normalIndices[i];

			model->vertices[i].position = temp_vertices[vertexIndex - 1];
			model->vertices[i].uv = temp_uvs[uvIndex - 1];
			model->vertices[i].normal = temp_normals[normalIndex - 1];
			model->vertices[i].color = color;
		}


		return model;
	}
}