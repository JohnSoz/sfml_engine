#pragma once
#include <TGUI/TGUI.hpp>
#include <iostream>

using namespace tgui;
using namespace sf;

namespace Engine
{
	typedef std::pair<std::string, std::string> pair_s;

	enum renderPriority { Low = 0, Medium, High };

	tgui::Button::Ptr makeButton(std::string TEXT, pair_s pos, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr);

	tgui::Button::Ptr makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr);

	tgui::TextBox::Ptr makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos);

	tgui::ListBox::Ptr makeListBox(std::vector<std::string> items, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f = nullptr, int itemHeight = 25);

	tgui::Canvas::Ptr makeCanvas(std::string_view path, sf::Vector2f size = { 1920,1080 });

	tgui::Picture::Ptr makePicture(std::string_view path, sf::Vector2f pos, sf::Vector2f size, float opacity = 1);

	tgui::Picture::Ptr makePicture(sf::Image tex, sf::Vector2f pos, float opacity = 1);
}