#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "TinyXML/tinyxml.h"
#include <Windows.h>
#include <stdlib.h>

namespace Engine {

	struct ObjectLevel
	{
		int GetPropertyInt(std::string name);  //номер свойства объекта в нашем списке
		float GetPropertyFloat(std::string name);
		std::string GetPropertyString(std::string name);

		std::string name;  //объявили переменную name типа string
		std::string type;  //а здесь переменную type типа string
		sf::Rect<float> rect;  //тип Rect с нецелыми значениями
		std::map<std::string, std::string> properties;  //создаём ассоциатиный массив. ключ - строковый тип, значение - строковый

		sf::Sprite sprite;  //объявили спрайт
	};

	struct Tileset
	{
	private:
		sf::Sprite sprite;
		sf::Texture tilesetImage;
		int TileCount;
		int columns;
		int rows;
		int tileWidth;
		int tileHeight;
		std::vector<sf::Rect<int>> subRects;
		void CalculateSubRects()
		{
			for (int y = 0; y < rows; y++)
				for (int x = 0; x < columns; x++)
				{
					sf::Rect<int> rect;

					rect.top = y * tileHeight;
					rect.height = tileHeight;
					rect.left = x * tileWidth;
					rect.width = tileWidth;

					subRects.push_back(rect);
				}
		}
	public:
		int firstTileID;
		Tileset(TiXmlElement* element)
		{
			firstTileID = atoi(element->Attribute("firstgid"));
			TileCount = atoi(element->Attribute("tilecount"));
			TiXmlElement *image;
			image = element->FirstChildElement("image");
			std::string imagepath = image->Attribute("source");
			sf::Image img;
			img.loadFromFile(imagepath);
			img.createMaskFromColor(sf::Color(255, 255, 255));
			tilesetImage.loadFromImage(img);
			tilesetImage.setSmooth(true);//сглаживание
			tileWidth = atoi(element->Attribute("tilewidth"));
			tileHeight = atoi(element->Attribute("tileheight"));
			columns = tilesetImage.getSize().x / tileWidth;
			rows = tilesetImage.getSize().y / tileHeight;
			CalculateSubRects();
		}
		int GetMaxID() { return firstTileID + TileCount; }
		std::vector<sf::Rect<int>> GetSubRect() { return subRects; }

	};

	struct Layer  //слои
	{
		float opacity;  //непрозрачность слоя
		std::vector<sf::Sprite> tiles;  //закидываем в вектор тайлы
	};

	class Level  //главный класс - уровень
	{
	public:
		bool LoadFromFile(std::string filename, int MapScale = 1);//возвращает false если не получилось загрузить
		ObjectLevel GetObject(std::string name);
		std::vector<Tileset> tilesets;
		std::vector<ObjectLevel> GetObjects(std::string name);//выдаем объект в наш уровень
		std::vector<ObjectLevel> GetAllObjects();//выдаем все объекты в наш уровень
		void DrawLevel(sf::RenderWindow &window);//рисуем в окно
		sf::Vector2i GetTileSize();//получаем размер тайла
		sf::IntRect GetRect() const; //Прямоугольник карты
	private:
		int MapScale;
		int posX = 0;
		int posY = 0;
		int Xc = 0;
		void ParseLayer(TiXmlElement *layerElement);
		sf::IntRect rect; //Прямоугольник карты
		int width, height, tileWidth, tileHeight;//в tmx файле width height в начале,затем размер тайла
		int firstTileID;//получаем айди первого тайла
		sf::Rect<float> drawingBounds;//размер части карты которую рисуем
		sf::Texture tilesetImage;//текстура карты
		std::vector<ObjectLevel> objects;//массив типа Объекты, который мы создали
		std::vector<Layer> layers;
	};

}
#endif