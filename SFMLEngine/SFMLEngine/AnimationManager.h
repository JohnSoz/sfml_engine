#pragma once
/*!
\file
\brief Header file.
Contains the necessary classes to work with animations
*/

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
	/*!
	\brief Class for storing animation information
	\bug
	\warning
	\example
	\todo
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
		sf::Texture  texture;
		sf::IntRect  rect;
		sf::Vector2f origin;
		AnimationState state;

		Animation() { frameCount = 0; }
		virtual sf::IntRect& tick(float time) = 0;
	};

	class AnimationXml final : public Animation
	{
	public:
		std::vector<sf::IntRect> frames;			

		AnimationXml() { looped = false; state = APause; }

		sf::IntRect& tick(float time) override
		{
			state = APlay;
			frame += 0.009 * time;
			if (frame > frameCount) { frame = 0; state = AEnd; }
			return frames[frame];
		}
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
		std::list <Animation*> animationList; ///< Container with the animations
		std::list <Animation*>::iterator currAnim; ///< Iterator to traverse the container \warning also stores the current animation
	public:
		AnimationManager() = default; ///< Standard constructor
		~AnimationManager()///< Standard destructor
		{
			for (auto & iter : animationList)
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

		const std::list<Animation*>*     getAnimationList();
	};
}