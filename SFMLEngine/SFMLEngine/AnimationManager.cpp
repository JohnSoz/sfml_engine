#include "AnimationManager.h"

using namespace Engine;
using std::string;
using sf::IntRect;
using sf::Vector2f;
using sf::Texture;
using std::vector;

AnimationManager::AnimationManager()
{
}

Animation AnimationManager::LoadAnimation(string path, bool LoadXML)
{
	Animation anim;
	if (!LoadXML)
	{
		json j;
		j.clear();
		std::ifstream i(path);
		i >> j;
		i.clear();
		i.close();
		IntRect rect;
		Vector2f center;

		if (j.at("IntRect").is_array())
			rect = IntRect(j.at("IntRect")[0].get<int>(), j.at("IntRect")[1].get<int>(), j.at("IntRect")[2].get<int>(), j.at("IntRect")[3].get<int>());
		if (j.at("Center").is_array())
			center = Vector2f(j.at("Center")[0].get<int>(), j.at("Center")[1].get<int>());

		anim.isXMLAnimation = false;
		anim.AnimationName = j.at("Name").get<string>();
		anim.rect = rect;

		/*anim.texture.loadFromFile(j["Path"].get<string>());
		anim.sprite.setTexture(anim.texture);
		anim.sprite.setTextureRect(rect);
		anim.sprite.setScale(Vector2f(j.at("Scale").get<float>(), j.at("Scale").get<float>()));*/
		anim.frame = j.at("Frame").get<int>();
		anim.speed = j.at("Speed").get<float>();
		anim.scale.x = j.at("Scale").get<float>();
		anim.scale.y = j.at("Scale").get<float>();
		anim.Center = center;
	}
	else
	{
		anim.isXMLAnimation = true;
		anim.frames = loadFromXML(path);
	}
	//anim.isXMLAnimation = true;
	//anim.frames = loadFromXML(j.at("PathToXML").get<string>());
//	Animation anim2 = CreateAnimation("asd", sf::IntRect(120, 120, 120, 120), "/", 12, 12, sf::Vector2f(12, 12));
	if (animationList.empty()) 
	{
		animationList.push_back(std::pair(anim.AnimationName, anim));
		CurrentAnimation = animationList.begin()->second;
	}
	else
		animationList.push_back(std::pair(anim.AnimationName, anim));
	return anim;
}

IntRect AnimationManager::AnimUpdate(float t) 
{
	return CurrentAnimation.tick(t);
}

vector<IntRect> AnimationManager::loadFromXML(std::string fileName)
{
	TiXmlDocument animFile(fileName.c_str());

	animFile.LoadFile();

	TiXmlElement *head;
	head = animFile.FirstChildElement("sprites");
	string img = head->Attribute("image");
	/*Texture texture;

	texture.loadFromFile("Data/images/" + img);*/

	TiXmlElement *animElement;
	animElement = head->FirstChildElement("animation");

	string name = animElement->Attribute("title");
	int delay = atoi(animElement->Attribute("delay"));

	vector<IntRect> frames;
	bool isXMLAnimation = true;

	while (animElement)
	{

		TiXmlElement *cut;
		cut = animElement->FirstChildElement("cut");
		while (cut)
		{
			int x = atoi(cut->Attribute("x"));
			int y = atoi(cut->Attribute("y"));
			int w = atoi(cut->Attribute("w"));
			int h = atoi(cut->Attribute("h"));

			frames.push_back(IntRect(x, y, w, h));
			cut = cut->NextSiblingElement("cut");
		}
		animElement = animElement->NextSiblingElement("animation");
	}
	return frames;
}

void AnimationManager::SetSpriteSettings(sf::Sprite & sprite)
{
	sprite.setScale(CurrentAnimation.scale);
	sprite.setTextureRect(CurrentAnimation.rect);
	sprite.setOrigin(CurrentAnimation.Center);
	//sprite.setTexture(CurrentAnimation.texture);
}

void AnimationManager::SetCurrAnimation(Animation && anim)
{
	currAnim->second = anim;
	CurrentAnimation = anim;
}

Animation AnimationManager::GetAnimationByName(string Name)
{
	for (auto & anim : animationList)
		if (anim.first == Name)
			return anim.second;
}


AnimationManager::~AnimationManager()
{
}


IntRect Animation::tick(float time)
{
	if (!isXMLAnimation)
	{
		int widht = rect.width;
		int height = rect.height;
		int top = rect.top;
		int left = rect.left;
		frame += speed * time;
		if (frame > 2) frame = 0;
		if (int(frame) >= 1)
			int z = 5;
		return IntRect(widht * int(frame), top, widht, height);
	}
	else
	{
		frame += 0.009 * time;
		std::cout<<int(frame)<<std::endl;
		if (frame > 19) frame = 0;
		return frames[frame];
	}
}
