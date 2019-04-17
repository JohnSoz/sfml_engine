#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "GUI.h"
#include "LogConsole.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>
using json = nlohmann::json;

namespace Engine
{
	class GuiEditor
	{
		BaseGui* ptrGui;

		enum MenuState { GROUPVIEW = 0, ADDWIDGET, ADDGROUP, SAVE, LOAD };

		MenuState menuState = GROUPVIEW;
		char buffer_name[256];
		char buffer_groupType[256];
		char buffer_path[256];

	public:
		bool isEnable = true;

		GuiEditor() = delete;

		GuiEditor(BaseGui& b) : ptrGui(&b)
		{
			buffer_name[0] = '\0';
			buffer_groupType[0] = '\0';
			buffer_path[0] = '\0';
		}


		void load()
		{
			ImGui::BeginChild("LoadWindow", ImVec2(0.f, 0.f));
			ImGui::InputText("path", buffer_path, IM_ARRAYSIZE(buffer_path));

			ImGui::Indent(ImGui::GetWindowSize().x - 120.f);
			if (ImGui::Button("Save", { 60, 40 }))
			{
				ptrGui->load(buffer_path);
			}
			ImGui::EndChild();
		}

		void saveWindow()
		{
			ImGui::BeginChild("SaveWindow", ImVec2(0.f, 0.f));
			ImGui::InputText("path", buffer_path, IM_ARRAYSIZE(buffer_path));

			ImGui::Indent(ImGui::GetWindowSize().x - 120.f);
			if (ImGui::Button("Save", { 60, 40 }))
			{
				ptrGui->save(buffer_path);
			}
			ImGui::EndChild();
		}

		void groupViewWindow()
		{
			static std::string selected = "";
			static std::string groupName = "";
			static bool is_prev_treeNode_open = false;
			ImGui::BeginChild("Layers", ImVec2(250, 0), true);
			for (auto i : ptrGui->groupArray)
			{
				if (ImGui::TreeNode(i.first.c_str()))
				{
					is_prev_treeNode_open = true;
					for (auto i2 : ptrGui->groupArray.get(i.first)->getWidgetNames())
						if (ImGui::Selectable(static_cast<std::string>(i2).c_str()))
						{
							selected = i2;
							groupName = i.first;
						}
					ImGui::TreePop();
				}
				else if (!is_prev_treeNode_open)
				{
					selected = "";
					groupName = "";
				}
			}

			ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginGroup();
			ImGui::BeginChild("Widgets Info", ImVec2(0, 0), true);
			if (!selected.empty())
			{
				auto w = ptrGui->groupArray.get(groupName)->get(selected);

				if (ImGui::TreeNode("Position"))
				{
					sf::Vector2f pos = w->getPosition();
					ImGui::DragFloat("Pos.x", &pos.x, 1.f, 0.f, 1920.f);
					ImGui::DragFloat("Pos.y", &pos.y, 1.f, 0.f, 1080.f);
					if (pos != w->getPosition())
						w->setPosition(pos);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Size"))
				{
					sf::Vector2f size = w->getSize();
					ImGui::DragFloat("Size.x", &size.x, 1.f, 0.f, 1920.f);
					ImGui::DragFloat("Size.y", &size.y, 1.f, 0.f, 1080.f);
					if (size != w->getSize())
						w->setSize(size);
					ImGui::TreePop();
				}

				bool vis = w->isVisible();
				if (ImGui::Checkbox("isVisible", &vis))
					w->setVisible(vis);
			}
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::EndGroup();
		}

		void addWidgetWindow()
		{
			enum State { NONE = 0, BUTTON, PICTURE };
			static State state = NONE;

			static sf::Vector2f pos;
			static sf::Vector2f size;

			ImGui::BeginChild("Widget Type", ImVec2(250.f, 0.f), true);
			if (ImGui::Selectable("Add Button", state == BUTTON))
			{
				state = BUTTON;
			}
			if (ImGui::Selectable("Add Picture", state == PICTURE))
			{
				state = PICTURE;
			}

			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginGroup();
			ImGui::BeginChild("property", ImVec2(0.f, 0.f), true);
			if (state != NONE)
			{
				ImGui::InputText("name", buffer_name, IM_ARRAYSIZE(buffer_name));
				ImGui::InputText("Group Name", buffer_groupType, IM_ARRAYSIZE(buffer_groupType));
				if (ImGui::TreeNode("Position"))
				{
					ImGui::DragFloat("Pos.x", &pos.x, 1.f, 0.f, 999.f);
					ImGui::DragFloat("Pos.y", &pos.y, 1.f, 0.f, 999.f);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Size"))
				{
					ImGui::DragFloat("Size.x", &size.x, 1.f, 0.f, 999.f);
					ImGui::DragFloat("Size.y", &size.y, 1.f, 0.f, 999.f);
					ImGui::TreePop();
				}

				switch (state)
				{
				case BUTTON:
				{
					ImGui::Indent(ImGui::GetWindowSize().x - 100.f);
					if (ImGui::Button("ADD", { 60.f, 40.f }))
					{
						auto Button = makeButton(buffer_name, pos, size);
						ptrGui->groupArray.addWidget(Button, buffer_name, buffer_groupType);
						strcpy_s(buffer_name, "");
						strcpy_s(buffer_groupType, "");
					}
				}
				break;
				case PICTURE:
				{
					ImGui::InputText("path", buffer_path, IM_ARRAYSIZE(buffer_path));
					ImGui::Indent(ImGui::GetWindowSize().x - 100.f);
					if (ImGui::Button("ADD", { 60.f, 40.f }))
					{
						sf::Texture t;
						t.loadFromFile(buffer_path);
						auto pic = makePicture(t, pos, size);
						ptrGui->groupArray.addWidget(pic, buffer_name, buffer_groupType);
						strcpy_s(buffer_name, "");
						strcpy_s(buffer_groupType, "");
						strcpy_s(buffer_path, "");
					}
				}
				break;
				default:;
				}
			}

			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::EndGroup();
		}

		void addGroupWindow()
		{
			ImGui::BeginChild("addGroupWindow", ImVec2(0.f, 0.f));
			ImGui::InputText("name", buffer_name, IM_ARRAYSIZE(buffer_name));
			ImGui::Indent(ImGui::GetWindowSize().x - 100.f);
			if (ImGui::Button("ADD", { 60, 40 }))
			{
				ptrGui->groupArray.addGroup(buffer_name);
				ptrGui->gui.add((ptrGui->groupArray.end() - 1)->second);
				strcpy(buffer_name, "");
			}
			ImGui::EndChild();
		}

		void drawEditor()
		{
			ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin("Gui Editor", &isEnable))
			{
				ImGui::End();
				return;
			}
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Group View", "", menuState == GROUPVIEW))
						menuState = GROUPVIEW;

					if (ImGui::MenuItem("AddWidget", "", menuState == ADDWIDGET))
						menuState = ADDWIDGET;

					if (ImGui::MenuItem("AddGroup", "", menuState == ADDGROUP))
						menuState = ADDGROUP;

					if (ImGui::MenuItem("Save", "", menuState == SAVE))
						menuState = SAVE;

					if (ImGui::MenuItem("Load", "", menuState == LOAD))
						menuState = LOAD;

						ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			switch (menuState)
			{
			case ADDWIDGET:
				addWidgetWindow();
				break;
			case ADDGROUP:
				addGroupWindow();
				break;
			case SAVE:
				saveWindow();
				break;
			case GROUPVIEW:
				groupViewWindow();
				break;
			case LOAD:
				load();
				break;
			}
			ImGui::End();
		}
	};
}
