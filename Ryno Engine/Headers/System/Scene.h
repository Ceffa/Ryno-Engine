#pragma once
#include "Global.h"
#include "Transform.h"
#include <map>
#include <memory>
#include "Network.h"
#include "PostProcessor.h"

namespace Ryno{

	class Game;
	class Camera3D;
	class NetStruct;
	class NetObject;

	class Scene{
	public:
		
		std::unique_ptr<Camera3D> camera;
		std::unique_ptr<PostProcessor> post_processor;

		Scene() {
		
		}
		void init();
		
		virtual ~Scene() {}
		virtual void start() {}
		virtual void input() {}
		virtual void input_scripts() final;
		virtual void update() {}
		virtual void update_scripts() final;
		bool camera_control_enabled = true;
		void camera_update();
		Game* game;		

	};
	
	template<typename T> Scene * create_scene() { return new T; }

	class SceneManager{
		
	public:

		static I8 get_scene_nr(std::string const& s) {
			auto it = scene_names.find(s);
			if (it == scene_names.end())
				return -1;
			return it->second;
		}
		static bool scene_exists(I8 n) {
			return n < last_scene && n>=0;
		}
		static Scene * new_scene(std::string const& s)
		{
			I8 n = get_scene_nr(s);
			if (!scene_exists(n))
				return nullptr;
			return new_scene(n);
		}
		static Scene * new_scene(I8 n)
		{
			if (!scene_exists(n))
				return nullptr;
			current = n;
			Scene* s = scenes[n]();
			s->init();
			return s;
		}


		static Scene * reset_scene()
		{
			return new_scene(current);
		}

		static Scene * next_scene()
		{
			return new_scene((current+1)%last_scene);
		}

		template<typename T>
		static void add_to_scenes(std::string& scene_name)
		{
			scene_names[scene_name] = last_scene++;
			scenes.push_back(&create_scene < T >);
		}
	protected: 
		static U8 current;
		static std::vector<Scene*(*)()> scenes;
		static std::map <std::string, U8> scene_names;
		static U8 last_scene;


	};
}


