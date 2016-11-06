#include "Game.h"
#include "Scene.h"
#include "Script.h"
namespace Ryno{

	std::vector<Scene*(*)()> SceneManager::scenes;
	std::map <std::string,U8> SceneManager::scene_names;
	U8 SceneManager::current = 0;
	U8 SceneManager::last_scene = 0;


	void Scene::init()
	{
		game = Game::get_instance();
		camera = new Camera3D(WindowSize::w, WindowSize::h);

	}

	void Scene::update_scripts()
	{
		for (auto* g : GameObject::game_objects)
			for (auto* s : g->scripts)
				s->update();
	}

	void Scene::input_scripts()
	{
		for (auto* g : GameObject::game_objects)
			for (auto* s : g->scripts)
				s->input();
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
			if (game->input_manager->is_key_down(SDLK_w, KEYBOARD)){
				camera->move_up(game->delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_s, KEYBOARD)){
				camera->move_down(game->delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_q, KEYBOARD)) {
				camera->move_forward(game->delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_e, KEYBOARD)) {
				camera->move_back(game->delta_time);
			}
		}
		camera->generate_matrices();
		camera->get_light_V_matrix();
		glm::vec2 mouse_coords = game->input_manager->get_mouse_movement();
		camera->rotate(mouse_coords.x * game->delta_time,mouse_coords.y* game->delta_time);
		glm::vec2 rotation_view = game->input_manager->get_controller_RX_coords();
		camera->rotate(0.01f * rotation_view.x* game->delta_time, 0.01f* rotation_view.y* game->delta_time);
		glm::vec2 direction = game->input_manager->get_controller_LX_coords();
		camera->move_forward(game->delta_time *1.0f * -direction.y);
		camera->move_right(game->delta_time * 1.0f * direction.x);
		/*Log::println("X: ");
		Log::print(camera->position.x);
		Log::print(" Y: ");
		Log::print(camera->position.y);
		Log::print(" Z: ");
		Log::print(camera->position.z);
		Log::println("Yaw: ");
		Log::print(camera->yaw);
		Log::print(" Pitch: ");
		Log::print(camera->pitch);*/
	}

	

}