#include "GuiUtility.h"
using namespace Engine;
#define TextSize 18

tgui::Button::Ptr Engine::makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f)
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
	return Button;
}


tgui::Button::Ptr Engine::makeButton(std::string TEXT, pair_s pos, sf::Vector2f SIZE, std::function<void()> f)
{
	auto Button = tgui::Button::create();
	Button->setPosition(pos.first, pos.second);
	Button->setText(TEXT);
	Button->setTextSize(TextSize);
	Button->setPosition(pos.first, pos.second);
	if (f)
		Button->connect("Pressed", f);
	Button->setSize(SIZE);
	return Button;
}

tgui::TextBox::Ptr Engine::makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos)
{
	auto textbox = tgui::TextBox::create();
	textbox->setSize(SIZE);
	textbox->setEnabled(false);
	textbox->setTextSize(36);
	pos.x -= SIZE.x / 2;
	pos.y -= SIZE.y / 2;
	textbox->setPosition(pos);
	textbox->setText(TEXT);
	return textbox;
}

tgui::ListBox::Ptr Engine::makeListBox(std::vector<std::string> items, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f, int itemHeight)
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
	return listBox;
}


tgui::Canvas::Ptr Engine::makeCanvas(std::string_view path, sf::Vector2f size)
{
	sf::Texture t;
	t.loadFromFile(path.data());
	sf::Sprite spr(t);
	auto canvas = tgui::Canvas::create(size);
	canvas->setUserData(spr);
	canvas->setPosition(0, 0);
	canvas->clear();
	canvas->draw(spr);
	canvas->display();
	return canvas;
}

tgui::Picture::Ptr Engine::makePicture(std::string_view path, sf::Vector2f pos, sf::Vector2f size, float opacity)
{
	sf::Texture tex;
	tex.loadFromFile(path.data());
	auto picture = tgui::Picture::create(tex, true);
	picture->setUserData(tex);
	picture->setSize(size);
	picture->setPosition(pos);
	picture->setInheritedOpacity(opacity);
	return picture;
}

tgui::Picture::Ptr Engine::makePicture(sf::Image tex, sf::Vector2f pos, float opacity)
{
	sf::Texture t;
	t.loadFromImage(tex);
	auto picture = tgui::Picture::create(t, true);
	picture->setSize((sf::Vector2f)t.getSize());
	picture->setPosition(pos);
	picture->setInheritedOpacity(opacity);
	return picture;
}
