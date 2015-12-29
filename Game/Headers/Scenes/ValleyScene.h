#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Scene.h"
#include "Game.h"

namespace Ryno{

	class ValleyScene : public Scene
	{

	public:
		ValleyScene() : Scene(){}
	private:
		void start() override;
		void input() override;
		void update() override;

		New<Sound> sound;
		New<Music> music;
		New<Shader> shader;
		New<Texture> bt, bn;
		New<I32> terrain_mesh;
		New<GameObject> go;
		New<Model> mod;
	
	};
}
