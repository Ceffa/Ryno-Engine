#include "FireworkScene.h"
#include "HouseScene.h"
#include "ValleyScene.h"
#include "WaveScene.h"
#include "MinecraftScene.h"
#include "LightsScene.h"
#include "PhysicsScene.h"

#include <string>


#include <string>

namespace Ryno{
	class ScenesList{
	public:
		static void create_scenes(){
			SceneManager::add_to_scenes<PhysicsScene>(std::string("physics"));
			SceneManager::add_to_scenes<HouseScene>(std::string("house"));
			SceneManager::add_to_scenes<LightsScene>(std::string("lights"));
			SceneManager::add_to_scenes<ValleyScene>(std::string("valley"));
			SceneManager::add_to_scenes<WaveScene>(std::string("wave"));
			SceneManager::add_to_scenes<FireworkScene>(std::string("firework"));
			SceneManager::add_to_scenes<MinecraftScene>(std::string("minecraft"));

		}


		};
	}