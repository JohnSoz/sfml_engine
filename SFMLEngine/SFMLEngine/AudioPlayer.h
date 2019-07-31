#pragma once
#include <vector>
#include <SFML\Audio.hpp>
#include <filesystem>
#include "JsonLoader.h"
#include "LogConsole.h"
#include <sfeMovie/Movie.hpp>
#include <audiere.h>
using namespace std;
namespace fs = std::experimental::filesystem;
namespace Engine
{
	class MusicPlayer
	{
	private:
		audiere::AudioDevicePtr device;
		std::vector<std::pair<std::string, audiere::OutputStreamPtr>>::iterator iter;
		std::vector<std::pair<std::string, audiere::OutputStreamPtr>> muslist2;
		float volume = 0.3;
		std::string folderName;
		void GetMusic();
	public:
		MusicPlayer();
		MusicPlayer(std::string folder) : folderName(std::move(folder))
		{
			device = audiere::OpenDevice();
			GetMusic();
			srand(time(NULL));
			iter = muslist2.begin() + rand() % muslist2.size();
		}
		~MusicPlayer() = default;

		void Play();
		void NextSong();
		void PauseMus();
		void SetVolume(float);
		void PlaySongByName(std::string_view);
		void LoadMusic(std::string_view, std::string_view);
		void Update();
		float getVolume() { return volume; }
		std::vector<std::string> GetMusList();
		std::pair<std::string, audiere::OutputStreamPtr>* GetCurrMus() const;
	};
}
