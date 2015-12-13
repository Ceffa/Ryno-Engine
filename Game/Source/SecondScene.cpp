#include "SecondScene.h"
#include "MainScene.h"
#include "ShowScene.h"

namespace Ryno{

	void SecondScene::start(){

		game->camera->position = glm::vec4(0, 150, -500, 1);

		Texture white = game->texture_manager->load_png("white_pixel", GAME);
		Texture white_normal = game->texture_manager->load_png("normal_pixel", GAME);

		I32 cone_mesh = game->mesh_manager->load_mesh("cone", 1, GAME);

		game->camera->skybox = game->texture_manager->load_cube_map("full_moon_small", GAME);

		////loading audio
		//sound = game->audio_manager->load_sound("stomp.wav", GAME);
		//music = game->audio_manager->load_music("journey.ogg", GAME);
		//sound.set_volume(0.0f);
		//music.set_volume(0.0f);
		//music.play();

		go.create(game->ref_allocator);
		go->model.create(game->ref_allocator);
		go->model->set_color_and_flatness(255, 255, 255, 0);
		go->model->set_texture_normal(white, white_normal);
		go->model->mesh = cone_mesh;
		go->transform.create(game->ref_allocator);
		go->transform->set_scale(50, 100, 100);
		go->transform->set_position(120, 55, 50);
		go->dir_light.create(game->ref_allocator);
		go->dir_light->set_direction(-65, 150);
		go->dir_light->diffuse_intensity = 0.2;
		go->dir_light->set_diffuse_color(255, 255, 200);
		go->dir_light->specular_intensity = .05;
		go->dir_light->set_specular_color(255, 255, 200);
		go->dir_light->ambient_intensity = .05;
		go->dir_light->set_ambient_color(255, 255, 200);
	}


	void SecondScene::update(){
		
	}

	void SecondScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene(new ShowScene());

			}
		}
	}



}