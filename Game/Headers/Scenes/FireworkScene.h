#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Global.h"
#include "Scene.h"
#include "Game.h"
namespace Ryno{
	class FireworkScene : public Scene
	{

	public:
		FireworkScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;

		GameObject go[4];
		Texture white, white_normal, brick, brick_normal;
		Shader shader,shader2;
		I32 star_mesh, cube_mesh, sphere_mesh, cone_mesh;
		I32 idx = 0;
	};
}
