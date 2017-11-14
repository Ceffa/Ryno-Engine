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

namespace Ryno {

	class MinecraftScene : public Scene
	{

	public:
		MinecraftScene() : Scene() {}
	private:

		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;
		Font font;

		
		GameObject sponza;

		Texture white, white_normal;

		Shader material;
		int light_index = 0;

	};
}