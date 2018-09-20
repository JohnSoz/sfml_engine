#include "LogConsole.h"

vector<Console::Log> Console::AppLog::Buffer = {};
const char*          Console::AppLog::items[5] = { "all", "error", "info", "fatal", "system" };
bool                 Console::AppLog::ScrollToBottom = 0;
void Console::AppLog::Draw(const char* title, bool *p_open)
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

			for (auto & item : items)
			{
				bool is_selected = (item_current == item);
				if (ImGui::Selectable(item, is_selected))
					item_current = item;
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
					ImGui::TextColored(item.color, item.text.c_str()); //Test
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

Console::Log::Log(std::string s, logType t)
{
	type = t;
	//boost::posix_time::ptime utcCur = boost::posix_time::second_clock::local_time();//+ std::to_string(utcCur.time_of_day().seconds());
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::string l = "[sec:" + std::to_string(st.wSecond);
	switch (t)
	{
	case logType::error:
		color = ImVec4(1, 0.35, 0, 1);
		l += "type:error]: ";
		break;
	case logType::info:
		color = ImVec4(0, 1, 0.3, 1);
		l += "type:info]: ";
		break;
	case logType::fatal:
		color = ImVec4(1, 0, 0, 1);
		l += "type:fatal]: ";
		break;
	case logType::system:
		color = ImVec4(1, 0, 0.8, 1);
		l += "type:system]: ";
		break;
	}
	l += s;
	l += " \n";
	text = l;
}
