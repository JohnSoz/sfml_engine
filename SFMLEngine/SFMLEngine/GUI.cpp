#include "GUI.h"
#include <assert.h> 
using namespace Engine;
#define TextSize 18

bool operator==(const pair_s& left, const pair_s& right)
{
	return (left.first == right.first && left.second == right.second);
}

void Layer::addWidjet(tgui::Widget::Ptr w, std::string type, std::string widgetName)
{
	if (!type.empty())
		w->setRenderer(theme.getRenderer(type));
	tgui::ScrollablePanel s;
	gui.add(w, widgetName);
}

void Engine::Layer::addWidjet(std::pair<tgui::Widget::Ptr, std::string> widget, std::string widgetName)
{
	if (!widget.second.empty())
		widget.first->setRenderer(theme.getRenderer(widget.second));
	gui.add(widget.first, widgetName);
}

void Engine::Layer::deleteWidjet(std::string w_name)

{
	gui.remove(gui.get(w_name));
}

renderPriority Engine::Layer::getRenderPriority()
{
	return render;
}

void Engine::Layer::setRenderPriority(renderPriority r)
{
	render = r;
}

void Engine::Layer::setTheme(std::string path)
{
	theme.load(path);
}

void Engine::GUI::addWidjetToLayer(tgui::Widget::Ptr w, std::string type, std::string widgetName, std::string nameLayer)
{
	for (auto &i : layers)
	{
		if (i->getName() == nameLayer)
			i->addWidjet(w, type, widgetName);
	}
}

void Engine::GUI::addWidjetToLayer(std::pair<tgui::Widget::Ptr, std::string> widget, std::string widgetName, std::string nameLayer /*= "Default"*/)
{
	for (auto &i : layers)
	{
		if (i->getName() == nameLayer)
			i->addWidjet(widget, widgetName);
	}
}

void Engine::GUI::addWidjetToNewLayer(std::pair<tgui::Widget::Ptr, std::string> widget, std::string widgetName, std::string nameLayer, sf::RenderWindow & window, renderPriority p, std::string pathToTheme)
{
	addLayer(nameLayer, window, p, pathToTheme);
	addWidjetToLayer(std::pair(widget.first, widget.second), widgetName, nameLayer);
}

void Engine::GUI::addWidjetToNewLayer(tgui::Widget::Ptr widget, std::string widgetName, std::string nameLayer, sf::RenderWindow& window, renderPriority p, std::string pathToTheme /*= ""*/)
{
	addLayer(nameLayer, window, p, pathToTheme);
	addWidjetToLayer(widget, "", widgetName, nameLayer);
}

void Engine::GUI::handleEvent(sf::Event & e)
{
	for (auto &i : layers)
	{
		i->gui.handleEvent(e);
	}
}

int Engine::GUI::update()
{
	return showGui;
}

int Engine::MainMenu::update()
{
	return state;
}

void Engine::GUI::draw()
{
	if (showGui)
	{
		std::sort(layers.begin(), layers.end(), [](Layer* l, Layer* l2) {return (l->getRenderPriority() < l2->getRenderPriority()); });
		for (auto &item : layers)
		{
			item->gui.draw();
		}
	}
}

void Engine::GUI::addLayer(Layer * layer)
{
	layers.push_back(layer);
}

void Engine::GUI::addLayer(std::string name, sf::RenderWindow & target, renderPriority p, std::string pathToTheme)
{
	layers.push_back(new Layer(name, target, pathToTheme, p));
}

std::pair<tgui::Button::Ptr, std::string> Engine::makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f, std::string type)
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
	return std::pair(Button, type);
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

std::pair<tgui::TextBox::Ptr, std::string> Engine::makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos)
{
	auto textbox = tgui::TextBox::create();
	textbox->setSize(SIZE);
	textbox->setEnabled(false);
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
	for (auto item : items)
		listBox->addItem(item);
	if (f != nullptr)
		listBox->connect("ItemSelected", f);
	return std::pair<tgui::ListBox::Ptr, std::string>(listBox, "ListBox");
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

tgui::Picture::Ptr Engine::makePicture(sf::Texture& tex, sf::Vector2f pos, sf::Vector2f size, float opacity)
{
	auto picture = tgui::Picture::create(tex, true);
	picture->setSize(size);
	picture->setPosition(pos);
	picture->setInheritedOpacity(opacity);
	return picture;
}

void Engine::MainMenu::makeMenu(std::string &p)
{
	sf::Texture t;
	t.loadFromFile("Data/images/bg.png");
	sf::Sprite spr(t);
	addWidjetToLayer(makeCanvas(spr), "", "BackGround");
	auto z = window->getSize();

	addWidjetToLayer(makeButton("Run", sf::Vector2f(220, 250), sf::Vector2f(150, 60),
		[&]()
	{
		showGui = false;
		state = appState::Play;
	}
	), "RunButton");

	addWidjetToLayer(makeButton("Exit", sf::Vector2f(380, 250), sf::Vector2f(150, 60),
		[&]()
	{
		showGui = false;
		state = appState::Exits;
	}
	), "ExitButton");

	addWidjetToLayer(makeListBox({ }, sf::Vector2f(400, 600), sf::Vector2f(250, 150),
		[&]()
	{
		p += "Data/Level/";
		p += getWidjet<tgui::ListBox>("TestListBox2")->getSelectedItem();
	}), "TestListBox2");

	tgui::ListBox::Ptr listBox = getWidjet<tgui::ListBox>("TestListBox2");
	std::string path = "Data\\Level";
	for (auto & p : fs::directory_iterator(path))
	{
		std::string fileName = p.path().filename().generic_string();
		const size_t pos = fileName.find_last_of(".");
		if (pos > 0 && pos < 100)
		{
			fileName.erase(pos, 4);
			listBox->addItem(fileName);
		}
	}
	addWidjetToLayer(makeTextBox("TestMenu", sf::Vector2f(120, 60), sf::Vector2f(960, 40)), "RandomBox");
}