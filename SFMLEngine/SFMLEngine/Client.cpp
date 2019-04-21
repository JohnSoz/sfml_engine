#include "Client.h"
#include "LogConsole.h"
#include <future>
#include <thread>
#include <mutex>
using namespace Engine;

Client::Client()
{
	IP = sf::IpAddress("18.185.27.153");
	std::srand((int)time(nullptr));
	Name = "Player#" + std::to_string(std::rand() % 100);
	thread = std::move(connect(std::bind(&Client::onConnect, this)));
}

//TODO: Если приняли пакет, то вызываем обработчик пакета в отдельном потоке 
void Client::recivePacket()
{
	std::mutex g_i_mutex;
	sf::Packet p;
	std::lock_guard<std::mutex> lock(g_i_mutex);
	if (socket.receive(p) == sf::Socket::Done)	
	{
		std::string recive;
		p >> recive;
		Console::AppLog::addLog(recive, Console::info);
	}
}

void Client::sendMsg_l(std::string msg)
{
	sf::Packet packet;
	packet << PacketType::message << msg;
	socket.send(packet);
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

std::thread Engine::Client::connect(std::function<void()> callback)
{
	sf::Packet packet;
	status = socket.connect(IP, 80);
	if (status != Socket::Error)
	{
		packet << Name;
		socket.send(packet);
		std::packaged_task<void()> task(std::bind(callback));
		auto future = task.get_future();
		std::thread t(std::move(task));
		return t;
	}
	else
	{
		Console::AppLog::addLog("Connect to 18.185.27.153:2000 failed", Console::error);
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
	sendPacket(3, sf::Socket::Disconnected);
	socket.disconnect();
}
