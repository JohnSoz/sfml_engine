#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/System/Export.hpp>

class timer
{
private:
	sf::Clock clock;
	float t;
	float delta;
public:
	timer() { t = delta = 0; }
	float getTime() { return t; }
	void setDelta(float x) { delta = x; };
	void Tick()
	{
		t = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		t = t / delta;
	}
	void restart() 
	{
		t = 0;
		clock.restart();
	}
	bool operator> (float x)
	{
		return t > x;
	}
	bool operator< (float x)
	{
		return !(*this > x);
	}
	bool operator== (float x)
	{
		return t == x;
	}
	bool operator>= (float x)
	{
		return (*this > x) && (*this == x);
	}
	bool operator<= (float x)
	{
		return !(*this >= x);
	}
};