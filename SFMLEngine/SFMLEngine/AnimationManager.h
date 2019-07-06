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

#define PI 3.141592653589793238462643383279502884L 

namespace Engine
{
	/*!
	\brief Class for storing animation information
	*/
	enum AnimationState { APlay, APause, AEnd };
	class Animation
	{
	public:
		float frame; ///< Number of frames
		float frameCount;
		float speed; ///< The speed of the animation
		bool looped;

		std::string  name;
		float        scale;
		sf::IntRect  rect;
		sf::Vector2f origin;
		AnimationState state;

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
		std::vector<sf::IntRect> frames;

		AnimationXml() { looped = false; state = APause; }

		sf::IntRect& tick(const float time) override;
	};
	class AnimationJson : public Animation
	{
	public:
		sf::IntRect rect; ///< Frame(rectangle of the current frame)
		std::vector<sf::IntRect> frames; ///< Container frame \warning used if \code bool isXMLAnimation = true \endcode
		sf::Vector2f Center; ///< Center of the sprite object

		AnimationJson()
		{
			frame = 0; scale = 0;  speed = 0;
		}
		/*!
		Updates the current animation frame, angle-rotation angle of the sprite
		*/
		sf::IntRect& tick(float time) override;
	};

	/*!
	\brief Class for loading animation and its subsequent use
	\bug
	\warning
	\example
	\todo
   */
	class AnimationManager
	{
	private:
		float AnimationFrame, time, Angle;

		int step; ///< Not use
		std::list<Animation*> animationList; ///< Container with the animations
		std::list<Animation*>::iterator currAnim; ///< Iterator to traverse the container \warning also stores the current animation
	public:
		sf::Texture  texture;
		sf::IntRect  rect;
		float scale;
		std::string path;

		AnimationManager() { AnimationFrame = time = Angle = 0; };
		~AnimationManager()
		{
			for (auto& iter : animationList)
			{
				delete iter;
			}
			animationList.clear();
		}

		/*!
		Loads an animation from a json||Xml file
		 \return Animation Will return the animation class and put it to the same vector animation
		 \param PATH path to animation file (xml or json)
		 \param LOADXML Flag true if you are going to load xml animation
		*/
		void LoadAnimation_j(std::string_view PATH);

		/*!
		Returns the current animation
		 \return Current animation
		 \warning Can throw std::bad_cast
		*/
		template<class T>
		T* GetCurrAnimation() { return dynamic_cast<T*>(*currAnim); }

		AnimationXml* GetCurrAnimation() { return static_cast<AnimationXml*>(*currAnim); }

		/*!
		Updates the current animation
		 \details Will call tick(float t) of the current animation
		 \return returns a rectangle
		*/
		sf::IntRect& AnimUpdate(float TIME);

		/*!
		Sets the current animation
		 \warning this animation will not be put into pull animations(bag)
		*/
		void SetCurrAnimation(std::list <Animation*>::iterator iter);

		/*!
		Loads the animation from xml file
		 \details load animation from xml file and return vector from frames (rectangles) for animation
		*/
		void LoadAnimation_x(std::string_view fileName);

		/*!
		Returns the animation by name
		*/
		std::list <Animation*>::iterator GetAnimationByName(std::string_view NAME);

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