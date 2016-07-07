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
	class PhysicsScene : public Scene
	{

	public:
		PhysicsScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void update() override;
		GameObject ball;

	};
}
