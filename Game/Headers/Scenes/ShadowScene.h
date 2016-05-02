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

	class ShadowScene : public Scene
	{

	public:
		ShadowScene() : Scene(){}
	private:
		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;
		Shader shader, dir_shader;
		Texture bt, bn;
		I32 terrain_mesh;
		GameObject go;
	
	};
}
