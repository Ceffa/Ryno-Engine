#pragma once
#include "Core.h"

namespace Ryno {
	//L & R -> Left and Right
	//U & D -> Up and Down
	//F & B -> Front and Back 
	enum OctreeEnum {
		DBL, DBR,
		UBL, UBR,
		DFL, DFR,
		UFL, UFR,
		SIZE
	}
	class OctreeNode {

		OctreeNode() {}
		union{
			OctreeNode* children;

	};
	class OctreeRoot {
	private:
		F WIDTH;
		F DEPTH;
		OctreeNode* root;

	

	
	
}