#include "Level.h"
#include <algorithm>
#include <windows.h>
#include "LogConsole.h"
using namespace Engine;

int ObjectLevel::GetPropertyInt(std::string name)
{
	return atoi(properties[name].c_str());
}

float ObjectLevel::GetPropertyFloat(std::string name)
{
	return strtod(properties[name].c_str(), nullptr);
}

std::string ObjectLevel::GetPropertyString(std::string name)
{
	return properties[name];
}

bool Level::LoadFromFile(std::string filename, int ScaleMap)
{
	MapScale = ScaleMap;
	TiXmlDocument levelFile(filename.c_str());

	if (!levelFile.LoadFile())
	{
		std::string error = "Loading level \"" + filename + "\" failed.";
		Console::AppLog::addLog(error, Console::error);
		return false;
	}

	TiXmlElement *map;
	map = levelFile.FirstChildElement("map");

	width = atoi(map->Attribute("width"));
	height = atoi(map->Attribute("height"));
	tileWidth = atoi(map->Attribute("tilewidth"));
	tileHeight = atoi(map->Attribute("tileheight"));
	orientation = map->Attribute("orientation");

	TiXmlElement *el;
	el = map->FirstChildElement("tileset");
	
	while (el)
	{
		tilesets.push_back(new Tileset(el));
		el = el->NextSiblingElement("tileset");
	}

	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement("layer");

	if (orientation == "orthogonal")
		ParseLayer(layerElement);
	else if (orientation == "isometric")
		ParseIsoLayer(layerElement);

	TiXmlElement *objectGroupElement;

	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
		if (orientation == "orthogonal")
			ParseObject(objectGroupElement);
		else if (orientation == "isometric")
			ParseIsoObject(objectGroupElement);
	}
	else
	{
		Console::AppLog::addLog("No object layers found...", Console::error);
	}
	std::string info = "Level load at path(" + filename + ")";
	Console::AppLog::addLog(info, Console::info);
	return true;
}

ObjectLevel Level::GetObjectByName(std::string name)
{
	for (int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			return objects[i];
}

std::vector<ObjectLevel> Level::GetObjects(std::string name)
{
	std::vector<ObjectLevel> vec;
	for (int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			vec.push_back(objects[i]);
	return vec;
}

const std::vector<ObjectLevel>& Level::GetAllObjects()
{
	return objects;
};

sf::Vector2i Level::GetTileSize()
{
	return sf::Vector2i(tileWidth, tileHeight);
}

sf::IntRect Engine::Level::GetRect() const
{
	return rect;
}

void Level::ParseObject(TiXmlElement *layerElement)
{
	while (layerElement)
	{
		TiXmlElement *objectElement;
		objectElement = layerElement->FirstChildElement("object");

		while (objectElement)
		{
			std::string objectType;
			if (objectElement->Attribute("type") != NULL)
			{
				objectType = objectElement->Attribute("type");
			}
			std::string objectName;
			if (objectElement->Attribute("name") != NULL)
			{
				objectName = objectElement->Attribute("name");
			}
			int x = atoi(objectElement->Attribute("x"));
			int y = atoi(objectElement->Attribute("y"));

			int width, height;

			if (objectElement->Attribute("width") != NULL)
			{
				width = atoi(objectElement->Attribute("width"));
				height = atoi(objectElement->Attribute("height"));
			}

			ObjectLevel object;
			object.name =objectName;
			object.type = objectType;

			sf::Rect<float> objectRect;
			objectRect.top = y * MapScale;
			objectRect.left = x * MapScale;
			objectRect.height = height * MapScale;
			objectRect.width = width * MapScale;
			object.rect = objectRect;

			TiXmlElement *properties;
			properties = objectElement->FirstChildElement("properties");
			if (properties != NULL)
			{
				TiXmlElement *prop;
				prop = properties->FirstChildElement("property");
				if (prop != NULL)
				{
					while (prop)
					{
						std::string propertyName = prop->Attribute("name");
						std::string propertyValue = prop->Attribute("value");

						object.properties[propertyName] = propertyValue;

						prop = prop->NextSiblingElement("property");
					}
				}
			}

			objects.push_back(object);

			objectElement = objectElement->NextSiblingElement("object");
		}
		layerElement = layerElement->NextSiblingElement("objectgroup");
	}
}

void Engine::Level::ParseIsoObject(TiXmlElement *layerElement)
{
	while (layerElement)
	{

		TiXmlElement *objectElement;
		objectElement = layerElement->FirstChildElement("object");

		while (objectElement)
		{
			std::string objectType;
			if (objectElement->Attribute("type") != NULL)
			{
				objectType = objectElement->Attribute("type");
			}
			std::string objectName;
			if (objectElement->Attribute("name") != NULL)
			{
				objectName = objectElement->Attribute("name");
			}
			int x = atoi(objectElement->Attribute("x"));
			int y = atoi(objectElement->Attribute("y"));

			int width, height;

			if (objectElement->Attribute("width") != NULL)
			{
				width = atoi(objectElement->Attribute("width"));
				height = atoi(objectElement->Attribute("height"));
			}

			ObjectLevel object;
			object.name = std::move(objectName);
			object.type = std::move(objectType);

			sf::Rect<float> objectRect;

			/*
			sfloat isoX, isoY;
			isoX = x - y;
			isoY = (x + y) / 2;
			std::cout<<"x = " << x<<"  y = " << y <<std::endl;
			std::cout<<"isoX = " << isoX<<"  isoY = " << isoY <<std::endl;
			std::cout<<"tileWidth = " << width<<"  tileHeight = " << height <<std::endl;
			*/
			//	isoX = (x - y) * tileWidth / 2; isoY = (x + y) * tileHeight / 2;
			// auto newX = centerX + (x - centerX) * cos(lastradian) - (y - centerY) * sin(lastradian); //x,y

			float isoX, isoY;
			isoX = (x - y);
			isoY = (x + y) / 2;
			float isoW, isoH;
			float x2 = (x + width);
			float y2 = (y + height);
			isoW = (x2 - y2);
			isoH = (x2 + y2) / 2;

			objectRect.left = isoX - width / 2 * MapScale;
			objectRect.top = isoY + height / 2 * MapScale;
			objectRect.height = isoH - height / 2 * MapScale;
			objectRect.width = isoW + width / 2 * MapScale;
			object.rect = objectRect;

			object.quad.leftRect.left = isoX;
			object.quad.leftRect.width = isoW;
			object.quad.leftRect.top = isoY;
			object.quad.leftRect.height = isoH;

			object.quad.rightRect.left = isoX - width / 2;
			object.quad.rightRect.width = isoW + width / 2;
			object.quad.rightRect.top = isoY + height / 2;
			object.quad.rightRect.height = isoH - height / 2;

			std::cout << "x = " << x << "  y = " << y << std::endl;
			std::cout << "isoX = " << isoX << "  isoY = " << isoY << std::endl;
			std::cout << "isoXW = " << isoW << "  isoXW = " << isoH << std::endl;

			TiXmlElement *properties;
			properties = objectElement->FirstChildElement("properties");
			if (properties != NULL)
			{
				TiXmlElement *prop;
				prop = properties->FirstChildElement("property");
				if (prop != NULL)
				{
					while (prop)
					{
						std::string propertyName = prop->Attribute("name");
						std::string propertyValue = prop->Attribute("value");
						object.properties[propertyName] = propertyValue;
						prop = prop->NextSiblingElement("property");
					}
				}
			}

			objects.push_back(object);

			objectElement = objectElement->NextSiblingElement("object");
		}
		layerElement = layerElement->NextSiblingElement("objectgroup");
	}
}

void Engine::Level::ParseIsoLayer(TiXmlElement *layerElement)
{
	auto iter = tilesets.begin();
	while (layerElement)
	{
		MapLayer layer;
		if (layerElement->Attribute("opacity") != nullptr)
		{
			float opacity = strtod(layerElement->Attribute("opacity"), nullptr);
			layer.opacity = 255 * opacity;
		}
		else
			layer.opacity = 255;

		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == nullptr)
			Console::AppLog::addLog("Bad map. No layer information found.", Console::error);

		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == nullptr)
			Console::AppLog::addLog("Bad map. No tile information found.", Console::error);

		int x = 0;
		int y = 0;
		rect.left = 0;
		rect.top = 0;
		rect.width = width * tileWidth;
		rect.height = height * tileHeight;

		auto subRects = (*iter)->getSubRect();
		int MaxID = (*iter)->getMaxID();
		int firstID = (*iter)->firstTileID;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			if (tileGID != 0)
			{
				int subRectToUse = tileGID - firstID;
				if (subRectToUse >= 0)
				{
					sf::Sprite sprite;
					float isoX, isoY;
					sprite.setTexture((*iter)->getTexture());
					sprite.setTextureRect(subRects[subRectToUse]);
					isoX = (x - y) * tileWidth / 2;
					isoY = (x + y) * tileHeight / 2;
					sprite.setPosition(isoX - 32, isoY - 32);
					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
					sprite.setScale(MapScale, MapScale);
					layer.tiles.push_back(sprite);
				}
			}
			tileElement = tileElement->NextSiblingElement("tile");
			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.push_back(layer);
		layerElement = layerElement->NextSiblingElement("layer");
		if (iter != tilesets.end() - 1)
			++iter;
	}
}

void Engine::Level::ParseLayer(TiXmlElement *layerElement)
{
	auto iter = tilesets.begin();
	while (layerElement)
	{
		MapLayer layer;
		if (layerElement->Attribute("opacity") != NULL)
		{
			float opacity = strtod(layerElement->Attribute("opacity"), NULL);
			layer.opacity = 255 * opacity;
		}
		else
		{
			layer.opacity = 255;
		}

		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL)
		{
			Console::AppLog::addLog("Bad map. No layer information found.", Console::error);
		}

		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL)
		{
			Console::AppLog::addLog("Bad map. No tile information found.", Console::error);
		}

		int x = 0;
		int y = 0;
		rect.left = 0;
		rect.top = 0;
		rect.width = width * tileWidth;
		rect.height = height * tileHeight;

		auto subRects = (*iter)->getSubRect();
		int MaxID = (*iter)->getMaxID();
		int firstID = (*iter)->firstTileID;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			if (tileGID != 0)
			{
				int subRectToUse = tileGID - firstID;
				if (subRectToUse >= 0)
				{
					sf::Sprite sprite;
					sprite.setTexture((*iter)->getTexture());
					sprite.setTextureRect(subRects[subRectToUse]);
					sprite.setPosition(x * tileWidth * MapScale, y * tileHeight * MapScale);
					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
					sprite.setScale(MapScale, MapScale);
					layer.tiles.push_back(sprite);
				}
			}
			tileElement = tileElement->NextSiblingElement("tile");
			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.push_back(layer);
		layerElement = layerElement->NextSiblingElement("layer");
		if (iter != tilesets.end() - 1)
			++iter;
	}
}

void Level::DrawLevel(sf::RenderWindow &window)
{
	for (auto &layer : layers)
		for (const auto &tile : layer.tiles)
		{
			window.draw(tile);
			Sleep(10);
			window.display();
		}
}

sf::Image Level::DrawLevel2()
{
	sf::RenderTexture t;
	t.create(1920, 1080);
	t.clear();
	for (auto &layer : layers)
		for (const auto &tile : layer.tiles)
		{
			t.draw(tile);
		}
	t.display();
	return sf::Image(t.getTexture().copyToImage());
}

sf::Image Engine::Tileset::getImage()
{
	return tilesetImage;
}

std::vector<sf::Rect<int>> Engine::Tileset::getSubRect()
{
	std::vector<sf::Rect<int>> subRects;
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
	return subRects;
}
