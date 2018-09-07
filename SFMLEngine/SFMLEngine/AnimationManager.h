#pragma once
#include <SFML\Graphics.hpp>
#include <list>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <math.h>
#include <boost\align.hpp>
#include "TinyXML/tinyxml.h"

using json = nlohmann::json;

#define PI 3.141592653589793238462643383279502884L 

namespace Engine
{
	struct Animation
	{
	private:
		//?
	public:
		float frame;
		float speed;
		bool isXMLAnimation = false;
		//sf::Sprite sprite;
		//sf::Texture texture;
		sf::IntRect rect;
		std::vector<sf::IntRect> frames;
		sf::Vector2f Center;
		sf::Vector2f scale;
		std::string AnimationName;

		Animation()
		{
			frame = 0; scale.x = 0; scale.y = 0; speed = 0;
		}

		/////////////////////////
		///��������� ������� ���� ��������, angle - ���� �������� ������� 
		/////////////////////////
		sf::IntRect tick(float time);

		//sf::Sprite getSprite() { return sprite; }

		//void SetSpriteRotation(float Angle) { sprite.setRotation(Angle); }
	};

	class AnimationManager
	{
	private:
		float AnimationFrame, time, Angle;
		int step;
		std::list <std::pair<std::string, Animation>> animationList;
		std::list <std::pair<std::string, Animation>>::iterator currAnim;
		Animation CurrentAnimation;

	public:
		AnimationManager();

		/////////////////////////
		///��������� �������� �� json||Xml �����
		/////////////////////////
		Animation LoadAnimation(std::string PATH, bool LOADXML = false);

		/////////////////////////
		///���������� ������� ��������
		/////////////////////////
		Animation GetCurrAnimation() { return CurrentAnimation; }

		/////////////////////////
		///��������� ������� ���� ��������, �������� Animation::tick(float time,float angle)
		/////////////////////////
		sf::IntRect AnimUpdate(float TIME);

		/////////////////////////
		///������ ������� ��������
		/////////////////////////
		void SetCurrAnimation(Animation&& ANIM);

		/////////////////////////
		///��������� �������� �� xml �����
		/////////////////////////
		std::vector<sf::IntRect> loadFromXML(std::string fileName);

		/////////////////////////
		///
		/////////////////////////
		void SetSpriteSettings(sf::Sprite& sprite);

		/////////////////////////
		///���������� �������� �� �����
		/////////////////////////
		Animation GetAnimationByName(std::string NAME);

		~AnimationManager();
	};
}