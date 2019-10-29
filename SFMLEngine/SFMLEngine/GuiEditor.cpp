#include "GuiEditor.h"
#include "LogConsole.h"
#include "json.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <cstring>
using json = nlohmann::json;

void Engine::GuiEditor::load()
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

void Engine::GuiEditor::saveWindow()
{
	ImGui::BeginChild("SaveWindow", ImVec2(0.f, 0.f));
	strcpy_s(buffer_path, "D:\\!VisualStudioProject\\SfmlProject\\SFMLEngine\\SFMLEngine\\Data\\GUI\\gui.json");
	ImGui::InputText("path", buffer_path, IM_ARRAYSIZE(buffer_path));

	ImGui::Indent(ImGui::GetWindowSize().x - 120.f);
	if (ImGui::Button("Save", { 60, 40 }))
		ptrGui->save(buffer_path);

	ImGui::EndChild();
}

void Engine::GuiEditor::groupViewWindow()
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
		std::string type = w->getWidgetType();
		ImGui::Text(("Type: " + type).c_str());

		if (type == "Picture")
		{
			if (ImGui::TreeNode("Texture"))
			{
				auto pic = w->cast<Picture>();
				auto tex = pic->getRenderer()->getTexture().getData()->texture;
				auto size_x = tex.getSize().x;
				auto size_y = tex.getSize().y;
				ImGui::Text("%ix%i", size_x, size_y);
				ImGui::Image(pic->getRenderer()->getTexture().getData()->texture, sf::Vector2f(size_x, size_y), sf::FloatRect(0, 0, size_x, size_y));
				ImGui::TreePop();
			}
		}
		if (ImGui::TreeNode("Position"))
		{
			auto new_pos = w->getPosition();
			auto w_pos = w->getPosition();

			new_pos.x += w->getSize().x / 2;
			new_pos.y += w->getSize().y / 2;
			w_pos.x += w->getSize().x / 2;
			w_pos.y += w->getSize().y / 2;

			ImGui::DragFloat("Pos.x", &new_pos.x, 1.f, 0.f, 1920.f);
			ImGui::DragFloat("Pos.y", &new_pos.y, 1.f, 0.f, 1080.f);

			if (new_pos != w_pos)
			{
				new_pos.x -= w->getSize().x / 2;
				new_pos.y -= w->getSize().y / 2;
				w->setPosition(new_pos);
			}
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

		float opacity = w->getInheritedOpacity();
		ImGui::PushItemWidth(80.f);
		ImGui::DragFloat("Opacity", &opacity, 0.01f, 0.f, 1.f);
		ImGui::PopItemWidth();
		w->setInheritedOpacity(opacity);

		bool vis = w->isVisible();
		if (ImGui::Checkbox("isVisible", &vis))
			w->setVisible(vis);
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::EndGroup();
}

void Engine::GuiEditor::addWidgetWindow()
{
	enum State { NONE = 0, BUTTON, PICTURE };
	static State state = State::NONE;

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
		if (ImGui::BeginCombo("", item_current.c_str(), ImGuiComboFlags_NoArrowButton))
		{
			for (auto& item : ptrGui->get_group_array())
			{
				bool is_selected = (item_current == item.first);
				if (ImGui::Selectable(item.first.data(), is_selected))
					item_current = item.first;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
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
				ptrGui->groupArray.addWidget(Button, buffer_name, item_current);
				strcpy_s(buffer_name, "");
			}
		}
		break;
		case PICTURE:
		{
			ImGui::InputText("path", buffer_path, IM_ARRAYSIZE(buffer_path));
			ImGui::Indent(ImGui::GetWindowSize().x - 100.f);
			if (ImGui::Button("ADD", { 60.f, 40.f }))
			{
				auto pic = makePicture(buffer_path, pos, size);
				ptrGui->groupArray.addWidget(pic, buffer_name, item_current);
				strcpy_s(buffer_name, "");
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

void Engine::GuiEditor::addGroupWindow()
{
	ImGui::BeginChild("addGroupWindow", ImVec2(0.f, 0.f));
	ImGui::InputText("name", buffer_name, IM_ARRAYSIZE(buffer_name));
	ImGui::Indent(ImGui::GetWindowSize().x - 100.f);
	if (ImGui::Button("ADD", { 60, 40 }))
	{
		ptrGui->groupArray.addGroup(buffer_name);
		ptrGui->gui.add((ptrGui->groupArray.end() - 1)->second);
		strcpy_s(buffer_name, "");
	}
	ImGui::EndChild();
}

void Engine::GuiEditor::drawEditor()
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
