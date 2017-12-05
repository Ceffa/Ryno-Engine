#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Scene.h"
#include "Game.h"
#include "Material.h"
#include "GUIObject.h"

namespace Ryno{

	class HouseScene : public Scene
	{

	public:
		HouseScene() : Scene(){}
	private:

		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;
		Font font;

		Shader material;
		GameObject light[1];
		GameObject sponza;

		Texture white, white_normal, noise;
	
		int light_index = 0;

	};
}