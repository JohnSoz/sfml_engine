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

		std::string name;  //объ€вили переменную name типа string
		std::string type;  //а здесь переменную type типа string
		sf::Rect<float> rect;  //тип Rect с нецелыми значени€ми
		std::map<std::string, std::string> properties;  //создаЄм ассоциатиный массив. ключ - строковый тип, значение - строковый

		sf::Sprite sprite;  //объ€вили спрайт
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
			tilesetImage.setSmooth(true); //сглаживание
			tileWidth = atoi(element->Attribute("tilewidth"));
			tileHeight = atoi(element->Attribute("tileheight"));
			columns = tilesetImage.getSize().x / tileWidth;
			rows = tilesetImage.getSize().y / tileHeight;
			CalculateSubRects();
		}
		~Tileset() = default;
		int GetMaxID() { return firstTileID + TileCount; }
		std::vector<sf::Rect<int>> GetSubRect() { return subRects; }

	};

	struct MapLayer 
	{
		float opacity; 
		std::vector<sf::Sprite> tiles;  
	};

	class Level  
	{
	private:
		int MapScale;
		int posX = 0;
		int posY = 0;
		int Xc = 0;
		void ParseLayer(TiXmlElement *layerElement);
		sf::IntRect rect; 
		int width, height, tileWidth, tileHeight;
		int firstTileID;
		sf::Rect<float> drawingBounds;
		sf::Texture tilesetImage;
		std::vector<ObjectLevel> objects;
		std::vector<MapLayer>    layers;
	public:
		bool LoadFromFile(std::string filename, int MapScale = 1);
		ObjectLevel GetObjectByName(std::string name);
		std::vector<Tileset> tilesets;
		std::vector<ObjectLevel> GetObjects(std::string name);
		std::vector<ObjectLevel> GetAllObjects();
		void DrawLevel(sf::RenderWindow &window);
		sf::Image DrawLevel2();
		sf::Vector2i GetTileSize();
		sf::IntRect GetRect() const;
	};

}
#endif