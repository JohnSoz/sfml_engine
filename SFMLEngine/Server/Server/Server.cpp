#include "Server.h"
#include <future>
#include <algorithm>
using namespace std;

Server::Server()
{
	cout << "Server Running" << endl;
	auto z = listener.listen(80);
	selector.add(listener);
}

void Server::Start()
{
	while (!done)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				TcpSocket* socket = new TcpSocket;
				listener.accept(*socket);
				Packet packet;
				string PlayerName;
				int type;
				string txt;
				char data[100];
				std::size_t recived;
				txt = "Usres in server: ";
				if (socket->receive(packet) == Socket::Done)
				{
					packet >> PlayerName;
					cout << "User: " << PlayerName << ", IP: " << socket->getRemoteAddress().toString() << endl;
					std::string serverInfo = "Connect success";
					txt += PlayerName + "; ";
					for (auto user : clients)
					{
						txt += user.first + "; ";
						sf::Packet p;
						p << " new user connection: " + PlayerName;
						user.second->send(p);
						serverInfo += "client: " + user.first;
					}
					sf::Packet infoPacket;
					infoPacket << serverInfo;
					if (socket->send(infoPacket) == sf::Socket::Done)
						std::cout << "infoPacket send success" << std::endl;
					clients.emplace_back(PlayerName, socket);
					selector.add(*socket);
				}
				else if (socket->receive(data, sizeof(data), recived))
				{
					std::cout << data;
				}
			}
			else if (!clients.empty())
			{
				for (auto index = 0; index < clients.size(); ++index)
				{
					if (selector.isReady(*clients[index].second))
					{
						sf::Packet packet;
						sf::Packet infoPacket;
						if (clients[index].second->receive(packet) == Socket::Done)
						{
							int type;
							packet >> type;
							if (type == 1)
							{
								std::string msg;
								packet >> msg;
								std::cout << "PacketType = " << type << " Msg: " << msg << std::endl;
								for (auto client : clients)
								{
									infoPacket << msg;
									client.second->send(infoPacket);
								}
							}
							else if (type == 2)
							{
								float xa;
								float xz;
								packet >> xz;
								packet >> xa;
								std::cout << "PacketType = " << type << " PosX: " << xa << " PosY: " << xz << std::endl;
							}
						}
						else
						{
							std::cout << clients[index].first << " disconnected" << std::endl;
							selector.remove(*clients[index].second);
							delete clients[index].second;
							clients.erase(clients.begin() + index);
						}
					}
				}
			}
		}
	}
}

Server::~Server()
{
	for (auto c : clients)
		delete c.second;
}
