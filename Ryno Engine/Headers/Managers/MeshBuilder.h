#pragma once
#include "Global.h"
#include <iostream>
#include "MeshManager.h"

namespace Ryno{

	class MeshBuilder{

	public:
		I32 get_mesh();
		bool set_mesh(I32 _mesh);
		static MeshBuilder* get_instance();

		void reset();

		void new_vertex();

		void set_position(glm::vec3 pos);
		void set_position(F32 x, F32 y, F32 z) {set_position(glm::vec3(x, y, z));}

		void set_uvs(glm::vec2 uvs);
		void set_uvs(F32 u, F32 v) { set_uvs(glm::vec2(u,v)); }

		void set_normals(glm::vec3 pos);
		void set_normals(F32 p, F32 y, F32 r) { set_normals(glm::vec3(p,y,r)); }

		void set_tangent(glm::vec3 pos);
		void set_tangent(F32 p, F32 y, F32 r) { set_tangent(glm::vec3(p, y, r)); }

		void add_triangle(U32 a, U32 b, U32 c);
		void add_quad(U32 a, U32 b, U32 c, U32 d);

		static void calculate_normals(Mesh* mesh);
		static void calculate_tangents(Mesh* mesh);
		void calculate_normals() { calculate_normals(mesh);}
		void calculate_tangents() { calculate_tangents(mesh);};

		U32 get_vertices_count(){ return mesh->vertices.size(); };
		U32 get_indices_count(){ return mesh->indices.size(); };


	private:
		MeshBuilder::MeshBuilder(){}
		I32 mesh_id;
		Mesh* mesh;
		Vertex3D* last_vertex = nullptr;
	};

}