#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/System/Export.hpp>
#include <iostream>
#include <iosfwd>
class timer
{
private:
	sf::Clock clock;
	float t;
	float delta;
	bool IsPause;
public:
	timer() { t = delta = 0; IsPause = false; }

	float getTime() { return t; }

	void setDelta(float x) { delta = x; };

	void Tick()
	{
		if (!IsPause)
		{
			t = clock.getElapsedTime().asMicroseconds();
			clock.restart();
			t = t / delta;
		}
	}

	bool isPause() { return IsPause; }

	void pause() { restart(); IsPause = true; }

	void resume() { restart();  IsPause = false; }

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
	friend std::ostream& operator<<(std::ostream &output, const timer &t)
	{
		output << "Time: " << t.t << "  ClockTimeAsSeconds: " << t.clock.getElapsedTime().asSeconds() << std::endl;
		return output;
	}
	operator float()
	{
		return t;
	}
};