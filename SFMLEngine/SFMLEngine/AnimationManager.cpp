#include "AnimationManager.h"

using namespace Engine;
using std::string;
using sf::IntRect;
using sf::Vector2f;
using sf::Texture;
using std::vector;

void AnimationManager::LoadAnimation_j(std::string_view path)
{
	AnimationJson* anim = new AnimationJson;
	json j;
	j.clear();
	std::ifstream i(path.data());
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
	anim->scale = j.at("Scale").get<float>();
	anim->Center = center;
	if (animationList.empty())
	{
		animationList.push_back(anim);
		currAnim = animationList.begin();
	}
	else
		animationList.push_back(anim);
}

IntRect& AnimationManager::AnimUpdate(float t)
{
	return (*currAnim)->tick(t);
}

void AnimationManager::LoadAnimation_x(std::string_view fileName)
{
	TiXmlDocument animFile(fileName.data());

	animFile.LoadFile();

	TiXmlElement* head;
	head = animFile.FirstChildElement("sprites");
	string img = head->Attribute("image");

	TiXmlElement* settings;
	settings = head->FirstChildElement("settings");

	TiXmlElement* animElement;
	animElement = head->FirstChildElement("animation");

	string Name = animElement->Attribute("title");
	float delay = atof(animElement->Attribute("delay"));

	vector<IntRect> frames;
	bool isXMLAnimation = true;
	AnimationXml* anim = new AnimationXml;

	auto xml_origin = settings->FirstChildElement("origin");
	anim->origin.x = atoi(xml_origin->Attribute("x"));
	anim->origin.y = atoi(xml_origin->Attribute("y"));

	auto xml_rect = settings->FirstChildElement("rect");
	anim->rect.left = atoi(xml_rect->Attribute("left"));
	anim->rect.top = atoi(xml_rect->Attribute("top"));
	anim->rect.width = atoi(xml_rect->Attribute("widht"));
	anim->rect.height = atoi(xml_rect->Attribute("height"));

	auto xml_loop = settings->FirstChildElement("loop");
	anim->looped = xml_loop->Attribute("value") == "1";

	auto xml_scale = settings->FirstChildElement("scale");
	anim->scale = atof(xml_scale->Attribute("value"));

	anim->name = Name;
	anim->speed = delay;
	anim->texture.loadFromFile("Data\\OSprite\\" + img);
	anim->texture.setSmooth(true);
	while (animElement)
	{
		TiXmlElement* cut;
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
		anim->frameCount++;
		animElement = animElement->NextSiblingElement("animation");
	}
	frames.shrink_to_fit();
	anim->frameCount = frames.size();
	anim->frames = std::move(frames);
	if (animationList.empty())
	{
		animationList.push_back(anim);
		currAnim = animationList.begin();
	}
	else
		animationList.push_back(anim);

	animFile.Clear();
}

void AnimationManager::SetCurrAnimation(std::list <Animation*>::iterator iter)
{
	currAnim = iter;
}

std::list <Animation*>::iterator AnimationManager::GetAnimationByName(std::string_view Name)
{
	return
		std::find_if(animationList.begin(), animationList.end(),
			[=](Animation * anim)
	{
		return (anim->name == Name);
	});
}

std::list<Engine::Animation*>* const Engine::AnimationManager::getAnimationList()
{
	return &animationList;
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
	return x; //err
}
