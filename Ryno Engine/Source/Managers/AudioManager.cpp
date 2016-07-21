#include "AudioManager.h"
#include <iostream>

namespace Ryno{

	AudioManager* AudioManager::get_instance(){
		static AudioManager instance;
		return &instance;
	}
	AudioManager::AudioManager()
	{
	}


	AudioManager::~AudioManager()
	{
		destroy();
	}


	void AudioManager::init(){
		if (m_is_initialized){
			std::cout << "Tried to initialize audio engine twice" << std::endl;

		}
		//take bitwise with MIX_INIT_formato
		if (Mix_Init(MIX_INIT_MP3) == -1){
			std::cout << "SDL_Mixer failed: " + std::string(Mix_GetError()) << std::endl;
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)){
			std::cout << "SDL_Mixer failed: " + std::string(Mix_GetError()) << std::endl;
		}

		m_is_initialized = true;
	}
	void AudioManager::destroy(){
		if (m_is_initialized){
			m_is_initialized = false;

			reset();

			Mix_CloseAudio();//opposite of openaudio
			Mix_Quit();//opposite of init
		}
	}

	Sound AudioManager::load_sound(const std::string& file_path, Owner loc){

		Sound sound;
		static const std::string middle_path = "Resources/Sounds/";

	
		//check if already cached
		auto it = m_sound_map.find(file_path);
		if (it == m_sound_map.end()){
			//if not load
			Mix_Chunk* chunk = Mix_LoadWAV(std::string(BASE_PATHS[loc] + middle_path + file_path).c_str()); //despite name load everything
			if (chunk == nullptr)
				std::cout << "Mix_LoadWAV failed: " + std::string(Mix_GetError()) << std::endl;
			//insert it in map
			m_sound_map[file_path] = chunk; //map used as associative array
			sound.m_chunk = chunk;

		}
		else{
			sound.m_chunk = it->second;
		}

		return sound;

	}

	Music AudioManager::load_music(const std::string& file_path, Owner loc){
		Music music;
		static const std::string middle_path = "Resources/Music/";
		//check if already cached
		auto it = m_music_map.find(file_path);
		if (it == m_music_map.end()){
			//if not load
			Mix_Music* mus = Mix_LoadMUS(std::string(BASE_PATHS[loc] +  middle_path + file_path).c_str()); //despite name load everything
			if (mus == nullptr)
				std::cout << "Mix_LoadMus failed: " + std::string(Mix_GetError()) << std::endl;
			//insert it in map
			m_music_map[file_path] = mus; //map used as associative array
			music.m_music = mus;
		}
		else{
			music.m_music = it->second;
		}
		return music;
	}


	void AudioManager::reset()
	{
		for (auto it : m_sound_map){
			Mix_FreeChunk(it.second);
		}
		for (auto it : m_music_map){
			Mix_FreeMusic(it.second);
		}
		m_sound_map.clear();
		m_music_map.clear();
	}

	void Sound::play(U32 loops){
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1){
			//if all channels are busy, override first one
			if (Mix_PlayChannel(0, m_chunk, loops) == -1){
				std::cout <<"Mix_PlayChannel error: " + std::string(Mix_GetError()) << std::endl;
			}
		}
	}

	void Sound::set_volume(F32 volume){
		Mix_Volume(-1, (I32)(volume * 128));
	}
	//loops -1 = infinite
	void Music::play(U32 loops){
		Mix_PlayMusic(m_music, loops);
	}

	void Music::pause(){

		Mix_PauseMusic();

	}

	void Music::stop(){
		Mix_HaltMusic();
	}

	void Music::resume(){

		Mix_ResumeMusic();
	}

	void Music::set_volume(F32 volume){
		Mix_VolumeMusic((I32)(volume * 128));
	}

}
