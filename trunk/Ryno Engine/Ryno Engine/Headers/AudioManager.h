#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <map>
#include "Types.h"


namespace Ryno{

	//wrapper of sound effect
	class Sound{
		public:
			friend class AudioManager;
			void play(U32 loops = 0);
			static void set_volume(float volume);

		private:
			Mix_Chunk* m_chunk = nullptr;
	};
	//wrapper of music
	class Music{
		public:
			friend class AudioManager;
			void play(U32 loops = 1); 
			static void pause();
			static void set_volume(F32 volume);
			static void stop();
			static void resume();
		private:
			Mix_Music* m_music = nullptr;
	};

	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		void init();
		void destroy();
		Sound load_sound(const std::string& file_path);
		Music load_music(const std::string& file_path);



	private:
		std::map <std::string, Mix_Chunk*> m_sound_map;
		std::map <std::string, Mix_Music*> m_music_map;


		bool m_is_initialized = false;
	};

}