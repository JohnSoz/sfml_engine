#include "DebuggingSystem.h"
#include <windows.h>
using namespace Engine;
#include "LogConsole.h"
#include "staticVariable.h"

sf::RenderWindow* Engine::DebuggingSystem::window = nullptr;

void DebuggingSystem::draw(sf::RenderTarget& target)
{
	if (showVertex)
	{
		int count = 0;
		const float viewPortScale = 1.92;
		sf::Vector2f offset = sf::Vector2f(500, 281.5) - window->getView().getCenter();
		offset.x *= 1.92;
		offset.y *= 1.92;

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

			//500,281.5
			ImGUI::SimpleText(sf::Vector2f(x * 1.92, y * 1.92), &overlay, "D_Window_" + std::to_string(count));
			count++;
			ImGUI::SimpleText(sf::Vector2f(w * 1.92, h * 1.92), &overlay, "D_Window_" + std::to_string(count));
			count++;
			target.draw(triangle);
		}
		for (auto& z : obj) //Оптимизировать
		{
			auto x = z.second.left;
			auto y = z.second.top;
			auto w = z.second.width;
			auto h = z.second.height;

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


			const float posX = x * 1.92;
			const float posY = y * 1.92;
			if ((posX >= abs(offset.x) && posX <= abs(offset.x + 1800)) &&
				(posY >= abs(offset.y) && posY <= abs(offset.y + 930)))
			{
				ImGUI::SimpleText(sf::Vector2f(posX + offset.x, posY + offset.y), &overlay, "D_Window_" + std::to_string(count));
				count++;
			}
			if (((x + w) * 1.92 >= abs(offset.x) && (x + w) * 1.92 <= abs(offset.x + 1800)) &&
				((y + h)  * 1.92 >= abs(offset.y) && (y + h)  * 1.92 <= abs(offset.y + 930)))
			{
				ImGUI::SimpleText(sf::Vector2f((x + w) * 1.92 + offset.x, (y + h)  * 1.92 + offset.y), &overlay, "D_Window_" + std::to_string(count));
				count++;
			}
			if ((posX + w * 1.92 / 2 - 35 >= abs(offset.x) && posX + w * 1.92 / 2 - 35 <= abs(offset.x + 1800)) &&
				(posY + h * 1.92 / 2 - 20 >= abs(offset.y) && posY + h * 1.92 / 2 - 20 <= abs(offset.y + 930)))
			{
				ImGUI::Text(sf::Vector2f(posX + w * 1.92 / 2 - 35 + offset.x, posY + h * 1.92 / 2 - 20 + offset.y), &overlay, "D_Window_" + std::to_string(count), z.first);
				count++;
			}
			target.draw(triangle);
		}
	}
	drawDebugWindows(Engine::VStaticContainer::ShowDebugWindow);
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
			showVertex = !showVertex;
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			Engine::VStaticContainer::ShowDebugWindow = !Engine::VStaticContainer::ShowDebugWindow;
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
		ImGui::SetNextWindowBgAlpha(0.4f);

		if (ImGui::Begin("Overlay", open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Show player position");
			ImGui::Separator();
			if (ImGui::IsMousePosValid())
			{
				ImGui::Text("Window size: (%.1f,%.1f)", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
				ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
				static float values[100] = { 0 };
				static int values_offset = 0;
				static float refresh_time = 0.0f;
				static float middle = 0;
				static float counter = 0;
				while (refresh_time < ImGui::GetTime())
				{
					values[values_offset] = ImGui::GetIO().Framerate;
					values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
					refresh_time += 1.0f / 60.0f;
					middle += ImGui::GetIO().Framerate;
					counter++;
				}
				std::string t = std::to_string(middle / counter);
				if (counter > 250)
				{
					middle = 0;
					counter = 0;
				}
				ImGui::PlotLines("Framerate", values, IM_ARRAYSIZE(values), values_offset, t.c_str(), 200, 400, ImVec2(0, 80));
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

void Engine::ImGUI::SimpleText(sf::Vector2f position, bool * open, std::string name, std::string text)
{
	if (*open)
	{
		ImVec2 window_pos = ImVec2(position.x, position.y);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.6f);
		if (ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::SetWindowFontScale(1);
			if (!text.empty())
				ImGui::Text(text.c_str());
			else
				ImGui::Text("Info: ");
			ImGui::Separator();
			ImGui::Text("PosX: (%.1f)", position.x);
			ImGui::Text("PosY: (%.1f)", position.y);
			ImGui::End();
		}
	}
	else
		return;
}

void Engine::ImGUI::Text(sf::Vector2f position, bool *open, std::string name, std::string text)
{
	if (*open)
	{
		ImVec2 window_pos = ImVec2(position.x, position.y);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.8f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.96, 0.6, 0.46, 1));
		if (ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Spacing();
			ImGui::Text(text.c_str());
			ImGui::Spacing();
			ImGui::End();
		}
		ImGui::PopStyleColor();
	}
	else
		return;
}
