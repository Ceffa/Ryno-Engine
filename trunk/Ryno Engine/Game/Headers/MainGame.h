#pragma once
#include "MainGameInterface.h"
#include "PointLight.h"
#include "DeferredRenderer.h"
#include "DirectionalLight.h"
#include <Sprite.h>

namespace Ryno{
	class MainGame : public MainGameInterface
	{
	private:
	
		void start() override;
		void input() override;
		void update() override;
		void draw() override;
		void end();
		
		bool swap_curve = false;
		bool just_played = false;
		I64 speed = 40;
		I64 time = 0;;
		Sound sound;
		Music music;
		std::vector<GameObject*> m_game_objects;
		std::vector<GameObject*> spheres;
		std::vector<Sprite*> m_sprites;

		GameObject* ball, *go1, *go2, *go3;
		I32 square_model;
		I32 bound_sphere;
		I32 cube_mesh;
		Model* sphere_box_model;

		
	
	};
}
