#include "PerlinNoise.h"

namespace Ryno{

	F64 PerlinNoise::octave_perlin(F64 x, F64 y, F64 z, F64 freq, U8 octaves, F64 persistence)
	{
		
		F64 total = 0;
		F64 frequency = freq;
		F64 amplitude = 1;
		F64 maxValue = 0;			// Used for normalizing result to 0.0 - 1.0
		for (U8 i = 0; i < octaves; i++) {
			total += perlin(x, y,z, frequency) * amplitude;

			maxValue += amplitude;

			amplitude *= persistence;
			frequency *= 2;
		}

		return total / maxValue;
	}

	F64 PerlinNoise::perlin(F64 x, F64 y, F64 z, F64 freq)
	{
		x = x*freq;
		y = y*freq;
		z = z*freq;
		U8 xi = (U32)x & 255;								// Calculate the "unit cube" that the point asked will be located in
		U8 yi = (U32)y & 255;								// The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
		U8 zi = (U32)z & 255;								// plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
		F64 xf = x - (U32)x;								// We also fade the location to smooth the result.
		F64 yf = y - (U32)y;
		F64 zf = z - (U32)z;
		F64 u = fade(xf);
		F64 v = fade(yf);
		F64 w = fade(zf);

		I32 aaa, aba, aab, abb, baa, bba, bab, bbb;
		aaa = p[p[p[xi] + yi] + zi];
		aba = p[p[p[xi] + yi+1] + zi];
		aab = p[p[p[xi] + yi] + zi+1];
		abb = p[p[p[xi] + yi+1] + zi+1];
		baa = p[p[p[xi+1] + yi] + zi];
		bba = p[p[p[xi+1] + yi+1] + zi];
		bab = p[p[p[xi+1] + yi] + zi+1];
		bbb = p[p[p[xi+1] + yi+1] + zi+1];

		F64 x1, x2, y1, y2;
		x1 = lerp(grad(aaa, xf, yf, zf),				// The gradient function calculates the dot product between a pseudorandom
			grad(baa, xf - 1, yf, zf),				// gradient vector and the vector from the input coordinate to the 8
			u);										// surrounding points in its unit cube.
		x2 = lerp(grad(aba, xf, yf - 1, zf),				// This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
			grad(bba, xf - 1, yf - 1, zf),				// values we made earlier.
			u);
		y1 = lerp(x1, x2, v);

		x1 = lerp(grad(aab, xf, yf, zf - 1),
			grad(bab, xf - 1, yf, zf - 1),
			u);
		x2 = lerp(grad(abb, xf, yf - 1, zf - 1),
			grad(bbb, xf - 1, yf - 1, zf - 1),
			u);
		y2 = lerp(x1, x2, v);

		return (lerp(y1, y2, w) + 1) / 2;
	}

	F64 PerlinNoise::grad(I32 hash, F64 x, F64 y, F64 z)
	{
		//Calculate dot product between vector x,y,z
		//and one of the 12 random standard vectors.
		//Since the 12 vectors are all like (-1,0,1) or (1,-1,0) etc
		//the cross can be calculated like follows
		switch (hash & 0xF)
		{
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x + z;
		case 0x5: return -x + z;
		case 0x6: return  x - z;
		case 0x7: return -x - z;
		case 0x8: return  y + z;
		case 0x9: return -y + z;
		case 0xA: return  y - z;
		case 0xB: return -y - z;
		case 0xC: return  y + x;
		case 0xD: return -y + z;
		case 0xE: return  y - x;
		case 0xF: return -y - z;
		default: return 0; // never happens
		}
	}

	F64 PerlinNoise::fade(F64 t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	F64 PerlinNoise::lerp(F64 a, F64 b, F64 x)
	{
		return a + x * (b - a);
	}

	U8 PerlinNoise::p[512]  = { 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15,
		131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};

}