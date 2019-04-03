#include "AudioPlayer.h"
using namespace sf;
using namespace std;

void Engine::MusicPlayer::GetMusic()
{
	std::string path = "Music/";
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
	iter = musList.begin();
}

void Engine::MusicPlayer::Play()
{
	Console::AppLog::addLog("Play music(" + (*iter).first + ")", Console::logType::system);
	(*iter).second->play();
}

void Engine::MusicPlayer::NextSong()
{
	if (iter + 1 != musList.end())
	{
		(*iter).second->stop();
		iter++;
		(*iter).second->setVolume(volume);
		Play();
	}
	else
	{
		(*iter).second->stop();
		iter = musList.begin();
		(*iter).second->setVolume(volume);
		Play();
	}
}

void Engine::MusicPlayer::PauseMus()
{
	(*iter).second->pause();
}

void Engine::MusicPlayer::SetVolume(float v)
{
	volume = v;
	(*iter).second->setVolume(volume);
}

void Engine::MusicPlayer::PlaySongByName(std::string_view name)
{
	(*iter).second->stop();
	iter = musList.begin();
	for (int i = 0; i < musList.size(); i++)
	{
		if (musList[i].first == name)
		{
			iter += i;
			Play();
		}
	}
}

void Engine::MusicPlayer::LoadMusic(std::string_view name, std::string_view patch)
{
	auto Mus = new Music;
	Mus->openFromFile(patch.data());
	Mus->setVolume(volume);
	Console::AppLog::addLog("Load music at path(" + (std::string)patch + ")", Console::logType::system);
	musList.emplace_back(name, Mus);
}

void Engine::MusicPlayer::Update()
{
	if ((*iter).second->getStatus() == 0)
		NextSong();
}

std::vector<std::string> Engine::MusicPlayer::GetMusList()
{
	std::vector<std::string> vec;
	for (const auto& mus : musList)
		vec.push_back(mus.first);
	return vec;
}

std::pair<std::string, sf::Music*>* Engine::MusicPlayer::GetCurrMus() const
{
	return &(*iter);
}


void Engine::SongPlayer::addSound(std::pair<std::string, sf::Sound*> sound)
{
	Console::AppLog::addLog("New sound added to sound_array named: " + sound.first, Console::info);
	arraySound.emplace_back(std::move(sound));
}

void Engine::SongPlayer::LoadSoundFromJson(std::string_view path)
{
	//JsonLoader json;
	//json.LoadFromPath(path.data());
}

void Engine::SongPlayer::Play(std::string_view name)
{
	auto s = std::find_if(begin(arraySound), end(arraySound), [name](const auto & sound) { return (sound.first == name); });
	(*s).second->play();
}
