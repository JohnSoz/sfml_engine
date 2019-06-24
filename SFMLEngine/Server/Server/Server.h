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
using namespace sf;
using namespace std;

class Server
{
private:
	enum PacketType
	{
		message = 1,
		position,
		status
	};
	TcpListener listener;
	SocketSelector selector;
	vector<pair<string, TcpSocket*>> clients;
	vector<pair<std::string, std::string>> users_debug;
	bool done = false;
	size_t port;
public:
	Server();
	void Start(PyObject* obj);
	int makeUniqueId() { static int id; return ++id; }
	void acceptNewConnectin() {}


	~Server();
};


