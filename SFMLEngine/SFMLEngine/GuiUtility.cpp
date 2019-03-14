#include "GuiUtility.h"
using namespace Engine;
#define TextSize 18

std::pair<tgui::Button::Ptr, std::string> Engine::makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f)
{
	auto Button = tgui::Button::create();
	POS.x -= SIZE.x / 2;
	POS.y -= SIZE.y / 2;
	Button->setPosition(POS);
	Button->setText(TEXT);
	Button->setTextSize(TextSize);
	if (f)
		Button->connect("Pressed", f);
	Button->setSize(SIZE);
	return std::pair(Button, "Button");
}


std::pair<tgui::Button::Ptr, std::string> Engine::makeButton(std::string TEXT, pair_s pos, sf::Vector2f SIZE, std::function<void()> f)
{
	auto Button = tgui::Button::create();
	Button->setPosition(pos.first, pos.second);
	Button->setText(TEXT);
	Button->setTextSize(TextSize);
	Button->setPosition(pos.first, pos.second);
	if (f)
		Button->connect("Pressed", f);
	Button->setSize(SIZE);
	return std::pair(Button, "Button");
}

std::pair<tgui::TextBox::Ptr, std::string> Engine::makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos)
{
	auto textbox = tgui::TextBox::create();
	textbox->setSize(SIZE);
	textbox->setEnabled(true);
	textbox->setTextSize(TextSize);
	pos.x -= SIZE.x / 2;
	pos.y -= SIZE.y / 2;
	textbox->setPosition(pos);
	textbox->addText(TEXT);
	return std::pair(textbox, "TextBox");
}

std::pair<tgui::ListBox::Ptr, std::string> Engine::makeListBox(std::vector<std::string> items, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f, int itemHeight)
{

	auto listBox = tgui::ListBox::create();
	listBox->setSize(SIZE);
	listBox->setItemHeight(itemHeight);
	listBox->setPosition(POS);
	listBox->setTextSize(TextSize);
	for (const auto& item : items)
		listBox->addItem(item);
	if (f != nullptr)
		listBox->connect("ItemSelected", f);
	return std::pair(listBox, "ListBox");
}


tgui::Canvas::Ptr Engine::makeCanvas(sf::Sprite spr, sf::Vector2f size)
{
	auto canvas = tgui::Canvas::create(size);
	canvas->setPosition(0, 0);
	canvas->clear();
	canvas->draw(spr);
	canvas->display();
	return canvas;
}

tgui::Picture::Ptr Engine::makePicture(sf::Texture & tex, sf::Vector2f pos, sf::Vector2f size, float opacity)
{
	auto picture = tgui::Picture::create(tex, true);
	picture->setSize(size);
	picture->setPosition(pos);
	picture->setInheritedOpacity(opacity);
	return picture;
}