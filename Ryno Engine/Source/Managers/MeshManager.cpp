#include "MeshManager.h"
#include "MeshBuilder.h"
#include "Log.h"
#include <iostream>
#include "ObjLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

	Model* MeshManager::load_model(const std::string& name, Owner loc, Shader& shader)
	{
		static const std::string middle_path = "Resources/Models/";

		const std::string& obj_path = BASE_PATHS[loc] + middle_path + name + ".obj";
		const std::string& mtl_path = BASE_PATHS[loc] + middle_path;



		std::string err;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(obj_path, aiProcess_Triangulate | aiProcess_FlipUVs);


		if (!err.empty()) {
			std::cerr << err << std::endl;
			exit(1);
		}

		Model* model = new Model();

		//std::cout << "Meshes: " << scene->mNumMeshes << std::endl;
		//std::cout << "Meshes: " << scene->mNumMaterials << std::endl;

		TextureManager* text_man = TextureManager::get_instance();
		for (int i = 0; i < scene->mNumMeshes; i++) {
			
			
			auto* assimp_mesh = scene->mMeshes[i];
			auto mat = scene->mMaterials[assimp_mesh->mMaterialIndex];
			

			//Aboid rendering of invisible objects
			float transp;
			mat->Get(AI_MATKEY_OPACITY, transp);
			if (transp < .9f)
				continue;

			aiColor3D diffuse;
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			aiColor3D specular;
			mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			F32 shininess;
			mat->Get(AI_MATKEY_SHININESS, shininess);
			ColorRGBA diffuse_final(diffuse.r*255, diffuse.g*255, diffuse.b*255,0);
		
			ColorRGBA specular_final(specular.r*255, specular.g*255, specular.b*255,shininess*255);

			SubModel& sm = model->add_sub_model();
			sm.cast_shadows = true;
			sm.material.set_shader(&shader);
			//Set colors
			sm.material.set_attribute("diffuse_color", diffuse_final);
			sm.material.set_attribute("specular_color", specular_final);

			//Set textures
			aiString tex_name;
			aiReturn r = mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex_name);	
			
			sm.material.set_uniform("diffuse_texture", r == aiReturn_SUCCESS ? text_man->load_png(name + "/" + tex_name.C_Str(), GAME).id : -1);

			r = mat->GetTexture(aiTextureType_SPECULAR, 0, &tex_name);
			sm.material.set_uniform("specular_texture", r == aiReturn_SUCCESS ? text_man->load_png(name + "/" + tex_name.C_Str(), GAME).id : -1);
			
			r = mat->GetTexture(aiTextureType_NORMALS, 0, &tex_name);
			sm.material.set_uniform("normal_texture", r == aiReturn_SUCCESS ? text_man->load_png(name + "/" + tex_name.C_Str(), GAME).id : -1);
		
			Mesh* mesh = new Mesh();

			//Set vertices
			for (U32 i = 0; i < assimp_mesh->mNumVertices; i++)
			{
				Vertex3D v;
				v.position = glm::vec3(assimp_mesh->mVertices[i].x, assimp_mesh->mVertices[i].y, assimp_mesh->mVertices[i].z);
				if (assimp_mesh->mNormals)
					v.normal = glm::vec3(assimp_mesh->mNormals[i].x, assimp_mesh->mNormals[i].y, assimp_mesh->mNormals[i].z);
				if (assimp_mesh->mTangents)
					v.tangent = glm::vec3(assimp_mesh->mTangents[i].x, assimp_mesh->mTangents[i].y, assimp_mesh->mTangents[i].z);
				
				if (assimp_mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
				{
					v.uv.x = assimp_mesh->mTextureCoords[0][i].x;
					v.uv.y = assimp_mesh->mTextureCoords[0][i].y;
				}
				else {

					v.uv.x = 0;
					v.uv.y = 0;
				}

				mesh->vertices.push_back(v);

			}
			//Set indices
			for (GLuint i = 0; i < assimp_mesh->mNumFaces; i++)
			{
				aiFace face = assimp_mesh->mFaces[i];
				for (GLuint j = 0; j < face.mNumIndices; j++)
					mesh->indices.push_back(face.mIndices[j]);
			}

			mesh->indices_number = assimp_mesh->mNumFaces*3;
			mesh->vertices_number = assimp_mesh->mNumVertices;

			if (!assimp_mesh->mNormals) {
				MeshBuilder::calculate_normals(mesh);
			}
			if (!assimp_mesh->mTangents) {
	
				MeshBuilder::calculate_tangents(mesh);
			}


			if (loc == Owner::ENGINE)
				lifetime_meshes.push_back(mesh);
			else
				temporary_meshes.push_back(mesh);


			if (loc == Owner::ENGINE)
				sm.mesh = ++last_lifetime_mesh;
			else
				sm.mesh = ++last_temporary_mesh;

		}

		return model;
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