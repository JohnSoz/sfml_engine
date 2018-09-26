#include "AnimationManager.h"

using namespace Engine;
using std::string;
using sf::IntRect;
using sf::Vector2f;
using sf::Texture;
using std::vector;

void AnimationManager::LoadAnimation_j(string path)
{
	AnimationJson* anim = new AnimationJson;
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

	anim->name = j.at("Name").get<string>();
	anim->rect = rect;
	anim->frame = j.at("Frame").get<int>();
	anim->speed = j.at("Speed").get<float>();
	anim->scale.x = j.at("Scale").get<float>();
	anim->scale.y = j.at("Scale").get<float>();
	anim->Center = center;
	if (animationList.empty())
	{
		animationList.push_back(anim);
		CurrentAnimation = *animationList.begin();
	}
	else
		animationList.push_back(anim);
}

IntRect& AnimationManager::AnimUpdate(float t)
{
	return CurrentAnimation->tick(t);
}

void AnimationManager::LoadAnimation_x(std::string fileName)
{
	TiXmlDocument animFile(fileName.c_str());

	animFile.LoadFile();

	TiXmlElement *head;
	head = animFile.FirstChildElement("sprites");
	string img = head->Attribute("image");

	TiXmlElement *animElement;
	animElement = head->FirstChildElement("animation");

	string Name = animElement->Attribute("title");
	float delay = atof(animElement->Attribute("delay"));

	vector<IntRect> frames;
	bool isXMLAnimation = true;
	AnimationXml *anim = new AnimationXml;
	anim->name = Name;
	anim->speed = delay;
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

			frames.emplace_back(x, y, w, h);
			cut = cut->NextSiblingElement("cut");
		}
		animElement = animElement->NextSiblingElement("animation");
	}
	frames.shrink_to_fit();
	anim->frameCount = frames.size();
	anim->frames = std::move(frames);
	if (animationList.empty())
	{
		animationList.push_back(anim);
		CurrentAnimation = *animationList.begin();
	}
	else
		animationList.push_back(anim);
}

void AnimationManager::SetCurrAnimation(Animation && anim)
{
	//currAnim = std::forward<Animation>(anim);
	CurrentAnimation = &anim;
}
template<class T>
T AnimationManager::GetAnimationByName(string Name)
{
	for (auto & anim : animationList)
		if (anim.name == Name)
			return anim;
}

const std::list<Engine::Animation*>& Engine::AnimationManager::getAnimationList() 
{
	return animationList;
}

IntRect& AnimationJson::tick(float time)
{
	int widht = rect.width;
	int height = rect.height;
	int top = rect.top;
	int left = rect.left;
	frame += speed * time;
	if (frame > 2) frame = 0;
	if (int(frame) >= 1)
		int z = 5;
	IntRect x(widht * int(frame), top, widht, height);
	return x;
}
