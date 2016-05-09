#include "FireworkScene.h"
#include "HouseScene.h"
#include "ValleyScene.h"
#include <string>


#include <string>

namespace Ryno{
	class ScenesList{
	public:
		static void create_scenes(){
			SceneManager::add_to_scenes<HouseScene>(std::string("house"));
			SceneManager::add_to_scenes<FireworkScene>(std::string("firework"));
			SceneManager::add_to_scenes<ValleyScene>(std::string("valley"));
		}


		};
	}