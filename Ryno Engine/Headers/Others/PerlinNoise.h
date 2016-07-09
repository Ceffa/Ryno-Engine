#pragma once
#include "Global.h"

namespace RynoEngine{
	class PerlinNoise {
	public:
		//3D
		static F64 octave_perlin(F64 x, F64 y, F64 z, F64 freq, U8 octaves, F64 persistence);
		static F64 perlin(F64 x, F64 y, F64 z, F64 freq);
		//2D
		static F64 octave_perlin(F64 x, F64 y, F64 freq, U8 octaves, F64 persistence){ return octave_perlin(x, y, 0, freq, octaves, persistence); }
		static F64 perlin(F64 x, F64 y,  F64 freq){ return perlin(x, y, 0, freq); }

	private:
		
		static F64 grad(I32 hash, F64 x, F64 y, F64 z);
		static F64 fade(F64 t);
		static F64 lerp(F64 a, F64 b, F64 x);

		//Hash lookup table
		static U8 p[512];
	};
}