#pragma once
#include <TGUI/TGUI.hpp>
#include <iostream>

using namespace tgui;
using namespace sf;

namespace Engine
{
	typedef std::pair<std::string, std::string> pair_s;

	enum renderPriority { Low = 0, Medium, High };

	std::pair<tgui::Button::Ptr, std::string> makeButton(std::string TEXT, pair_s pos, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr);

	std::pair <tgui::Button::Ptr, std::string> makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr);

	std::pair<tgui::TextBox::Ptr, std::string> makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos);

	std::pair<tgui::ListBox::Ptr, std::string> makeListBox(std::vector<std::string> items, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f = nullptr, int itemHeight = 25);

	tgui::Canvas::Ptr makeCanvas(sf::Sprite spr, sf::Vector2f size = { 1920,1080 });

	tgui::Picture::Ptr makePicture(sf::Texture& tex, sf::Vector2f pos, sf::Vector2f size, float opacity = 1);
}