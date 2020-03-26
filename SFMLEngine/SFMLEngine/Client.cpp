#include "Client.h"
#include "LogConsole.h"
#include <future>
#include <thread>
#include <mutex>
using namespace Engine;

Client::Client(Object* d) : d(d)
{
}

//TODO: Если приняли пакет, то вызываем обработчик пакета в отдельном потоке 
void Client::recivePacket()
{
	std::mutex g_i_mutex;
	sf::Packet p;
	std::lock_guard<std::mutex> lock(g_i_mutex);
	uint8_t type;
	while (socket.receive(p) == sf::Socket::Done)
	{
		uint32_t id = -1;
		p >> type;
		if (type == PacketType::updateData)
		{
			p >> id;
			float x;
			float y;
			p >> x >> y;
			_users[id]->setPos(x, y);
		}
		if (type == PacketType::newConnection)
		{
			p >> id;
			float x;
			float y;
			p >> x >> y;
			_users.emplace(std::make_pair(id, std::make_unique<Object>(sf::Vector2f{ x, y }, std::to_string(id))));
			sendPacket(PacketType::syncClient, id, d->getPos().x, d->getPos().y);
		}
		if (type == PacketType::syncClient)
		{
			float x, y;
			p >> id >> x >> y;
			sf::CircleShape c1(10.f);
			c1.setPosition(x, y);
			_users.emplace(std::make_pair(id, std::make_unique<Object>(sf::Vector2f{ x, y }, std::to_string(id))));
		}
		if (type == PacketType::disconnection)
		{
			p >> id;
			_users.erase(id);
		}
	}
}

Client::~Client()
{
	if (status != sf::Socket::Error)
	{
		disckonnect();
		if (thread.joinable())
			thread.join();
	}
}

std::thread Engine::Client::_connect(std::function<void()> callback)
{
	sf::Packet packet;
	status = socket.connect(IP, 300);
	std::cout << status;
	if (status != sf::Socket::Error)
	{
		packet << d->getPos().x << d->getPos().y;
		socket.send(packet);
		std::packaged_task<void()> task(std::bind(callback));
		auto future = task.get_future();
		std::thread t(std::move(task));
		return t;
	}
	else
	{
		Console::AppLog::addLog("Connect to 18.185.27.153 nor success", Console::error);
		return std::thread();
	}
}
void Engine::Client::onRecivePacket(sf::Packet packet)
{
	std::string recive;
	packet >> recive;
}

void Engine::Client::onConnect()
{
	Console::AppLog::addLog("Connect to 18.185.27.153 success", Console::info);
	while (status != sf::Socket::Disconnected)
	{
		recivePacket();
	}
}

void Engine::Client::disckonnect()
{
	Console::AppLog::addLog("Disconnect from 18.185.27.153:2000", Console::info);
	status = sf::Socket::Disconnected;
	sendPacket(PacketType::disconnection, sf::Socket::Disconnected);
	socket.disconnect();
}
