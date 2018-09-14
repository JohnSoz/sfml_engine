#include "Object.h"
using namespace Engine;
using namespace sf;
Engine::Entity::Entity(sf::Vector2f POSITION, std::string NAME) : Object(POSITION, NAME)
{
}

Engine::Entity::Entity(sf::Image & IMAGE, sf::Vector2f POSITION, std::string NAME) : Object(POSITION, NAME)
{
	texture.loadFromImage(IMAGE);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position);
}

Engine::Entity::Entity(sf::Image & i, sf::IntRect r, sf::Vector2f pos, std::string name) : Object(pos, name)
{
}

Engine::Entity::~Entity()
{
}

void Engine::Actor::RotateToMouse(float speed, sf::RenderWindow& window)
{
	float rotateSpeed = speed;
	bool rot = false;
	LastAngle = sprite.getRotation();

	Vector2i pixelPos = Mouse::getPosition(window);
	Vector2f posMouse = window.mapPixelToCoords(pixelPos, window.getView());
	float a = posMouse.x - sprite.getPosition().x;
	float v = posMouse.y - sprite.getPosition().y;

	Radian = atan2f(v, a);
	float rotation = (atan2f(v, a)) * 180 / 3.14159265;
	CurrAngle = rotation; //?

	auto x = globalRectangle.left;
	auto y = globalRectangle.top;
	auto w = globalRectangle.width;
	auto h = globalRectangle.height;

	float centerX = sprite.getPosition().x;
	float centerY = sprite.getPosition().y;

	auto newX = centerX + (x - centerX) * cos(Radian) - (y - centerY) * sin(Radian); //x,y
	auto newY = centerY + (y - centerY) * cos(Radian) + (x - centerX) * sin(Radian); //y,x
	auto newW = centerX + (w - centerX) * cos(Radian) - (h - centerY) * sin(Radian); //w,h
	auto newH = centerY + (h - centerY) * cos(Radian) + (w - centerX) * sin(Radian); //h,w

	auto newX2 = centerX + (x - centerX) * cos(Radian) - (h - centerY) * sin(Radian); //x,h
	auto newY2 = centerY + (h - centerY) * cos(Radian) + (x - centerX) * sin(Radian); //h,x
	auto newW2 = centerX + (w - centerX) * cos(Radian) - (y - centerY) * sin(Radian); //w,y
	auto newY3 = centerY + (y - centerY) * cos(Radian) + (w - centerX) * sin(Radian); //y,w

	//auto newW2 = centerX + ((w - 30) - centerX) * cos(Radian) - ((y + 15) - centerY) * sin(Radian); //w,y
	//auto newY3 = centerY + ((y + 15) - centerY) * cos(Radian) + ((w - 30) - centerX) * sin(Radian); //y,w

	debugRectangle.left = newX2;//x2
	debugRectangle.top = newY2;//y2
	debugRectangle.width = newW2;//w2
	debugRectangle.height = newY3;//h2

	globalRectangle.left = newX;
	globalRectangle.top = newY;
	globalRectangle.width = newW;
	globalRectangle.height = newH;

	if (CurrAngle > 180) CurrAngle -= 360;
	if (CurrAngle < -180) CurrAngle += 360;
	posMouse.x = CurrAngle - LastAngle;
	posMouse.y = 360 + posMouse.x;
	if (posMouse.y > 360) posMouse.y -= 720;
	if (abs(posMouse.x) > abs(posMouse.y)) posMouse.x = posMouse.y;

	if (CurrAngle == LastAngle)
		return;

	if (abs(posMouse.x) < rotateSpeed)
	{
		LastAngle = CurrAngle;
	}
	else
	{
		if (posMouse.x > 0)
			LastAngle += rotateSpeed;
		else
			LastAngle -= rotateSpeed;
	}
	if (window.hasFocus())
	{
		sprite.setRotation(LastAngle);
	}
}

void Engine::Actor::handleEvent(sf::Event & e)
{
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		isWalk = true;
		direction = Direction::Up;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::S))
	{
		isWalk = true;
		direction = Direction::Down;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::A))
	{
		isWalk = true;
		direction = Direction::Left;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk += true;
		direction = Direction::Right;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else
	{
		isWalk = false;
		(speed > 0) ? speed -= friction : speed = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::T))
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			showDebugConsole = !showDebugConsole;
			Pressclock.restart();
		}
}

void Engine::Actor::checkClashes(sf::Vector2f pos)
{
	for (auto & i : obj)
	{
		sf::FloatRect Intersection;
		sf::FloatRect objGlobalRect = { i.rect.left, i.rect.top, i.rect.width, i.rect.height };
		auto zzxc = sprite.getGlobalBounds();
		if (sprite.getGlobalBounds().intersects(objGlobalRect, Intersection))
		{
			if (i.name == "barrier")
			{
				float offset = (abs(Intersection.width) > abs(Intersection.height)) ? Intersection.height : Intersection.width;
				isCollision = true;
				//	std::cout << "W = " << Intersection.width << " H = " << Intersection.height << " Top = " << Intersection.top << " Left = " << Intersection.left << std::endl;
			}
		}
		else
			isCollision = false;
	}
}

void Engine::Actor::update(float time)
{
	switch (direction)
	{
	case Up:    velocity.y = -speed; velocity.x = 0;  break;
	case Down:  velocity.y = speed; velocity.x = 0;  break;
	case Left:  velocity.x = -speed; velocity.y = 0;  break;
	case Right: velocity.x = speed; velocity.y = 0;  break;
	}
	localRectangle = sprite.getTextureRect();
	globalRectangle = sf::FloatRect(position.x - originOffset.x, position.y - originOffset.y, position.x - originOffset.x + localRectangle.width, position.y - originOffset.y + localRectangle.height);

	RotateToMouse(0.2 * time, *window);
	position += velocity * time;
	checkClashes(position);
	draw(&showDebugConsole, *this);
	if (isWalk)
		sprite.setTextureRect(animManager.AnimUpdate(time));
	sprite.setPosition(position);
}

void Engine::Actor::getDamage(float dmg)
{
	lives -= dmg * ((100 - armor) / 100);
}

void Engine::Debug::ShowHelpMarker(const char * desc)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

Engine::Object::Object(sf::Vector2f pos, std::string n)
{
	position = pos;
	name = n;
}

Engine::Object::Object(sf::Vector2f pos, ObjectType t, std::string n)
{
	position = pos;
	type = t;
	name = n;
	IsActive = true;
}

void Engine::Debug_Actor::actorInfo(bool *open, Actor& a)
{
	if (*open)
	{
		if (ImGui::Begin(a.getName().c_str(), open, ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::BeginChild(a.getName().c_str());
			ImGui::SetWindowSize(size);

			float *energy = &a.energy;
			float *friction = &a.friction;
			float maxSpeed = a.maxSpeed;
			float CurrAngle = a.CurrAngle;
			sf::Vector2f *position = &a.position;
			sf::Vector2f velocity = a.velocity;

			if (ImGui::CollapsingHeader("Actor info"))
			{
				if (ImGui::CollapsingHeader("Test2 info"))
				{
				}
				ImGui::Separator();
				ImGui::Text("energy: %.3f", *energy);
				if (ImGui::BeginPopupContextItem("itemEnergy"))
				{
					if (ImGui::Selectable("Set to zero")) *energy = 0.0f;
					if (ImGui::Selectable("Set to default")) *energy = 0.005;
					ImGui::PushItemWidth(200);
					ImGui::DragFloat("#energy", energy, 0.001f, 0.001f, 0.09f);
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
				ImGui::Separator();
				ImGui::Text("friction: %.3f", *energy);
				if (ImGui::BeginPopupContextItem("itemFriction"))
				{
					if (ImGui::Selectable("Set to zero")) *friction = 0.0f;
					if (ImGui::Selectable("Set to default")) *friction = 0.005;
					ImGui::PushItemWidth(200);
					ImGui::DragFloat("#energy", friction, 0.001f, 0.001f, 0.09f);
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
				ImGui::Spacing();
				ImGui::Text("position X = %.1f | Y = %.1f", position->x, position->y);
				if (ImGui::BeginPopupContextItem("itemPosition"))
				{
					if (ImGui::Selectable("Set to zeroX")) position->x = 0.0f;
					if (ImGui::Selectable("Set to zeroY"))  position->y = 0.0f;
					ImGui::PushItemWidth(200);
					ImGui::DragFloat("#positionX", &position->x, 2, 0, 1000);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(200);
					ImGui::DragFloat("#positionY", &position->y, 2, 0, 1000);
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
				ImGui::Spacing();
				ImGui::Text("maxSpeed: %.2f", maxSpeed);
				if (ImGui::BeginPopupContextItem("itemMaxSpeed"))
				{
					if (ImGui::Selectable("Set to zero")) maxSpeed = 0.0f;
					if (ImGui::Selectable("Set to default")) maxSpeed = 0.3;
					ImGui::PushItemWidth(200);
					ImGui::DragFloat("#maxSpeed", &maxSpeed, 0.05f, 0.05f, 1.5f);
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
				ImGui::Spacing();
				ImGui::Text("CurrAngle: %.2f", CurrAngle);
				ShowHelpMarker("immutable value");
				ImGui::Spacing();
				ImGui::Text("velocity X = %.3f | Y = %.3f", velocity.x, velocity.y);
				ShowHelpMarker("immutable value");
				if (ImGui::TreeNode("Current animation Info"))
				{
					auto animation = *a.animManager.GetCurrAnimation<AnimationXml>();
					ImGui::Text(("Animation name: " + animation.name).c_str());
					ImGui::Separator();
					ImGui::Text("Current frame: %.1f", animation.frame);
					ImGui::Separator();
					ImGui::Text("Speed: %.3f", animation.speed);
					ImGui::Separator();
					ImGui::Text("Frames size(): %.1f", animation.frameCount);
					ImGui::TreePop();
				}
			}
			ImGui::EndChild();
			ImGui::End();
		}
	}
	else
		return;
}

void Engine::Debug_Object::objectInfo(bool *open, Object& a)
{
	if (*open)
	{
		if (ImGui::Begin(a.getName().c_str(), open, ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::SetWindowSize(size);
			bool Active = a.IsActive;
			sf::Vector2f *position = &a.position;
			auto ObjectType = a.type;

			if (ImGui::CollapsingHeader("Object info"))
			{
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Text("isActive: %.1f", Active);
					ShowHelpMarker("1 = True; 0 = False");
					ImGui::Spacing();
					ImGui::Text("objectType: %.1f", ObjectType);
					ShowHelpMarker("OEntity = 1, OPawn, OActor");
					ImGui::Text("position X = %.1f | Y = %.1f", position->x, position->y);
					if (ImGui::BeginPopupContextItem("itemPosition"))
					{
						if (ImGui::Selectable("Set to zeroX")) position->x = 0.0f;
						if (ImGui::Selectable("Set to zeroY"))  position->y = 0.0f;
						ImGui::PushItemWidth(200);
						ImGui::DragFloat("#positionX", &position->x, 2, 0, 1000);
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(200);
						ImGui::DragFloat("#positionY", &position->y, 2, 0, 1000);
						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
			}
			ImGui::End();
		}
	}
	else
		return;
}

void Engine::Debug_Object::draw(bool *open, Object& a)
{
	objectInfo(open, a);
}

void Engine::Debug_Actor::draw(bool *open, Actor& a)
{
	Debug_Object::draw(open, a);
	actorInfo(open, a);
}

