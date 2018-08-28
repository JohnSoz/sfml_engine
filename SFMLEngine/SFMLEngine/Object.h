#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Level.h"
#include <vector>
#include <array>
#include "imgui.h"
#include "imgui-sfml.h"

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
		ObjectType type;
		Vector2D position;

		Rectangle localRectangle;
		Rectangle globalRectangle;
		Rectangle debugRectangle;
		sf::Sprite sprite;
		sf::Texture texture;
		std::string name;

		int id = 0;
		bool active = true;

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

		const Rectangle &getRect() { return localRectangle; }
		const Vector2D &getPos() { return position; }
		const ObjectType &getType() { return type; }

		std::pair<Rectangle*, Rectangle*> getDebugRect() { return std::make_pair(&globalRectangle, &debugRectangle); }

		friend class ObjectHandler;
	};

	enum Direction
	{
		Up = 1, Down, Left, Right, State
	};

	class Actor;

	struct IMGUI
	{
		static void ShowHelpMarker(const char* desc)
		{
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		template <typename ... T>
		static void objectStatusInfo(bool *open, std::string name, std::tuple<T...> tup) //..........
		{
			if (ImGui::Begin(name.c_str(), open))
			{
				ImGui::SetWindowSize(ImVec2(300, 300));

				auto[energy, velocityX, velocityY] = tup;

				if (ImGui::CollapsingHeader("Game info"))
				{
					if (ImGui::TreeNode("Object info"))
					{
						ImGui::Text("Entity:");
						ImGui::SameLine();
						ShowHelpMarker("Help Mark \nHelp Mark \nHelp Mark \n");
						ImGui::Separator();
						ImGui::Text("Value %.3f", *energy);
						if (ImGui::BeginPopupContextItem("item context menu"))
						{
							if (ImGui::Selectable("Set to zero")) *energy = 0.0f;
							if (ImGui::Selectable("Set to default")) *energy = 0.005;
							ImGui::PushItemWidth(200);
							ImGui::DragFloat("#Value", energy, 0.001f, 0.001f, 0.09f);
							ImGui::PopItemWidth();
							ImGui::EndPopup();
						}
						ImGui::Spacing();
						ImGui::Text("Value X = %.3f | Y = %.3f", *velocityX, *velocityY);
						if (ImGui::BeginPopupContextItem("item"))
						{
							if (ImGui::Selectable("Set to zero")) { *velocityX = 0.0f; *velocityY = 0.0f; }
							ImGui::PushItemWidth(200);
							ImGui::DragFloat("#velocityX", velocityX, 0.01f, -0.3f, 0.3f);
							ImGui::PopItemWidth();
							ImGui::PushItemWidth(200);
							ImGui::DragFloat("#velocityY", velocityY, 0.01f, -0.3f, 0.3f);
							ImGui::PopItemWidth();
							ImGui::EndPopup();
						}
						ImGui::TreePop();
					}
					/*	if (ImGui::TreeNode("Change object"))
						{
							ImGui::Text("Entity:");
							static char str0[128] = "Hello, world!";
							static int i0 = 123;
							ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
							ImGui::InputInt("input int", &i0);
							ImGui::TreePop();
						}*/
				}
				ImGui::End();
			}
		}

	};


	class Actor : public Entity, public IMGUI
	{
	protected:
		std::vector<ObjectLevel> obj;
		Vector2D velocity;

		bool showDebugConsole = false;
		bool isWalk = false;
		bool life = true;
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
		Actor(sf::Image& IMAGE, Vector2D POSITION, Rectangle rect, std::string NAME, sf::RenderWindow& w, Level& lvl) : Entity(IMAGE, POSITION, NAME)
		{
			localRectangle = rect;
			globalRectangle = Rectangle(position.x, position.y, position.x + rect.w, position.y + rect.y);
			obj = lvl.GetAllObjects();
			originOffset = { 90,120 };
			sprite.setOrigin(originOffset);
			sprite.setTextureRect(localRectangle.getSfmlRect_i());
			window = &w;
		}

		void handleEvent(sf::Event& e);
		void checkClashes(Vector2D pos);
		void RotateToMouse(float speed, sf::RenderWindow& window);
		void update(float time) override;
		void getDamage(float dmg);
	};

}

