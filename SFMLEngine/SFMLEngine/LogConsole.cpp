#include "LogConsole.h"
#include <cmath>
#include <iostream>

vector<Console::Log> Console::AppLog::Buffer = {};
std::string         Console::AppLog::items[7] = { "all", "error", "info", "fatal", "system" ,"script", "script_result" };
bool                 Console::AppLog::ScrollToBottom = 0;
std::vector<std::string>   Console::AppLog::current_input = {};

void Console::AppLog::Draw(const char* title, bool* p_open, sol::state& state)
{
	if (*p_open)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y * 0.3));
		ImGui::SetNextWindowBgAlpha(0.8f);
		ImGui::Begin(title, p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		static bool find = false;
		static int itemCount = 0;
		ImGui::Checkbox("Find By Type", &find);
		static std::string item_current = items[0];
		ImGui::PushItemWidth(45);
		if (ImGui::BeginCombo("", item_current.c_str(), ImGuiComboFlags_NoArrowButton))
		{
			for (auto& item : items)
			{
				bool is_selected = (item_current == item);
				if (ImGui::Selectable(item.c_str(), is_selected))
					item_current = item;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		char buff_search[256] = { 0 };
		char buff_input[256] = { 0 };

		ImGui::PushItemWidth(150);
		bool value_changed = ImGui::InputText("Filter", buff_search, IM_ARRAYSIZE(buff_search));
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(300);
		bool input_change = ImGui::InputText("Input", buff_input, IM_ARRAYSIZE(buff_input), ImGuiInputTextFlags_EnterReturnsTrue);
		std::string last_result;
		if (input_change)
		{
			ScrollToBottom = true;
			if (buff_input[0] == '/')
			{
				Buffer.emplace_back(buff_input, script);
				buff_input[0] = ' ';
				string do_string = (string)buff_input;
				state.do_string(do_string.c_str());
				auto a = state["lua_result"];
				if (a.valid())
				{
					std::string answer = a.get<std::string>();
					if (last_result != answer)
						Buffer.emplace_back(answer, script_result);
					last_result = answer;
				}
			}
		}
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (copy) ImGui::LogToClipboard();
		if (value_changed)
		{
			std::string input = buff_search;
			for (auto item : Buffer)
			{
				std::string found = "";
				std::size_t start_pos = -1;
				start_pos = item.text.find_first_of(" ");
				for (char i : input)
				{
					std::size_t pos_begin = -1;

					pos_begin = item.text.find_first_of(i, start_pos);
					if (pos_begin != -1)
					{
						found += item.text.substr(pos_begin, 1);
					}
				}
				if (found == input)
				{
					ImGui::TextColored(item.color, item.text.c_str());
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
						ImGui::TextColored(item.color, item.text.c_str());
				}
				else if (item_current == "info")
				{
					if (item.type == logType::info)
						ImGui::TextColored(item.color, item.text.c_str());
				}
				else if (item_current == "fatal")
				{
					if (item.type == logType::fatal)
						ImGui::TextColored(item.color, item.text.c_str());
				}
				else if (item_current == "system")
				{
					if (item.type == logType::system)
						ImGui::TextColored(item.color, item.text.c_str());
				}
				else if (item_current == "script")
				{
					if (item.type == logType::script)
						ImGui::TextColored(item.color, item.text.c_str());
				}
				else if (item_current == "script_result")
				{
					if (item.type == logType::script_result)
						ImGui::TextColored(item.color, item.text.c_str());
				}
			}
		}
		else
		{
			for (auto i : Buffer)
			{
				ImGui::TextColored(i.color, i.text.c_str());
			}
		}
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.f);
		ScrollToBottom = false;
		ImGui::EndChild();
		ImGui::End();
	}

}

Console::Log::Log(std::string s, logType t)
{
	type = t;
	double time = clock();
	std::string ti = std::to_string(std::round(time / 10) / 100);
	ti.erase(ti.find_first_of('.') + 3, ti.size());
	std::string l = "[sec:" + ti;
	switch (t)
	{
	case logType::error:
		color = ImVec4(1, 0.35, 0, 1);
		l += " type:error]: ";
		break;
	case logType::info:
		color = ImVec4(0, 1, 0.3, 1);
		l += " type:info]: ";
		break;
	case logType::fatal:
		color = ImVec4(1, 0, 0, 1);
		l += " type:fatal]: ";
		break;
	case logType::system:
		color = ImVec4(1, 0, 0.8, 1);
		l += " type:system]: ";
		break;
	case logType::script:
		color = ImVec4(0.1, 0.5, 0.1, 1);
		l += " type:script]: ";
		break;
	case logType::script_result:
		color = ImVec4(0.1, 0.5, 0.1, 1);
		l += " type:script_result]: ";
		break;
	}
	l += s;
	l += " \n";
	text = l;
}
