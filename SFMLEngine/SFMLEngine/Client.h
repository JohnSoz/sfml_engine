#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <functional>
#include <future>
#include <thread>
#include <mutex>
using namespace sf;

namespace Engine
{

	/*enum Status {
		Connect = 1,
		Disconnect
	};*/

	enum PacketType {
		message = 1,
		position,
		status
	};

	class Client
	{
	private:
		TcpSocket socket;
		std::string Name;
		sf::Socket::Status status;
		IpAddress IP;
		std::thread thread;
	public:
		Client();
		std::thread connect(std::function<void()>);
		void recivePacket();
		void onRecivePacket(sf::Packet packet);
		void onConnect();
		template<typename... Args>
		void sendPacket(int type, Args... argc);
		void sendMsg_l(std::string msg);
		void disckonnect();
		~Client();
	};

	template<typename... Args>
	void Engine::Client::sendPacket(int type, Args... argc)
	{
		sf::Packet packet;
		((packet << type << argc), ...);
		socket.send(packet);
	}

}
