#pragma once
#include <vector>
#include <SFML\Audio.hpp>
#include <filesystem>
#include "JsonLoader.h"
#include "LogConsole.h"
using namespace std;
namespace fs = std::experimental::filesystem;
namespace Engine
{
	class MusicPlayer
	{
	private:
		std::vector<std::pair<std::string, sf::Music*>> musList;
		std::vector<std::pair<std::string, sf::Music*>>::iterator iter;
		float volume = 10;

		void GetMusic();
	public:
		MusicPlayer();

		~MusicPlayer()
		{
			for (iter = musList.begin(); iter != musList.end(); ++iter)
				delete iter->second;
			musList.clear();
		}

		void Play();
		void NextSong();
		void PauseMus();
		void SetVolume(float);
		void PlaySongByName(std::string_view);
		void LoadMusic(std::string_view, std::string_view);
		void Update();
		float getVolume() { return volume; }
		std::vector<std::string> GetMusList();
		std::pair<std::string, sf::Music*>* GetCurrMus() const;
	};

	class SongPlayer
	{
	private:
		std::vector<std::pair<std::string, sf::Sound*>> arraySound;
	public:
		SongPlayer() = default;
		~SongPlayer() = default;

		void addSound(std::pair<std::string, sf::Sound*> sound);
		void LoadSoundFromJson(std::string_view path);
		void Play(std::string_view name);
	};
}
