#include <SFML/Graphics.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"
using namespace sf;
using namespace std;

float Distance(sf::Vector2f v2, sf::Vector2f v1)
{
	return sqrtf(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2));
}

float raycast(sf::Vector2f start, float end, sf::FloatRect object)
{
	if (object.contains(start.x, start.y + end))
	{
		return Distance(start, { start.x,object.top - 30 });
	}
	return 0;
}

int main()
{
	RenderWindow w(VideoMode(800, 800), "Raycast", Style::Close);
	ImGui::SFML::Init(w);

	Event evt;

	Vector2f vec(200, 200);
	sf::Clock deltaClock;
	float angle = 0;
	sf::RectangleShape rect({ 50,60 });
	rect.setOrigin(25, 30);
	rect.setFillColor(sf::Color::Black);
	rect.setPosition(400, 300);
	while (w.isOpen())
	{
		while (w.pollEvent(evt))
		{
			ImGui::SFML::ProcessEvent(evt);
		}

		ImGui::SFML::Update(w, deltaClock.restart());
		ImGui::SetNextWindowSize({ 300,200 });
		ImGui::Begin("debug");

		ImGui::BeginChild("objects", ImVec2(100, 0), true);
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("wqe", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
		ImGui::DragFloat("PosX", &vec.x, 1.f, -999.f, 999.f);
		ImGui::DragFloat("PosY", &vec.y, 1.f, -999.f, 999.f);
		ImGui::DragFloat("angle", &angle, 1.f, -999.f, 999.f);
		ImGui::EndChild();
		ImGui::End();

		w.clear(sf::Color::White);
		ImGui::SFML::Render(w);
		VertexArray vs(Lines, 2);
		vs[0].position = vec;
		vs[1].position = { vec.x,vec.y + angle };
		vs[0].color = Color::Red;
		vs[1].color = Color::Red;
		cout << "1 " << raycast(vec, angle, rect.getGlobalBounds()) << endl;
		if (rect.getGlobalBounds().contains(vec.x, vec.y + angle))
		{
			cout << "2 " << Distance(vec, { vec.x,rect.getPosition().y - rect.getOrigin().y }) << endl;
		}
		w.draw(vs);
		w.draw(rect);
		w.display();
	}
}






//#include "Server.h"
//#include <thread>
//#include <future>
//
//#include <mutex>
//using namespace std;
//
//int main()
//{
//
//	Server server;
//	server.Start();
//	return 0;
//}
