#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <functional>
#include <future>
#include <thread>
#include <mutex>
#include <map>
#include "Entity.h"
using namespace sf;

namespace Engine
{

	/*enum Status {
		Connect = 1,
		Disconnect
	};*/

	enum PacketType : uint8_t
	{
		newConnection = 0,
		syncClient,
		disconnection,
		updateData
	};

	struct data
	{
		sf::Vector2f position;
		std::string id;
	};

	class Client
	{
	private:
		std::string Name;
		sf::Socket::Status status;
		sf::IpAddress IP;
		std::thread thread;
		std::thread _connect(std::function<void()>);
	public:
		Object* d;
		std::map<uint32_t, std::unique_ptr<Object>> _users;
		Client() : d(nullptr) {}
		Client(Object* d);

		sf::TcpSocket socket;

		void connect()
		{
			IP = sf::IpAddress::getLocalAddress();
			std::srand((int)time(nullptr));
			Name = "Player#" + std::to_string(std::rand() % 100);
			thread = std::move(_connect(std::bind(&Client::onConnect, this)));
		}
		void recivePacket();

		void onRecivePacket(sf::Packet packet);

		void onConnect();

		template<typename... Args>
		void sendPacket(uint8_t type, Args... argc);

		void disckonnect();

		~Client();
	};

	template<typename... Args>
	void Client::sendPacket(uint8_t type, Args... argc)
	{
		sf::Packet packet;
		packet << type;
		(packet << ... << std::forward<Args>(argc));
		socket.send(packet);
	}

}
