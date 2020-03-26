#include "Server.h"

#include <algorithm>
using namespace std;

Server::Server()
{
	cout << "Server Running" << endl;
	listener.listen(300);
	listener.setBlocking(true);
}

void Server::Start(PyObject* pModule)
{
	std::cout << "Server.Start()" << endl;
	sf::Clock c;
	std::thread([this]()
		{
			std::mutex m;
			std::lock_guard<std::mutex> mu(m);
			while (true)
			{
				
			}
		}).detach();

		std::thread([this]()
			{
				
			}).detach();
}
//}
//	}
//}


Server::~Server()
{
	/*for (auto c : clients)
		delete c.second;*/
}
