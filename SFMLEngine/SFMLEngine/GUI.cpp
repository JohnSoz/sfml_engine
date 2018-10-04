#include "GUI.h"
#include <assert.h> 
using namespace Engine;


bool operator==(const pair_s& left, const pair_s& right)
{
	return (left.first == right.first && left.second == right.second);
}

void Layer::addWidjet(tgui::Widget::Ptr w, std::string type)
{
	if (!type.empty())
		w->setRenderer(theme.getRenderer(type));
	gui.add(w);
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

void Engine::GUI::addWidjetToLayer(tgui::Widget::Ptr w, std::string type, std::string nameLayer)
{
	for (auto &i : layers)
	{
		if (i->getName() == nameLayer)
			i->addWidjet(w, type);
	}
}

void Engine::GUI::addWidjetToNewLayer(tgui::Widget::Ptr w, std::string type, std::string nameLayer, sf::RenderWindow & window, renderPriority p, std::string pathToTheme)
{
	addLayer(nameLayer, window, p, pathToTheme);
	addWidjetToLayer(w, type, nameLayer);
}

auto Engine::GUI::getWidjet(std::string name, int layer) const
{
	return layers[layer]->gui.get(name);
}

void Engine::GUI::handleEvent(sf::Event & e)
{
	for (auto &i : layers)
	{
		i->gui.handleEvent(e);
	}
}

bool Engine::GUI::update(sf::Event& e)
{
	handleEvent(e);
	return !showGui;
}

void Engine::GUI::draw()
{
	std::sort(layers.begin(), layers.end(), [](Layer* l, Layer* l2) {return (l->getRenderPriority() < l2->getRenderPriority()); });
	for (auto &i : layers)
	{
		i->gui.draw();
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

tgui::Button::Ptr Engine::makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f)
{
	auto Button = tgui::Button::create();
	POS.x -= SIZE.x / 2;
	POS.y -= SIZE.y / 2;
	Button->setPosition(POS);
	Button->setText(TEXT);
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
	if (f)
		Button->connect("Pressed", f);
	Button->setSize(SIZE);
	return Button;
}

tgui::TextBox::Ptr Engine::makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos)
{
	auto textbox = tgui::TextBox::create();
	textbox->setSize(SIZE);
	textbox->disable();
	pos.x -= SIZE.x / 2;
	pos.y -= SIZE.y / 2;
	textbox->setPosition(pos);
	textbox->addText(TEXT);
	return textbox;
}


tgui::Canvas::Ptr Engine::makeCanvas(sf::Sprite spr)
{
	auto canvas = tgui::Canvas::create({ 1920, 1080 });
	canvas->setPosition(0, 0);
	canvas->clear();
	canvas->draw(spr);
	canvas->display();
	return canvas;
}
