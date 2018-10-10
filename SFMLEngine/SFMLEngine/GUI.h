#pragma once
#ifndef GUI_H
#define GUI_H
#include <TGUI/TGUI.hpp>
#include <iostream>
using namespace tgui;
using namespace sf;

namespace Engine
{
//	typedef void(*func)();
	typedef std::pair<std::string, std::string> pair_s;

	enum renderPriority
	{
		Low = 0,
		Medium,
		Highc
	};


	tgui::Button::Ptr makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr);

	tgui::Button::Ptr makeButton(std::string TEXT, pair_s pos, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr);

	tgui::TextBox::Ptr makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos);

	tgui::Canvas::Ptr makeCanvas(sf::Sprite spr);

	class Layer
	{
	private:
		std::string name;
		tgui::Theme theme;
		tgui::Gui gui;
		//TODO:
		//std::vector<tgui::Gui*> gui;
		renderPriority render;

	public:
		Layer() { setTheme("Data/GUI/MyUI/MainMenu.txt"); }
		Layer(std::string n, sf::RenderWindow& target, std::string pathToTheme, renderPriority p = renderPriority::Low)
		{
			render = p;
			gui.setTarget(target);
			if (!pathToTheme.empty())
				setTheme(pathToTheme);
			else
				setTheme("Data/GUI/MyUI/MainMenu.txt");
			name = n;
		}

		std::string getName() const { return name; }

		void addWidjet(tgui::Widget::Ptr w, std::string type);

		void deleteWidjet(std::string w_name);

		renderPriority getRenderPriority();

		void setRenderPriority(renderPriority r);

		void setTheme(std::string path);


		friend class GUI;
		friend class Menu;
	};

	class GUI
	{
	protected:
		std::vector<Layer*> layers;
		sf::RenderWindow* window;
		bool showGui = true;
	public:
		GUI() = default;
		GUI(const GUI& g) = default;
		GUI(GUI&& g) = default;
		~GUI()
		{
			for (auto & layer : layers)
			{
				delete layer;
			}
			layers.clear();
		}

		explicit
			GUI(sf::RenderWindow& w)
		{
			window = &w;
			addLayer("Default", w, renderPriority::Low);
		}

		explicit
			GUI(std::string pathToTheme, sf::RenderWindow& w)
		{
			window = &w;
			addLayer("Default", w, renderPriority::Low, pathToTheme);
			/*
				sf::Texture t;
				t.loadFromFile("bg.png");
				sf::Sprite s(t);
				layers[0]->addWidjet(makeWidjets::makeCanvas(s), "");
			*/
		}

		void addWidjetToLayer(tgui::Widget::Ptr w, std::string type, std::string nameLayer = "Default");

		void addWidjetToNewLayer(tgui::Widget::Ptr w, std::string type, std::string nameLayer, sf::RenderWindow& window, renderPriority p, std::string pathToTheme = "");

		auto getWidjet(std::string name, int layer) const;

		void handleEvent(sf::Event& e);

		bool update(sf::Event& e);

		void draw();

	private:

		void addLayer(Layer* layer);

		void addLayer(std::string name, sf::RenderWindow &target, renderPriority p, std::string pathToTheme = "");
	};

	class Menu : public GUI
	{
	private:
		
	public:
		Menu() = default;
		Menu(std::string pathToTheme, sf::RenderWindow& w) : GUI(pathToTheme, w) {}
		Menu(const Menu&) = default;

		void makeMenu()
		{
			sf::Texture t;
			t.loadFromFile("Data/images/bg.png");
			sf::Sprite spr(t);
			addWidjetToLayer(makeCanvas(spr), "");
			addWidjetToLayer(makeButton("Run", sf::Vector2f(220, 250), sf::Vector2f(150, 60),
				[&]()
			{
				showGui = false;
			}
			), "Button");
			addWidjetToLayer(makeTextBox("TestMenu", sf::Vector2f(120, 60), sf::Vector2f(960, 40)), "TextBox");
		}
	};
}
#endif