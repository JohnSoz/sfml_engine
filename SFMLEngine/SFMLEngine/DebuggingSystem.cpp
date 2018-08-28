#include "DebuggingSystem.h"
#include <windows.h>
using namespace Engine;

sf::RenderWindow* Engine::DebuggingSystem::window = nullptr;
ImGUI::AppLog* Engine::DebuggingSystem::log = new ImGUI::AppLog;

void DebuggingSystem::draw()
{

	int count = 0;
	for (auto &i : entites) 
	{
		//auto x = i.second->x - 90;
		//auto y = i.second->y - 120;
		auto x = i.first->x;
		auto y = i.first->y;
		auto w = i.first->w;
		auto h = i.first->h;

		auto x2 = i.second->x;
		auto y2 = i.second->y;
		auto w2 = i.second->w;
		auto h2 = i.second->h;

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
	log->Draw("Hi", &LogConsole);
}

void Engine::DebuggingSystem::handleEvent(sf::Event& event)
{
	if (event.type = sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::T)
		{
			if (Pressclock.getElapsedTime().asMilliseconds() > 500)
			{
				overlay = !overlay;
				Pressclock.restart();
			}
		}
		if (event.key.code == sf::Keyboard::Tilde)
		{
			if (Pressclock.getElapsedTime().asMilliseconds() > 600)
			{
				LogConsole = !LogConsole;
				Pressclock.restart();
			}
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
				if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
				if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
				if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
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

void Engine::ImGUI::AppLog::addLog(Log log)
{
	Buffer.push_back(log);
}

void Engine::ImGUI::AppLog::Draw(const char * title, bool * p_open)
{
	if (*p_open)
	{
		//ImVec2 window_pos = ImVec2(0, 0);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y * 0.2));
		ImGui::SetNextWindowBgAlpha(0.8f);
		ImGui::Begin(title, p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		static bool find = false;
		ImGui::Checkbox("Find By Type", &find);

		static const char* item_current = items[0];
		ImGui::PushItemWidth(45);
		if (ImGui::BeginCombo("", item_current, ImGuiComboFlags_NoArrowButton))
		{

			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (item_current == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
					item_current = items[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		std::string input;
		char buff[256] = { 0 };
		ImGui::PushItemWidth(280);
		bool value_changed = ImGui::InputText("Filter", buff, IM_ARRAYSIZE(buff));
		ImGui::PopItemWidth();
		input = buff;
		ImGui::Separator();

		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (copy) ImGui::LogToClipboard();

		if (value_changed)
		{
			for (auto item : Buffer)
			{
				std::string found = "";
				std::size_t start_pos = -1;
				start_pos = item.text.find_first_of(" ");
				for (int i = 0; i < input.size(); i++)
				{
					std::size_t pos_begin = -1;

					pos_begin = item.text.find_first_of(input[i], start_pos);
					if (pos_begin != -1)
					{
						found += item.text.substr(pos_begin, 1);
					}
				}
				if (found == input)
				{
					ImGui::TextColored(ImVec4(140, 50, 60, 255), item.text.c_str()); //Test
				}
			}

		}
		else if (find && item_current != "all")
		{
			for (auto item : Buffer)
			{
				if (item_current == "error")
				{
					if (item.type == logType::error)
						ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
				}
				else if (item_current == "info")
				{
					if (item.type == logType::info)
						ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
				}
				else if (item_current == "fatal")
				{
					if (item.type == logType::fatal)
						ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
				}
				else if (item_current == "system")
				{
					if (item.type == logType::system)
						ImGui::TextColored(ImVec4(0, 0, 100, 255), item.text.c_str());
				}

			}
		}
		else
		{
			for (auto i : Buffer)
				ImGui::TextColored(i.color, i.text.c_str()); //Test
		}

		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::EndChild();
		ImGui::End();
	}
}

Engine::ImGUI::Log::Log(std::string s, ImVec4 c, logType t)
{
	type = t;
	color = c;
	//boost::posix_time::ptime utcCur = boost::posix_time::second_clock::local_time();//+ std::to_string(utcCur.time_of_day().seconds());
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::string l = "[sec:" + std::to_string(st.wSecond);
	switch (t)
	{
	case Engine::ImGUI::error:
		l += "type:error]: ";
		break;
	case Engine::ImGUI::info:
		l += "type:info]: ";
		break;
	case Engine::ImGUI::fatal:
		l += "type:fatal]: ";
		break;
	case Engine::ImGUI::system:
		l += "type:system]: ";
		break;
	}
	l += s;
	l += " \n";
	text = l;
}
