#pragma once
#include <vector>
#include <SFML\Audio.hpp>
#include <filesystem>
#include "LogConsole.h"
using namespace std;
namespace fs = std::experimental::filesystem;
namespace Engine
{
	class AudioPlayer
	{
	private:
		std::vector<std::pair<std::string, sf::Music*>> musList;
		std::vector<std::pair<std::string, sf::Music*>>::iterator iter;
		float volume = 10;

		void GetMusic();
	public:
		AudioPlayer();

		~AudioPlayer()
		{
			for (iter = musList.begin(); iter != musList.end(); ++iter)
			{
				delete iter->second;
			}
			musList.clear();
		}

		void Play();
		void NextSong();
		void PauseMus();
		void SetVolume(float);
		void PlaySongByName(std::string);
		void LoadMusic(std::string, std::string);
		void Update();
		float getVolume() { return volume; }
		std::vector<std::string> GetMusList();
		std::pair<std::string, sf::Music*> &GetCurrMus() const;
	};
}
