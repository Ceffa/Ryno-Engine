#include "Game.h"
#include "Scene.h"
#include "Component.h"
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
			for (auto* s : g->components)
				s->update();
	}

	void Scene::input_scripts()
	{
		for (auto* g : GameObject::game_objects)
			for (auto* s : g->components)
				s->input();
	}




	void Scene::camera_update()
	{

		if (camera_control_enabled) {

			if (game->input_manager->is_key_down(SDLK_d, KEYBOARD)) {
				camera->move_right(TimeManager::delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_a, KEYBOARD)) {
				camera->move_left(TimeManager::delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_w, KEYBOARD)) {
				camera->move_up(TimeManager::delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_s, KEYBOARD)) {
				camera->move_down(TimeManager::delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_q, KEYBOARD)) {
				camera->move_forward(TimeManager::delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_e, KEYBOARD)) {
				camera->move_back(TimeManager::delta_time);
			}
		}
		glm::vec2 mouse_coords = game->input_manager->get_mouse_movement();
		camera->rotate(mouse_coords.x * TimeManager::delta_time, mouse_coords.y* TimeManager::delta_time);
		glm::vec2 rotation_view = game->input_manager->get_controller_RX_coords();
		camera->rotate(0.01f * rotation_view.x* TimeManager::delta_time, 0.01f* rotation_view.y* TimeManager::delta_time);
		glm::vec2 direction = game->input_manager->get_controller_LX_coords();
		camera->move_forward(TimeManager::delta_time *1.0f * -direction.y);
		camera->move_right(TimeManager::delta_time * 1.0f * direction.x);
		
		camera->generate_matrices();
		camera->get_light_V_matrix();
		
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