#include "World.h"
using namespace Engine;

template<class Obj>
Obj& Engine::ObjectHandler::GetObjects(std::string NAME)
{
	Entity* retObj = nullptr;
	for (auto &item : ObjectsArray)
	{
		if (item->name == NAME)
			retObj = item;
	}
	return *static_cast<Obj*>(retObj);
}

void Engine::ObjectHandler::PushObject(Entity* obj)
{
	ObjectsArray.shrink_to_fit();
	ObjectsArray.push_back(obj);
}

void Engine::ObjectHandler::UpdateObjects(float time)
{
	for (auto& o : ObjectsArray)
	{
		o->update(time);
	}
}

void Engine::ObjectHandler::RenderObjects(sf::RenderWindow & WINDOW)
{
	for (auto& o : ObjectsArray)
	{
		WINDOW.draw(o->sprite);
	}
}

void Engine::ObjectHandler::refresh()
{
	ObjectsArray.erase(std::remove_if(std::begin(ObjectsArray), std::end(ObjectsArray), [](const Entity *entity)
	{
		return !entity->isActive();
	}));
}


void Engine::World::update(sf::RenderWindow & window, float time, sf::Event& event)
{
	objHandler.GetObjects<Actor>("Test").handleEvent(event);
	ImGui::SFML::Update(window, deltaClock.restart());
	objHandler.UpdateObjects(time);
	if (ShowOverlay)
	{
		ImGUI::SimpleOverlay(&ShowOverlay);
	}
	draw(window);
}


void Engine::World::handleEvent(sf::Event & event)
{
	ImGui::SFML::ProcessEvent(event);
	debug.handleEvent(event);
}


void Engine::World::draw(sf::RenderWindow & window)
{
	window.clear(sf::Color::White);
	//level.DrawLevel(*window);
	//sf::Sprite sprite(level.DrawLevel2());
	window.draw(LevelSprite);
	objHandler.RenderObjects(window);
	debug.draw();
	ImGui::SFML::Render(window);
	window.display();
}


void Engine::World::Init(sf::RenderWindow & window)
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	window.setView(view);
	level.LoadFromFile("Data/Level/map5.tmx", 2);
	LevelTexture.loadFromImage(*level.DrawLevel2());
	LevelSprite.setTexture(LevelTexture);
	debug.levelObjects(level.GetAllObjects());

	///TEST
	DebuggingSystem::log->addLog(ImGUI::Log("Lanuch", ImGUI::logType::info));
	DebuggingSystem::log->addLog(ImGUI::Log("Lanuch", ImGUI::logType::error));
	DebuggingSystem::log->addLog(ImGUI::Log("Init",   ImGUI::logType::system));
	DebuggingSystem::log->addLog(ImGUI::Log("Exit",   ImGUI::logType::fatal));

	sf::Image i;
	i.loadFromFile("Data/OSprite/AnimTile.png");
	pushEntity(new Engine::Actor(i, Vector2D(100, 120), sf::IntRect(38, 39, 209, 154), "Test", window, level));
}
