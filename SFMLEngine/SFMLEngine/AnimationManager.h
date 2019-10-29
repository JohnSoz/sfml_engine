#pragma once
/*!
\file
\brief Header file.
Contains the necessary classes to work with animations
*/

#include <SFML\Graphics.hpp>
#include <list>
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <math.h>
#include "TinyXML/tinyxml.h"
#include <MetaStuff/Meta.h>
using json = nlohmann::json;

namespace Engine
{
	/*!
	\brief Class for storing animation information
	*/
	enum AnimationState { APlay, APause, AEnd };
	class Animation
	{
	public:
		std::string    name;
		sf::IntRect    rect;
		sf::Vector2f   origin;
		AnimationState state;
		std::vector<sf::IntRect> frames;
		float frame;
		float frameCount;
		float speed;
		bool  looped;
		float scale;

		Animation()
		{
			frame = 0;
			frameCount = 0;
			origin = { 0,0 };
			rect = { 0,0,0,0 };
			scale = 0.f;
			speed = 0.01f;
		}
		virtual sf::IntRect& tick(float time) = 0;
	};

	class AnimationXml final : public Animation
	{
	public:

		AnimationXml() { looped = false; state = APause; }
		sf::IntRect& tick(const float time) override;
	};
	class AnimationJson : public Animation
	{
	public:
		sf::IntRect rect;
		sf::Vector2f Center;
		AnimationJson() { frame = 0; scale = 0;  speed = 0; }

		sf::IntRect& tick(float time) override;
	};

	class AnimationManager
	{
	private:
		float AnimationFrame, time, Angle;

		int step; ///< Not use
		std::list<Animation*> animationList;
		std::list<Animation*>::iterator currAnim;
	public:
		sf::Texture  texture;
		sf::IntRect  rect;
		float scale;
		std::string path;

		AnimationManager() { AnimationFrame = time = Angle = 0; };
		~AnimationManager();

		void loadAnimation_j(std::string_view PATH);

		template<class T>
		T* getCurrAnimation() { return dynamic_cast<T*>(*currAnim); }

		AnimationXml* getCurrAnimation() { return static_cast<AnimationXml*>(*currAnim); }

		sf::IntRect& animUpdate(float TIME);

		void setCurrAnimation(std::list <Animation*>::iterator iter);

		void loadAnimation_x(std::string_view fileName, sf::RenderWindow&);

		std::list <Animation*>::iterator getAnimationByName(std::string_view NAME);

		std::list<Animation*>* const getAnimationList();

		//friend auto meta::registerMembers<Engine::AnimationManager>();
	};
}
namespace meta
{
	//template <>
	//inline auto registerMembers<Engine::AnimationManager>()
	//{
	//	return members(
	//		member("Path", &Engine::AnimationManager::path)
	//	);
	//}
}