#pragma once
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>
#include "imgui.h"
#include "imgui-SFML.h"
#include <Python.h>
#include <future>
using namespace sf;
using namespace std;

class Server
{
private:
	enum PacketType
	{
		connection = 0,
		newConnection,
		disconnection,
		updateData,
		getData,
		userList,
		sync
	};
	TcpListener listener;
	SocketSelector selector;
	TcpSocket* clients[2];
	vector<pair<std::string, std::string>> users_debug;
	bool done = false;
	size_t port;
	std::atomic<int> i = 0;
public:
	Server();
	void Start(PyObject* obj);
	int makeUniqueId() { static int id; return ++id; }
	void acceptNewConnectin() {}

	~Server();
};


