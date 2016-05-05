#pragma once
#include "Global.h"
#include "Transform.h"
namespace Ryno{

	class Game;


	class Scene{
	public:
		Camera3D* camera;


		Scene();
		virtual ~Scene() = 0{}
		virtual void start() = 0;
		virtual void input() = 0;
		virtual void input_scripts() final;
		virtual void update() = 0;
		virtual void update_scripts() final;
		void camera_update();

		

	
		Game* game;

	};
	
	template<typename T> Scene * create_scene() { return new T; }

	class SceneManager{
		
	public:
		static Scene * new_scene(std::string const& s)
		{
			auto it = scenes_map.find(s);
			if (it == scenes_map.end()){
				std::cout << "Scene manager: Scene doesn't exist" << std::endl;
				return nullptr;
			}
			current = s;
			return it->second();
		}

		static Scene * reset_scene()
		{
			auto it = scenes_map.find(current);
			if (it == scenes_map.end()) {
				std::cout << "Scene manager: Scene doesn't exist" << std::endl;
				return nullptr;
			}
			return it->second();
		}

		template<typename T>
		static void add_to_scenes(std::string& scene_name)
		{
			scenes_map[scene_name] = &create_scene < T > ;
		}
	protected: 
		static std::string current;
		static std::map<std::string, Scene*(*)()> scenes_map;


	};
}


