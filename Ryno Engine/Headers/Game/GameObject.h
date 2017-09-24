#pragma once
#include "Global.h"
#include "Transform.h"
#include <list>

namespace Ryno{

	class Component;
	class GameObject{
		friend class Scene;
	public:
		
		GameObject();
		~GameObject();
		GameObject(const GameObject& go);
		GameObject& operator = (const GameObject &t) { return copy(t); };
		
		Transform transform;

		std::list<Component*> components;

		bool active = true;

		static std::list<GameObject*> game_objects;


		//Templates

		template<class T>
		T* add_component(T* s) {
						
			s->game_object = this;
			components.push_back(s);
			s->start();
			return s;
		}
		template<class T>
		T* add_component() {
			return add_component(new T());
		}

		template<class T>
		T* remove_component(T* s)
		{
			s->game_object = nullptr;
			components.remove(s);
			return s;
		}

		template<class T>
		T* remove_component()
		{
			return remove_component(get_component<T>());
		}


		template<class T>
		void delete_component()
		{
			delete_component(get_component<T>());
		}
		template<class T>
		void delete_component(T* s)
		{
			components.remove(s);
			delete s;
		}



		template<class T>
		T* get_component() {
			for (auto* s : components)
				if (T* v = dynamic_cast<T*>(s))
					return v;
			return nullptr;
		}

		template<class T>
		std::list<T*> get_components() {
			std::list<T*> T_list;
			for (auto* s : components)
				if (T* v = dynamic_cast<T*>(s))
					T_list.push_back(v);
			return std::move(T_list);
		}
	

private:
	GameObject& copy(const GameObject& go);

	};
}
