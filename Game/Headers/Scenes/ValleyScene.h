#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Scene.h"
#include "Game.h"
#include "GroundElements.h"
#include "LightsElements.h"
#include "GroundElements.h"
#include "LightsElements.h"

namespace RynoEngine{

	class ValleyScene : public Scene
	{

	public:
		ValleyScene() : Scene(){}
	private:
		void start() override;
		void input() override;
		void update() override;
		
		GameObject floor,sphere;
	};
}
