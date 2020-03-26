#include <thread>
#include <future>
#include <mutex>
#include <SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace sf;

enum PacketType : uint8_t
{
	newConnection = 0,
	syncClient,
	disconnection,
	updateData
};

uint32_t makeUniqueId()
{
	static int id;
	return std::hash<std::string>()("Player#" + std::to_string(++id));
}

class Client
{
public:
	Client(uint32_t name, TcpSocket* sock) : hash_name(name), socket(sock) {}
	uint32_t hash_name;
	TcpSocket* socket;
	bool isDisconnected = false;
	~Client() { delete socket; }
};

int main()
{
	std::vector<std::unique_ptr<Client>> clients;
	std::thread([&clients]()
		{
			while (true)
			{
				bool needClear = false;
				std::mutex m;
				std::lock_guard<std::mutex> mu(m);
				for (auto& c : clients)
				{
					if (c->socket != nullptr && !c->isDisconnected)
					{
						sf::Packet p;
						sf::Socket::Status status;
						while (status = c->socket->receive(p), status == Socket::Done)
						{
							uint8_t type;
							p >> type;
							if (type == PacketType::updateData)
							{
								float x, y;
								p >> x >> y;
								p.clear();
								p << PacketType::updateData << c->hash_name << x << y;
								for (auto& c2 : clients)
									if (c2->socket != c->socket)
										c2->socket->send(p);
							}
							if (type == PacketType::syncClient)
							{
								uint32_t id;
								p >> id;
								if (auto it = std::find_if(clients.begin(), clients.end(), [id](auto& c)
									{
										return c->hash_name == id;
									}); it != clients.end())
								{
									float x, y;
									p >> x >> y;
									p.clear();
									p << PacketType::syncClient << c->hash_name << x << y;
									(*it)->socket->send(p);
								}
							}
							if (type == PacketType::disconnection)
							{
								p.clear();
								p << PacketType::disconnection << c->hash_name;
								for (auto& c2 : clients)
									if (c2->socket != c->socket)
										c2->socket->send(p);
							}
						}
						if (status == sf::Socket::Disconnected)
						{
							needClear = true;
							c->isDisconnected = true;
						}
					}
				}
				if (needClear)
					clients.erase(std::remove_if(clients.begin(), clients.end(), [](auto& c) {return c->isDisconnected; }), clients.end());
			}
		}).detach();

		TcpListener listener;
		listener.listen(300);

		while (true)
		{
			std::mutex m;
			std::lock_guard<std::mutex> mu(m);
			if (sf::TcpSocket* sock = new sf::TcpSocket(); listener.accept(*sock) == sf::Socket::Done)
			{
				std::cout << "New connection received from " << sock->getRemoteAddress();
				auto id = makeUniqueId();
				std::cout << " now has name " << std::to_string(id) << "\n";

				if (sf::Packet p; sock->receive(p) == Socket::Done)
				{
					float x, y;
					p >> x >> y;
					p.clear();
					p << PacketType::newConnection << id << x << y;
					for (auto& c2 : clients)
					{
						if (c2->socket != sock)
							c2->socket->send(p);
					}
				}
				else
				{
					std::cout << "The information packet was not received from the new connection \n";
				}

				sock->setBlocking(false);
				clients.emplace_back(std::make_unique<Client>(id, sock));
			}
			else
				delete sock;
		}
		//PyObject* pName, * pModule;
		//PyObject* pArgs, * pValue;
		//int i;
		//Server server;
		//Py_Initialize();
		//pName = PyUnicode_FromString("main");
		//pModule = PyImport_Import(pName);
		//Py_DECREF(pName);
		///* Error checking of pName left out */
		//if (pModule != NULL) {

		//	auto fut = std::async(std::launch::async, &Server::Start, &server, pModule);
		//	fut.wait();
		//	Py_DECREF(pModule);
		//}
		//else {
		//	PyErr_Print();
		//}
		//Py_Finalize();
		return 0;
}
