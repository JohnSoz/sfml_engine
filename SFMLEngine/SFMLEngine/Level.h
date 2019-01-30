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

namespace Engine
{

struct ObjectLevel
{
	int GetPropertyInt(std::string name);
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);

	std::string name;
	std::string type;
	sf::Rect<float> rect;
	Quad quad;
	std::map<std::string, std::string> properties;
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
	Tileset(TiXmlElement *element)
	{
		firstTileID = atoi(element->Attribute("firstgid"));
		TileCount = atoi(element->Attribute("tilecount"));
		TiXmlElement *image;
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

	sf::Texture &getTexture()
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
	void ParseLayer(TiXmlElement *layerElement);
	void ParseIsoLayer(TiXmlElement *layerElement);
	void ParseObject(TiXmlElement *layerElement);
	void ParseIsoObject(TiXmlElement *layerElement);


	sf::IntRect rect;
	int width, height, tileWidth, tileHeight;
	sf::Rect<float> drawingBounds;
	sf::Texture tilesetImage;
	std::vector<ObjectLevel> objects;
	std::vector<MapLayer> layers;
	std::string orientation;

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

} // namespace Engine
#endif