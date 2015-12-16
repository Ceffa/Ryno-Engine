#include "Game.h"
#include "Scene.h"

namespace Ryno{

	std::map<std::string, Scene*(*)()> SceneManager::scenes_map;

	Scene::Scene()
	{
		game = Game::get_instance();
		camera = new Camera3D(WINDOW_WIDTH, WINDOW_HEIGHT);

	}
	void Scene::camera_update()
	{
		if (!game->shell->active){


			if (game->input_manager->is_key_down(SDLK_d, KEYBOARD)){
				camera->move_right(game->delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_a, KEYBOARD)){
				camera->move_left(game->delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_w, KEYBOARD) || game->input_manager->is_key_down(SDL_BUTTON_LEFT, MOUSE)){
				camera->move_forward(game->delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_s, KEYBOARD) || game->input_manager->is_key_down(SDL_BUTTON_RIGHT, MOUSE)){
				camera->move_back(game->delta_time);
			}
		}
		camera->generate_VP_matrix();
		glm::vec2 mouse_coords = game->input_manager->get_mouse_movement();
		camera->rotate(mouse_coords.x * game->delta_time,mouse_coords.y* game->delta_time);
		glm::vec2 rotation_view = game->input_manager->get_controller_RX_coords();
		camera->rotate(0.01f * rotation_view.x* game->delta_time, 0.01f* rotation_view.y* game->delta_time);
		glm::vec2 direction = game->input_manager->get_controller_LX_coords();
		camera->move_forward(game->delta_time *1.0f * -direction.y);
		camera->move_right(game->delta_time * 1.0f * direction.x);
	}

	
	

}