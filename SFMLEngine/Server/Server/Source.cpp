#include "Server.h"
#include <thread>
#include <future>

#include <mutex>
using namespace std;

int main()
{

	Server server;
	server.Start();
	return 0;
}
