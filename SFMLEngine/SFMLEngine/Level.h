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
#include "Math.h"
#include "json.hpp"
using json = nlohmann::json;

namespace Engine
{
	enum _mapType
	{
		Json = 1 << 0,
		Xml = 1 << 1,
		orthogonal = 1 << 2,
		isometric = 1 << 3
	};
	struct ObjectLevel
	{
		int GetPropertyInt(std::string name);
		float GetPropertyFloat(std::string name);
		std::string GetPropertyString(std::string name);

		std::string name;
		std::string type;
		sf::FloatRect rect;
		Quad quad;
		std::map<std::string, std::string> properties;
		operator sf::FloatRect() const
		{
			return rect;
		}
	};

	struct Tileset
	{
	private:
		sf::Sprite sprite;
		sf::Image tilesetImage;
		sf::Texture texture;
		int TileCount;
		int columns;
		int rows;
		int tileWidth;
		int tileHeight;

	public:
		int firstTileID;
		Tileset(TiXmlElement* element)
		{
			firstTileID = atoi(element->Attribute("firstgid"));
			TileCount = atoi(element->Attribute("tilecount"));
			TiXmlElement* image;
			image = element->FirstChildElement("image");
			std::string imagepath = image->Attribute("source");
			tilesetImage.loadFromFile(imagepath);
			texture.loadFromImage(tilesetImage);
			texture.setSmooth(true);
			tileWidth = atoi(element->Attribute("tilewidth"));
			tileHeight = atoi(element->Attribute("tileheight"));
			columns = tilesetImage.getSize().x / tileWidth;
			rows = tilesetImage.getSize().y / tileHeight;
		}
		~Tileset() = default;

		sf::Texture& getTexture()
		{
			return texture;
		}
		sf::Image getImage();
		int getMaxID() { return firstTileID + TileCount; }
		int getFitrsTileId() { return firstTileID; }
		std::vector<sf::Rect<int>> getSubRect();
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
		void ParseLayer(TiXmlElement* layerElement);
		void ParseIsoLayer(TiXmlElement* layerElement);
		void ParseObject(TiXmlElement* layerElement);
		void ParseIsoObject(TiXmlElement* layerElement);


		sf::IntRect rect;
		int width, height, tileWidth, tileHeight;
		sf::Rect<float> drawingBounds;
		sf::Texture tilesetImage;
		std::vector<ObjectLevel> objects;
		std::vector<MapLayer> layers;
		std::string orientation;

	public:
		std::vector<Tileset*> tilesets;
		bool LoadFromFile(std::string filename, int MapScale = 1);
		ObjectLevel GetObjectByName(std::string name);
		std::vector<ObjectLevel> GetObjects(std::string name);
		const std::vector<ObjectLevel>& GetAllObjects();
		void DrawLevel(sf::RenderWindow& window);
		sf::Image DrawLevel2();
		sf::Vector2i GetTileSize();
		sf::IntRect GetRect() const;
	};

	inline float raycastLevelObject(sf::Vector2f start, sf::Vector2f end, const std::vector<ObjectLevel>& objects, std::string name_object = "")
	{
		for (auto& obj : objects)
		{
			bool check = (!name_object.empty()) ? name_object == obj.name : true;
			if (check && obj.rect.contains(start.x + end.x, start.y + end.y))
				return Distance(start, sf::Vector2f(start.x, obj.rect.top));
		}
		return 0;
	}

} // namespace Engine
#endif