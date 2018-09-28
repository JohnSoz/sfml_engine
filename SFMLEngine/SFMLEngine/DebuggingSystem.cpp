#include "DebuggingSystem.h"
#include <windows.h>
using namespace Engine;
#include "LogConsole.h"
sf::RenderWindow* Engine::DebuggingSystem::window = nullptr;

void DebuggingSystem::draw()
{
	if (showVertex)
	{
		int count = 0;
		for (auto &i : entites)
		{
			//auto x = i.second->x - 90;
			//auto y = i.second->y - 120;
			auto x = i.first->left;
			auto y = i.first->top;
			auto w = i.first->width;
			auto h = i.first->height;

			auto x2 = i.second->left;
			auto y2 = i.second->top;
			auto w2 = i.second->width;
			auto h2 = i.second->height;

			sf::VertexArray triangle(sf::LinesStrip, 5);
			triangle[0] = sf::Vector2f(x, y);
			triangle[0].color = sf::Color::Red;

			triangle[1].position = sf::Vector2f(w2, h2);
			triangle[1].color = sf::Color::Blue;

			triangle[2].position = sf::Vector2f(w, h);
			triangle[2].color = sf::Color::Red;

			triangle[3].position = sf::Vector2f(x2, y2);
			triangle[3].color = sf::Color::Blue;

			triangle[4].position = sf::Vector2f(x, y);
			triangle[4].color = sf::Color::Red;

			ImGUI::SimpleText(sf::Vector2f(x - 102, y - 70), &overlay, "D_Window_" + std::to_string(count));
			count++;
			ImGUI::SimpleText(sf::Vector2f(w, h), &overlay, "D_Window_" + std::to_string(count));
			count++;

			window->draw(triangle);
		}
		for (auto i : obj) //Оптимизировать
		{
			auto x = i.left;
			auto y = i.top;
			auto w = i.width;
			auto h = i.height;

			sf::VertexArray triangle(sf::LinesStrip, 5);
			triangle[0] = sf::Vector2f(x, y);
			triangle[0].color = sf::Color::Blue;

			triangle[1].position = sf::Vector2f(x + w, y);
			triangle[1].color = sf::Color::Blue;

			triangle[2].position = sf::Vector2f(x + w, y + h);
			triangle[2].color = sf::Color::Blue;

			triangle[3].position = sf::Vector2f(x, y + h);
			triangle[3].color = sf::Color::Blue;

			triangle[4].position = sf::Vector2f(x, y);
			triangle[4].color = sf::Color::Blue;

			ImGUI::SimpleText(sf::Vector2f(x, y), &overlay, "D_Window_" + std::to_string(count));
			count++;
			ImGUI::SimpleText(sf::Vector2f(x + w, y + h), &overlay, "D_Window_" + std::to_string(count));
			count++;

			window->draw(triangle);
		}
	}
	Console::AppLog::Draw("LogConsole", &LogConsole);
}

void Engine::DebuggingSystem::handleEvent(sf::Event& event)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
	{
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			overlay = !overlay;
			Pressclock.restart();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			showVertex= !showVertex;
			Pressclock.restart();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
	{
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			LogConsole = !LogConsole;
			Pressclock.restart();
		}
	}
}

void Engine::ImGUI::ShowHelpMarker(const char * desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Engine::ImGUI::SimpleOverlay(bool * open)
{
	if (*open)
	{
		const float DISTANCE = 10.0f;
		static int corner = 1;

		ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		if (corner != -1)
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(0.3f);

		if (ImGui::Begin("Overlay", open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Show player position");
			ImGui::Separator();
			if (ImGui::IsMousePosValid())
			{
				ImGui::Text("Window size: (%.1f,%.1f)", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
				//ImGui::Text("Framerate: (%.1f)", ImGui::GetIO().Framerate);
				ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
				static float values[90] = { 0 };
				static int values_offset = 0;
				static float refresh_time = 0.0f;
				//if (!animate || refresh_time == 0.0f)
				//	refresh_time = ImGui::GetTime();
				while (refresh_time < ImGui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
				{
					values[values_offset] = ImGui::GetIO().Framerate;
					values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
					refresh_time += 1.0f / 60.0f;
				}
				ImGui::PlotLines("Framerate", values, IM_ARRAYSIZE(values), values_offset, "avg 200 - 400", 200, 400, ImVec2(0, 80));
			}
			else
				ImGui::Text("Player Position: <invalid>");
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", nullptr, corner == -1)) corner = -1;
				if (ImGui::MenuItem("Top-left", nullptr, corner == 0)) corner = 0;
				if (ImGui::MenuItem("Top-right", nullptr, corner == 1)) corner = 1;
				if (ImGui::MenuItem("Bottom-left", nullptr, corner == 2)) corner = 2;
				if (ImGui::MenuItem("Bottom-right", nullptr, corner == 3)) corner = 3;
				if (*open && ImGui::MenuItem("Close")) *open = false;
				ImGui::EndPopup();
			}
			ImGui::End();
		}
	}
}

void Engine::ImGUI::SimpleText(sf::Vector2f position, bool * open, std::string name)
{
	if (*open)
	{
		ImVec2 window_pos = ImVec2(position.x, position.y);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.6f);
		ImGui::SetNextWindowSize(ImVec2(102, 70));
		if (ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::SetWindowFontScale(1);
			ImGui::Text("Info:");
			ImGui::Separator();
			ImGui::Text("PosX: (%.1f)", position.x);
			ImGui::Text("PosY: (%.1f)", position.y);
			ImGui::End();
		}
	}
	else
		return;
}
