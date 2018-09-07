#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Level.h"
#include <vector>
#include <array>
#include "imgui.h"
#include "imgui-sfml.h"
#include "AnimationManager.h"

namespace Engine
{
	enum ObjectType
	{
		OEntity = 1,
		OPawn,
		OActor
	};

	class World;

	class Object
	{
	protected:

	public:
		Object() = delete;
	};

	class Entity
	{
	protected:
		ObjectType  type;
		Vector2D    position;

		sf::IntRect      localRectangle;
		sf::FloatRect    globalRectangle;
		sf::FloatRect    debugRectangle;

		sf::Sprite  sprite;
		sf::Texture texture;
		std::string name;

		int         id = 0;
		bool        active = true;
		bool        life = true;

	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(Entity&&) = default;
		Entity(Vector2D POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, Vector2D POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::IntRect r, Vector2D pos, std::string name);
		virtual ~Entity();


		virtual void update(float time) = 0;
		void SetPos(int x, int y) { position.x = x; position.y = y; }
		void destroy() { active = false; }

		bool isActive() const { return active; }

		sf::IntRect getRect() { return localRectangle; }
		Vector2D &getPos() { return position; }
		ObjectType &getType() { return type; }

		std::pair<sf::FloatRect*, sf::FloatRect*> getDebugRect() { return std::make_pair(&globalRectangle, &debugRectangle); }

		friend class ObjectHandler;
	};

	enum Direction
	{
		Up = 1, Down, Left, Right, State
	};

	class Actor;

	struct IMGUI
	{
		static void ShowHelpMarker(const char* desc);

		//tuple можно заменить на std::vector<std::pair<string,value>> -- ?
		template <typename ... T>
		static void objectStatusInfo(bool *open, std::string name, std::tuple<T...> tup);
	};


	class Actor : public Entity, public IMGUI
	{
	protected:
		std::vector<ObjectLevel> obj;
		Vector2D velocity;
		AnimationManager animManager;

		bool showDebugConsole = false;
		bool isWalk = false;
		bool isCollision = false;
		float lives = 100;
		float armor = 100;
		float speed = 0;
		float energy = 0.005;
		float friction = 0.005;
		float maxSpeed = 0.3;
		float CurrAngle = sprite.getRotation(), Radian = 0, LastAngle = 0;

		sf::Clock Pressclock;
		sf::Vector2f originOffset = {};
		Direction direction = Direction::State;
		sf::RenderWindow* window;

	public:
		Actor() = delete;
		Actor(sf::Image& IMAGE, Vector2D POSITION, sf::IntRect rect, std::string NAME, sf::RenderWindow& w, Level& lvl) : Entity(IMAGE, POSITION, NAME)
		{
			animManager.LoadAnimation("Move.xml", 1);
			//animManager.SetCurrAnimation(animManager.GetAnimationByName("Move"));
			localRectangle = rect;
			globalRectangle = sf::FloatRect(position.x, position.y, position.x + rect.width, position.y + rect.top);
			obj = lvl.GetAllObjects();
			originOffset = { 90,120 };
			sprite.setOrigin(originOffset);
			sprite.setTextureRect(localRectangle);
			window = &w;
		}

		void handleEvent(sf::Event& e);
		void checkClashes(Vector2D pos);
		void RotateToMouse(float speed, sf::RenderWindow& window);
		void update(float time) override;
		void getDamage(float dmg);
	};



	template<typename ...T>
	static inline void IMGUI::objectStatusInfo(bool * open, std::string name, std::tuple<T...> tup)
	{
		if (!ImGui::Begin(name.c_str(), open))
		{
			ImGui::End();
			return;
		}
		ImGui::SetWindowSize(ImVec2(400, 400));
		auto[energy, velocityX, velocityY, maxSpeed, CurrAngle] = tup;

		if (ImGui::CollapsingHeader("Game info"))
		{
			if (ImGui::TreeNode("Object info"))
			{
				ImGui::Text("Entity:");
				ImGui::SameLine();
				ShowHelpMarker("HELP MARK");
				ImGui::Separator();
				ImGui::Text("energy %.3f", *energy);
				if (ImGui::BeginPopupContextItem("itemEnergy"))
				{
					if (ImGui::Selectable("Set to zero")) *energy = 0.0f;
					if (ImGui::Selectable("Set to default")) *energy = 0.005;
					ImGui::PushItemWidth(200);
					ImGui::DragFloat("#energy", energy, 0.001f, 0.001f, 0.09f);
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
				ImGui::Spacing();
				ImGui::Text("maxSpeed %.2f", *maxSpeed);
				if (ImGui::BeginPopupContextItem("itemMaxSpeed"))
				{
					if (ImGui::Selectable("Set to zero")) *maxSpeed = 0.0f;
					if (ImGui::Selectable("Set to default")) *maxSpeed = 0.3;
					ImGui::PushItemWidth(200);
					ImGui::DragFloat("#maxSpeed", maxSpeed, 0.05f, 0.05f, 1.5f);
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
				ImGui::Spacing();
				ImGui::Text("CurrAngle %.2f", *CurrAngle);
				ShowHelpMarker("immutable value");
				ImGui::Spacing();
				ImGui::Text("velocity X = %.3f | Y = %.3f", *velocityX, *velocityY);
				ShowHelpMarker("immutable value");
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}

