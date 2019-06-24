#include "Server.h"
#include <future>
#include <algorithm>
using namespace std;

Server::Server()
{
	cout << "Server Running" << endl;
	auto z = listener.listen(300);
	selector.add(listener);
}

void Server::Start(PyObject* pModule)
{
	std::cout << "Server.Start()" << endl;
	auto time_prev = std::chrono::system_clock::now();
	std::string last_user;
	sf::Clock c;
	while (true)
	{
		if (c.getElapsedTime().asSeconds() > 30) {
			auto pFunc2 = PyObject_GetAttrString(pModule, "setStatus");
			if (pFunc2 && PyCallable_Check(pFunc2))
			{
				auto min = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::system_clock::now() - time_prev).count();
				std::string status = "uptime: " + std::to_string(min) + "|users: " + std::to_string(clients.size()) + "|lastUser: " + last_user;
				PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(status.c_str()));
				PyObject_CallObject(pFunc2, args);
				cout << status << endl;
			}
			Py_XDECREF(pFunc2);
			c.restart();
		}
		if (selector.wait(sf::seconds(220)))
		{
			if (selector.isReady(listener))
			{
				std::cout << 11111 << endl;
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
					auto ip = socket->getRemoteAddress().toString();
					ip.erase(ip.find_last_of('.'), ip.size());
					ip += ".***";
					std::string serverInfo = "Connect success";
					last_user = PlayerName + ",ip " + ip;
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
			else
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
									if (client != clients[index])
									{
										infoPacket << msg;
										client.second->send(infoPacket);
									}
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
