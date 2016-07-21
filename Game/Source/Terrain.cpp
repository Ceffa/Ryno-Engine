#pragma once
#include "PerlinNoise.h"
#include "Terrain.h"

namespace Ryno{

	void BuildQuadForGrid(MeshBuilder* builder, glm::vec3 position, glm::vec2 uv,
		bool buildTriangles, int vertsPerRow)
	{
		builder->new_vertex();
		builder->set_position(position);
		builder->set_uvs(uv);

		if (buildTriangles)
		{
			int baseIndex = builder->get_vertices_count() - 1;

			int index0 = baseIndex;
			int index1 = baseIndex - 1;
			int index2 = baseIndex - vertsPerRow;
			int index3 = baseIndex - vertsPerRow - 1;

			builder->add_triangle(index0, index2, index1);
			builder->add_triangle(index2, index3, index1);
		}
	}

	void NewTerrain(MeshBuilder* builder, I32 m_SegmentCount, F32 m_Length, F32 m_Width, F32 m_Height, F32 freq){
		

		for (int i = 0; i < m_SegmentCount; i++)
		{
			float z = m_Length * i;
			float v = freq * i;

			for (int j = 0; j < m_SegmentCount; j++)
			{
				float x = m_Width * j;
				float u = freq * j;

				glm::vec3 offset;
			
				offset = glm::vec3(x - m_Width * m_SegmentCount / 2.0f, m_Height * PerlinNoise::octave_perlin(x, z, freq,3,.4f), z - m_Length * m_SegmentCount / 2.0f);

				glm::vec2 uv = glm::vec2(u, v);
				bool buildTriangles = i > 0 && j > 0;

				BuildQuadForGrid(builder, offset, uv, buildTriangles, m_SegmentCount);
			}
		}
		builder->calculate_tangents();
		builder->calculate_normals();


	}


}