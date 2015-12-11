#include "GameObject.h"
#include "Emitter.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "ParticleManager.h"

namespace Ryno{

	void GameObject::set_emitter(Emitter* _emitter)
	{
		if (!*emitter)
			ParticleManager::get_instance()->add_emitter(this);
		emitter = New<Emitter>(_emitter);
		emitter->game_object = this;
	}

	void GameObject::remove_emitter()
	{
		ParticleManager::get_instance()->remove_emitter(this);
		emitter->game_object = nullptr;
		emitter = New<Emitter>(nullptr);
	}

	Emitter* GameObject::get_emitter()
	{
		return *emitter;
	}


	std::list<GameObject*> GameObject::game_objects;

	GameObject::GameObject(){
		transform = New<Transform>(new Transform());
	
		game_objects.push_back(this);

	}
	GameObject::GameObject(const GameObject* go) : GameObject()
	{
		*this = *go;
		
		if (*go->model)
			model = New<Model>(new Model(*go->model));
		if (*go->collider)
			collider = New<Collider>(go->collider->get_copy());
		if (*go->point_light)
			point_light = New<PointLight>(new PointLight(*go->point_light));
		if (*go->spot_light)
			spot_light = New<SpotLight> (new SpotLight(*go->spot_light));
		if (*go->dir_light)
			dir_light = New<DirectionalLight> (new DirectionalLight(*go->dir_light));
		if (*go->emitter)
			set_emitter(new Emitter(*go->emitter));
		
	}
	GameObject::~GameObject(){
		game_objects.remove(this);
	}

	

}

