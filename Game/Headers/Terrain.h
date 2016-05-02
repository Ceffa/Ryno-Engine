#pragma once
#include "MeshBuilder.h"

namespace Ryno{

	void BuildQuadForGrid(MeshBuilder* builder, glm::vec3 position, glm::vec2 uv,
		bool buildTriangles, int vertsPerRow);

	void NewTerrain(MeshBuilder* builder, I32 m_SegmentCount, F32 m_Length, F32 m_Width, F32 m_Height, F32 freq);
}