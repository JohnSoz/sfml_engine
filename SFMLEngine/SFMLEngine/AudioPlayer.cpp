#include "AudioPlayer.h"
using namespace sf;
using namespace std;

void Engine::AudioPlayer::GetMusic()
{
	std::string path = "Music/";
	for (auto & p : fs::directory_iterator(path))
	{
		auto fileName = p.path().filename().generic_string();
		const size_t pos = fileName.find_last_of(".");
		fileName.erase(pos, 4);
		LoadMusic(fileName, p.path().generic_string());
	}
}

Engine::AudioPlayer::AudioPlayer()
{
	GetMusic();
	iter = musList.begin();
}

void Engine::AudioPlayer::Play()
{
	(*iter).second->play();
}

void Engine::AudioPlayer::NextSong()
{
	if (iter + 1 != musList.end())
	{
		(*iter).second->stop();
		iter++;
		(*iter).second->setVolume(volume);
		(*iter).second->play();
	}
	else
	{
		(*iter).second->stop();
		iter = musList.begin();
		(*iter).second->setVolume(volume);
		(*iter).second->play();
	}
}

void Engine::AudioPlayer::PauseMus()
{
	(*iter).second->pause();
}

void Engine::AudioPlayer::SetVolume(float v)
{
	volume = v;
	(*iter).second->setVolume(volume);
}

void Engine::AudioPlayer::PlaySongByName(std::string name)
{
	(*iter).second->stop();
	iter = musList.begin();
	for (int i = 0; i < musList.size(); i++) {
		if (musList[i].first == name) {
			iter += i;
			(*iter).second->play();
			//std::cout << (*iter).first << endl;
		}
	}
}

void Engine::AudioPlayer::LoadMusic(std::string name, std::string patch)
{
	auto Mus = new Music;
	Mus->openFromFile(patch);
	Mus->setVolume(volume);
	Console::AppLog::addLog("Load music at path(" + patch + ")", Console::logType::system);
	musList.push_back(std::pair(name, Mus));
}

void Engine::AudioPlayer::Update()
{
	if ((*iter).second->getStatus() == 0) {
		NextSong();
	}
}

std::vector<std::string> Engine::AudioPlayer::GetMusList()
{
	std::vector<std::string> vec;
	for (auto mus : musList) {
		vec.push_back(mus.first);
	}
	return vec;
}

std::pair<std::string, sf::Music*> & Engine::AudioPlayer::GetCurrMus() const
{
	return (*iter);
}
