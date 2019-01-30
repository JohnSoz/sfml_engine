#pragma once
#ifndef GUI_H
#define GUI_H

#include <cassert>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include <filesystem>
#include "ApplicationState.h"
#include "LogConsole.h"
#include <boost/type_index.hpp>
#include "EngineEvents.h"
#include <entityx/entityx.h>
using namespace tgui;
using namespace sf;
using boost::typeindex::type_id_with_cvr;
namespace fs = std::experimental::filesystem;

namespace Engine
{
	typedef std::pair<std::string, std::string> pair_s;

	enum renderPriority { Low = 0, Medium, High };

	tgui::Button::Ptr makeButton(std::string TEXT, pair_s pos, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr);

	std::pair<tgui::Button::Ptr, std::string> makeButton(std::string TEXT, sf::Vector2f POS, sf::Vector2f SIZE = { 50,40 }, std::function<void()> f = nullptr, std::string type = "Button");

	std::pair<tgui::TextBox::Ptr, std::string> makeTextBox(std::string TEXT, sf::Vector2f SIZE, sf::Vector2f pos);

	std::pair<tgui::ListBox::Ptr, std::string> makeListBox(std::vector<std::string> items, sf::Vector2f POS, sf::Vector2f SIZE, std::function<void()> f = nullptr, int itemHeight = 25);

	tgui::Canvas::Ptr makeCanvas(sf::Sprite spr, sf::Vector2f size = { 1920,1080 });

	tgui::Picture::Ptr makePicture(sf::Texture& tex, sf::Vector2f pos, sf::Vector2f size, float opacity = 1);

	class Layer
	{
	private:
		std::string name;
		tgui::Theme theme;
		tgui::Gui gui;
		renderPriority render;
		bool isEnable;
	public:
		Layer() { setTheme("Data/GUI/MyUI/MainMenu.txt"); }
		Layer(std::string n, sf::RenderWindow& target, std::string pathToTheme, renderPriority p = renderPriority::Low)
		{
			isEnable = true;
			render = p;
			gui.setTarget(target);
			sf::Font font;
			font.loadFromFile("Data/Fonts/Bricks.otf");
			gui.setFont(font);
			if (!pathToTheme.empty())
				setTheme(pathToTheme);
			else
				setTheme("Data/GUI/MyUI/MainMenu.txt");
			name = n;
		}

		std::string getName() const { return name; }

		void addWidjet(tgui::Widget::Ptr w, std::string type, std::string widgetName);

		void addWidjet(std::pair<tgui::Widget::Ptr, std::string> widget, std::string widgetName);

		void deleteWidjet(std::string w_name);

		void moveAllWidgets(sf::Vector2f offset)
		{
			for (auto& item : gui.getWidgets())
				item->setPosition(item->getPosition() + offset);
		}

		void disableAllWidgets()
		{
			isEnable = false;
			for (auto& item : gui.getWidgets())
			{
				item->setEnabled(false);
				item->setVisible(false);
			}
		}

		void enableAllWidgets()
		{
			isEnable = true;
			for (auto& item : gui.getWidgets())
			{
				item->setEnabled(true);
				item->setVisible(true);
			}
		}

		bool IsEnable()
		{
			return isEnable;
		}

		renderPriority getRenderPriority();

		void setRenderPriority(renderPriority r);

		void setTheme(std::string path);


		friend class GUI;
		friend class MainMenu;
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
		bool& getShowGui() { return showGui; }
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
		}

		void addWidjetToLayer(tgui::Widget::Ptr w, std::string type, std::string widgetName, std::string nameLayer = "Default");

		void addWidjetToLayer(std::pair<tgui::Widget::Ptr, std::string> widget, std::string widgetName, std::string nameLayer = "Default");

		void addWidjetToNewLayer(tgui::Widget::Ptr widget, std::string widgetName, std::string nameLayer, sf::RenderWindow& window, renderPriority p, std::string pathToTheme = "");

		void addWidjetToNewLayer(std::pair<tgui::Widget::Ptr, std::string> widget, std::string widgetName, std::string nameLayer, sf::RenderWindow& window, renderPriority p, std::string pathToTheme = "");

		template<class T>
		typename T::Ptr getWidjet(std::string name, int layer = -1)
		{
			for (auto& layer : layers)
			{
				for (const auto& names : layer->gui.getWidgetNames())
				{
					if (names == name)
						return layer->gui.get<T>(name);
				}
			}
			std::string log = "getWidjet return nullptr(T: ";
			log += type_id_with_cvr<T>().pretty_name();
			log += ", name: " + name + ")";
			Console::AppLog::addLog(log, Console::error);
		}

		void handleEvent(sf::Event& e);

		virtual int update();

		void draw();

		void addLayer(Layer* layer);

		void addLayer(std::string name, sf::RenderWindow &target, renderPriority p, std::string pathToTheme = "");
	};

	class MainMenu : public GUI
	{
	private:
		int state = appState::UI;
	public:
		MainMenu() = default;
		MainMenu(std::string pathToTheme, sf::RenderWindow& w) : GUI(pathToTheme, w) {}
		MainMenu(const MainMenu&) = default;
		int update() override;
		void makeMenu(std::string&);
	};
}
#endif