#pragma once
#include "MainGameInterface.h"
#include "Lights/PointLight.h"
#include "DeferredRenderer.h"
#include "Lights/DirectionalLight.h"
#include <Sprite.h>

namespace Ryno{
	class MainGame : public MainGameInterface
	{
	private:
	
		void start() override;
		void input() override;
		void update() override;
		
		bool swap_curve = false;
		bool just_played = false;
		I64 speed = 40;
		I64 time = 0;;
		Sound sound;
		Music music;
		

		GameObject* spheres[4];
		I32 square_model;
		I32 bound_sphere;
		I32 cube_mesh;
		Model* sphere_box_model;

		
	
	};
}
