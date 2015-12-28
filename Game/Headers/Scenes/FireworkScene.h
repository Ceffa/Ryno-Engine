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

		New<Sound> sound;
		New<Music> music;

		New<GameObject> go[4];
		New<Texture> white, white_normal, brick, brick_normal;
		New<Shader> shader,shader2;
		New<I32> star_mesh, cube_mesh, sphere_mesh, cone_mesh;
		int i = 0;


	};
}
