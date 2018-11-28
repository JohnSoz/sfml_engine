#include "Level.h"
#include <algorithm>
#include <windows.h>

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
		std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
		return false;
	}


	TiXmlElement *map;
	map = levelFile.FirstChildElement("map");


	width = atoi(map->Attribute("width"));
	height = atoi(map->Attribute("height"));
	tileWidth = atoi(map->Attribute("tilewidth"));
	tileHeight = atoi(map->Attribute("tileheight"));

	TiXmlElement *el;
	el = map->FirstChildElement("tileset");
	while (el)
	{
		tilesets.push_back(std::move(Tileset(el)));
		el = el->NextSiblingElement("tileset");
	}

	TiXmlElement *tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	firstTileID = atoi(tilesetElement->Attribute("firstgid"));
	int TileCount = atoi(tilesetElement->Attribute("tilecount"));
	int Check = firstTileID + TileCount;

	TiXmlElement *image;
	image = tilesetElement->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");


	sf::Image img;

	if (!img.loadFromFile(imagepath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;
		return false;
	}


	img.createMaskFromColor(sf::Color(255, 255, 255));
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(true);


	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;


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

	// работа со слоями
	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement("layer");

	//ParseLayer(layerElement);

	while (layerElement)
	{
		MapLayer layer;
		if (layerElement->Attribute("opacity") != nullptr)
		{
			float opacity = strtod(layerElement->Attribute("opacity"), nullptr);
			layer.opacity = 255 * opacity;
		}
		else
		{
			layer.opacity = 255;
		}


		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == nullptr)
		{
			std::cout << "Bad map. No layer information found." << std::endl;
		}


		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == nullptr)
		{
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;
		rect.left = 0;
		rect.top = 0;
		rect.width = width * tileWidth;
		rect.height = height * tileHeight;
		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			// TextureRect каждого тайла
			if (subRectToUse >= 0)
			{
				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(x * tileWidth * MapScale, y * tileHeight * MapScale);
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
				sprite.setScale(MapScale, MapScale);
				layer.tiles.push_back(sprite);
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
		/*std::sort(layer.tiles.begin(), layer.tiles.end(),
			[](const sf::Sprite& spr1, const sf::Sprite& spr2)
		{
			return ((spr1.getPosition().x + spr1.getPosition().y) < (spr2.getPosition().y + spr2.getPosition().x));
		});*/

		layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// работа с объектами
	TiXmlElement *objectGroupElement;

	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{

			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

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

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(x, y);

				if (objectElement->Attribute("width") != NULL)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

				ObjectLevel object;
				object.name   = std::move(objectName);
				object.type   = std::move(objectType);
				object.sprite = std::move(sprite);

				sf::Rect <float> objectRect;
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
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found..." << std::endl;
	}
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


std::vector<ObjectLevel> Level::GetAllObjects()
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

void Engine::Level::ParseLayer(TiXmlElement * layerElement)
{
	for (auto i = 0; i < tilesets.size(); i++)
	{
		auto subRects = tilesets[i].GetSubRect();
		int MaxID = tilesets[i].GetMaxID();
		int firstID = tilesets[i].firstTileID;
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
				std::cout << "Bad map. No layer information found." << std::endl;
			}


			TiXmlElement *tileElement;
			tileElement = layerDataElement->FirstChildElement("tile");

			if (tileElement == NULL)
			{
				std::cout << "Bad map. No tile information found." << std::endl;
			}

			int x = 0;
			int y = 0;
			rect.left = 0;
			rect.top = 0;
			rect.width = width * tileWidth;
			rect.height = height * tileHeight;
			while (tileElement)
			{
				int tileGID = atoi(tileElement->Attribute("gid"));
				int subRectToUse = tileGID - firstTileID;
				if ((tileGID < MaxID) && (tileGID > firstID)) {
					// TextureRect каждого тайла
					if (subRectToUse >= 0)
					{
						sf::Sprite sprite;
						sprite.setTexture(tilesetImage);
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
		}
	}
}

void Level::DrawLevel(sf::RenderWindow &window)
{
	for (int layer = 0; layer < layers.size(); layer++)
		for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
		{
			window.draw(layers[layer].tiles[tile]);
			Sleep(10);
			window.display();
		}
}

sf::Image Level::DrawLevel2()
{
	sf::RenderTexture t;
	t.create(1920, 1080);
	t.clear();
	for (int layer = 0; layer < layers.size(); layer++)
		for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
		{
			t.draw(layers[layer].tiles[tile]);
		}
	t.display();
	return sf::Image(t.getTexture().copyToImage());
}
