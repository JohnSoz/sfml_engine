#pragma once
#include "Entity.h"
#include "AnimationManager.h"
#include "Inventory.h"
#include <MetaStuff/Meta.h>
#include "JsonLoader.h"
#include "InventoryMenu.h"
using namespace meta;
namespace Engine
{
	class Actor;

	class DebugWindow
	{
	private:
		void ShowHelpMarker(const char* desc);
		ImVec2 size = { 400,400 };
		std::vector<Object*> objectInf;
	public:
		bool isSelected;
		void pushObjectToDebug(Object& o) { objectInf.push_back(&o); }
		void actorInfo(bool *open, Actor& a);
		void objectInfo(bool *open, Object& a);
		void draw(bool *open)
		{
			for (auto i : objectInf)
			{
				auto z = static_cast<Entity*>(i);
				switch (i->type)
				{
				case OActor:
					objectInfo(open, *i);
					actorInfo(open, *(Actor*)(z));
					break;
				case None:
					objectInfo(open, *i);
					break;
				}
			}
		}
	};


	sf::Vector2f operator + (const sf::Vector2f& rect, float scale);
	class Actor : public Entity
	{
	protected:
		std::vector<ObjectLevel> obj;
		sf::Vector2f velocity;
		Inventory inventory;
		AnimationManager animManager;
		DebugWindows<Actor> dw_a;
		InventoryMenu inv;

		bool isWalk;
		bool isCollision;

		float lives, armor;
		float speed, energy, friction, maxSpeed;
		float scale = 0.5;
		float CurrAngle = sprite.getRotation(), LastAngle;

		sf::Clock Pressclock;
		sf::Clock gunClock;
		sf::Vector2f originOffset = {};
		Direction direction = Direction::State;
		sf::RenderWindow* window;
		sf::Transformable PointOfFire;

		void updateSprite();

	public:
		float Radian;
		~Actor() = default;
		Actor() = delete;
		Actor(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME, sf::RenderWindow& w, Level& lvl) : Entity(IMAGE, POSITION, NAME), inv("Data/GUI/MyUI/MainMenu.txt", w)
		{
			type = OActor;
			dw_a.set(this);
			animManager.LoadAnimation_x("MoveHandGun.xml");
			auto currAnim = animManager.GetCurrAnimation<AnimationXml>();
			animManager.LoadAnimation_x("ShootHandGun.xml");
			lives = armor = 100;
			speed = 0;
			energy = 0.001; friction = 0.005;
			maxSpeed = 0.2;
			localRectangle = currAnim->rect;
			globalRectangle = sf::FloatRect(position.x, position.y, position.x + localRectangle.width, position.y + localRectangle.top);
			obj = lvl.GetAllObjects();
			originOffset = currAnim->origin;
			sprite.setTexture(currAnim->texture);
			sprite.setOrigin(originOffset);
			sprite.setTextureRect(localRectangle);
			sprite.setScale(scale, scale);
			window = &w;
			PointOfFire.setPosition(position);
			inv.makeMenu(inventory);
		}
		sf::Vector2f getPointOfFire() { return PointOfFire.getPosition(); }

		virtual void handleEvent(sf::Event& e);

		void checkClashes(float time);

		void RotateToMouse(float speed, sf::RenderWindow& window);

		Engine::Bullet* shotUpdate(Level& lvl)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!Engine::VStaticContainer::ShowDebugWindow && ImGui::GetIO().MetricsRenderWindows < 2)
				{
					if (gunClock.getElapsedTime().asMilliseconds() > 800)
					{
						animManager.SetCurrAnimation(animManager.GetAnimationByName("handGunShoot"));
						updateSprite();
						gunClock.restart();

						auto item = inventory.getCurrItem<Gun>();
						sf::Image i;
						i.loadFromFile("Data/images/bullet.png");
						return new Engine::Bullet(i, sf::IntRect(0, 0, 16, 16), getPointOfFire(), "Bullet", Radian, item->getDamage(), lvl);
					}
				}
			}
			return nullptr;
		}

		void draw() { inv.draw(); }

		void invHandleEvent(sf::Event&);

		void update(float time) override;

		void start() override;

		void getDamage(float dmg);

		friend class DebugWindow;
		friend auto meta::registerMembers<Engine::Actor>();
	};
}

namespace meta
{
	template <>
	inline auto registerMembers<Engine::Actor>()
	{
		return members(
			member("direction", &Engine::Actor::direction),
			member("velocity", &Engine::Actor::velocity),
			member("friction", &Engine::Actor::friction),
			member("CurrAngle", &Engine::Actor::CurrAngle),
			member("maxSpeed", &Engine::Actor::maxSpeed),
			member("energy", &Engine::Actor::energy)
		);
	}
}