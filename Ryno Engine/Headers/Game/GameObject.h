#pragma once
#include "Global.h"
#include "Transform.h"
#include <list>

namespace Ryno{

	class Script;
	class GameObject{
		friend class Scene;
	public:
		
		GameObject();
		~GameObject();
		GameObject(const GameObject& go);
		void copy(const GameObject& go);
		
		Transform transform;

		std::list<Script*> scripts;

		bool active = true;

		static std::list<GameObject*> game_objects;


		//Templates

		template<class T>
		T* add_script(T* s) {
			//If the script is unique, check if exists already.
			//If so return nullptr
			if (s->is_unique()) {
				if (get_script<T>())
					return nullptr;
			}
			
			s->game_object = this;
			scripts.push_back(s);
			s->start();
			return s;
		}
		template<class T>
		T* add_script() {
			return add_script(new T());
		}

		template<class T>
		T* remove_script(T* s)
		{
			s->game_object = nullptr;
			scripts.remove(s);
			return s;
		}

		template<class T>
		T* remove_script()
		{
			return remove_script(get_script<T>());
		}


		template<class T>
		void delete_script()
		{
			delete_script(get_script<T>());
		}
		template<class T>
		void delete_script(T* s)
		{
			scripts.remove(s);
			delete s;
		}



		template<class T>
		T* get_script() {
			for (auto* s : scripts)
				if (T* v = dynamic_cast<T*>(s))
					return v;
			return nullptr;
		}

		template<class T>
		std::list<T*> get_scripts() {
			std::list<T*> T_list;
			for (auto* s : scripts)
				if (T* v = dynamic_cast<T*>(s))
					T_list.push_back(v);
			return std::move(T_list);
		}

		

		
	};


}
