#pragma once
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>
using namespace sf;
using namespace std;

class Server
{
private:
	enum PacketType 
	{
		message=1,
		position,
		status
	};
	TcpListener listener;
	SocketSelector selector;
	vector<pair<string, TcpSocket*>> clients;
	bool done = false;
	size_t port;
public:
	Server();
	void Start();
	~Server();
};


