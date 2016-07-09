#include "MeshBuilder.h"

namespace RynoEngine{

	MeshBuilder* MeshBuilder::get_instance(){

		static MeshBuilder instance;//only at the beginning
		return &instance;
	}

	void MeshBuilder::reset()
	{
		mesh = nullptr;
		last_vertex = 0;
		mesh_id = 0;
	}

	void MeshBuilder::new_vertex()
	{
		mesh->vertices.emplace_back();
		last_vertex = &mesh->vertices[++mesh->vertices_number - 1];
	}

	void MeshBuilder::set_position(glm::vec3 pos)
	{
		last_vertex->position = pos;
	}
	void MeshBuilder::set_uvs(glm::vec2 uvs)
	{
		last_vertex->uv = uvs;
	}
	void MeshBuilder::set_normals(glm::vec3 normals)
	{
		last_vertex->normal = normals;
	}

	void MeshBuilder::set_tangent(glm::vec3 normals)
	{
		last_vertex->normal = normals;
	}


	void MeshBuilder::add_triangle(U32 a, U32 b, U32 c)
	{
		mesh->indices.push_back(a);
		mesh->indices.push_back(b);
		mesh->indices.push_back(c);
		mesh->indices_number += 3;
	}
	void MeshBuilder::add_quad(U32 a, U32 b, U32 c, U32 d)
	{
		add_triangle(a, b, c);
		add_triangle(a, c, d);
	}


	I32 MeshBuilder::get_mesh()
	{
		return mesh_id;
	}

	bool MeshBuilder::set_mesh(I32 _mesh)
	{
		mesh_id = _mesh;
		mesh = MeshManager::get_instance()->get_mesh(_mesh);
		if (!mesh){
			std::cout << "The mesh with ID " << _mesh << " doesn't exist" << std::endl;
			return false;
		}

		mesh->indices_number = mesh->indices.size();
		mesh->vertices_number = mesh->vertices.size();

		if (mesh->vertices_number == 0)
			last_vertex = nullptr;
		else
			last_vertex = &mesh->vertices[mesh->vertices_number - 1];

		mesh_id = _mesh;
		return true;
	}

	void MeshBuilder::calculate_tangents(Mesh* mesh)
	{
		for (U32 i = 0; i < mesh->indices_number; i += 3) {
			Vertex3D* a = &mesh->vertices[mesh->indices[i]];
			Vertex3D* b = &mesh->vertices[mesh->indices[i + 1]];
			Vertex3D* c = &mesh->vertices[mesh->indices[i + 2]];
			glm::vec3 Edge1 = b->position - a->position;
			glm::vec3 Edge2 = c->position - a->position;

			F32 DeltaU1 = b->uv.x - a->uv.x;
			F32 DeltaV1 = b->uv.y - a->uv.y;
			F32 DeltaU2 = c->uv.x - a->uv.x;
			F32 DeltaV2 = c->uv.y - a->uv.y;

			F32 f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

			glm::vec3 Tangent, Bitangent;

			Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
			Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
			Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

			//Calculated in the fragment shader, but i leave the code here
			//Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
			//Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
			//Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

			Tangent = glm::normalize(Tangent);
			a->tangent = Tangent;
			b->tangent = Tangent;
			c->tangent = Tangent;
		}
	}

	void MeshBuilder::calculate_normals(Mesh* mesh)
	{
		for (U32 i = 0; i < mesh->indices_number; i += 3) {
			Vertex3D* a = &mesh->vertices[mesh->indices[i]];
			Vertex3D* b = &mesh->vertices[mesh->indices[i + 1]];
			Vertex3D* c = &mesh->vertices[mesh->indices[i + 2]];

			glm::vec3 Edge1 = b->position - a->position;
			glm::vec3 Edge2 = c->position - a->position;

			glm::vec3 normal = glm::normalize(cross(Edge1, Edge2));

			a->normal = normal;
			b->normal = normal;
			c->normal = normal;
		}
	}


}