#include "AudioPlayer.h"

using namespace sf;
using namespace std;

void Engine::MusicPlayer::GetMusic()
{
	std::string path = "Data/Music/";
	if (!folderName.empty())
		path += folderName + "/";
	for (auto& p : fs::directory_iterator(path))
	{
		auto fileName = p.path().filename().generic_string();
		const size_t pos = fileName.find_last_of(".");
		fileName.erase(pos, 4);
		LoadMusic(fileName, p.path().generic_string());
	}
}

Engine::MusicPlayer::MusicPlayer()
{
	GetMusic();
	srand(time(NULL));
	iter = muslist2.begin() + rand() % muslist2.size();
}

void Engine::MusicPlayer::Play()
{
	Console::AppLog::addLog("Play music(" + (*iter).first + ")", Console::logType::system);
	(*iter).second->setVolume(volume);
	Console::AppLog::addLog("Music volume" + std::to_string((*iter).second->getVolume()), Console::logType::system);
	(*iter).second->play();
}

void Engine::MusicPlayer::NextSong()
{
	(*iter).second->stop();
	if (iter + 1 != muslist2.end())
		iter++;
	else
		iter = muslist2.begin();
	(*iter).second->setVolume(volume);
	Play();
}

void Engine::MusicPlayer::PauseMus()
{
	(*iter).second->stop();
}

void Engine::MusicPlayer::SetVolume(float v)
{
	volume = v;
	(*iter).second->setVolume(volume);
}

void Engine::MusicPlayer::PlaySongByName(std::string_view name)
{
	(*iter).second->reset();
	auto newMus = std::find_if(begin(muslist2), end(muslist2), [name](const auto& mus)
		{
			return mus.first == name;
		});
	iter = (newMus == muslist2.end()) ? muslist2.begin() : newMus;
	Play();
}

void Engine::MusicPlayer::LoadMusic(std::string_view name, std::string_view patch)
{
	Console::AppLog::addLog("Load music at path(" + (std::string)patch + ")", Console::logType::system);
	muslist2.emplace_back(name, OpenSound(device, patch.data(), false));
}

void Engine::MusicPlayer::Update()
{
	if (!(*iter).second->isPlaying())
		NextSong();
}

std::vector<std::string> Engine::MusicPlayer::GetMusList()
{
	std::vector<std::string> vec;
	for (const auto& mus : muslist2)
		vec.push_back(mus.first);
	return vec;
}

std::pair<std::string, audiere::OutputStreamPtr>* Engine::MusicPlayer::GetCurrMus() const
{
	return &(*iter);
}
