#include "Actor.h"
using sf::Keyboard;

void Engine::Actor::RotateToMouse(float speed, sf::RenderWindow& window)
{
	float rotateSpeed = speed;
	bool rot = false;
	LastAngle = sprite.getRotation();

	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f posMouse = window.mapPixelToCoords(pixelPos, window.getView());
	float a = posMouse.x - sprite.getPosition().x;
	float v = posMouse.y - sprite.getPosition().y;

	Radian = atan2f(v, a);
	float rotation = (atan2f(v, a)) * 180 / 3.14159265;
	CurrAngle = rotation; //?

	auto x = globalRectangle.left;
	auto y = globalRectangle.top;
	auto w = globalRectangle.width;
	auto h = globalRectangle.height;

	float centerX = position.x;
	float centerY = position.y;

	auto newX = centerX + (x - centerX) * cos(Radian) - (y - centerY) * sin(Radian); //x,y
	auto newY = centerY + (y - centerY) * cos(Radian) + (x - centerX) * sin(Radian); //y,x
	auto newW = centerX + (w - centerX) * cos(Radian) - (h - centerY) * sin(Radian); //w,h
	auto newH = centerY + (h - centerY) * cos(Radian) + (w - centerX) * sin(Radian); //h,w

	auto newX2 = centerX + (x - centerX) * cos(Radian) - (h - centerY) * sin(Radian); //x,h
	auto newY2 = centerY + (h - centerY) * cos(Radian) + (x - centerX) * sin(Radian); //h,x
	auto newW2 = centerX + (w - centerX) * cos(Radian) - (y - centerY) * sin(Radian); //w,y
	auto newY3 = centerY + (y - centerY) * cos(Radian) + (w - centerX) * sin(Radian); //y,w

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
		if (direction != Left) speed -= speed * 0.5;
		direction = Direction::Left;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk = true;
		if (direction != Right) speed -= speed * 0.5;
		direction = Direction::Right;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else
	{
		isWalk = false;
		(speed > 0) ? speed -= friction : speed = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::U))
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
	if (!debug_actor.isSelected)
	{
		auto age = meta::getMemberValue<ObjectType>(*static_cast<Object*>(this), "type");
		std::cout << "Got person's age: " << age << '\n';
		switch (direction)
		{
		case Up:    velocity.y = -speed; velocity.x = 0;  break;
		case Down:  velocity.y = speed; velocity.x = 0;  break;
		case Left:  velocity.x = -speed; velocity.y = 0;  break;
		case Right: velocity.x = speed; velocity.y = 0;  break;
		}

		localRectangle = sprite.getTextureRect();
		sf::Vector2f pos = { position.x - originOffset.x * scale ,position.y - originOffset.y * scale };
		globalRectangle = sf::FloatRect(pos.x, pos.y, pos.x + localRectangle.width * scale, pos.y + localRectangle.height * scale);
		RotateToMouse(0.2 * time, *window);
		position += velocity * time;
		checkClashes(position);

		if (isWalk)
			sprite.setTextureRect(animManager.AnimUpdate(time));
		sprite.setPosition(position);
	}
	debug_actor.draw(&showDebugConsole, *this);
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
void Engine::Debug_Actor::actorInfo(bool *open, Actor& a)
{
	if (*open)
	{
		if (ImGui::Begin(a.getName().c_str(), open, ImGuiWindowFlags_NoSavedSettings))
		{
			if (ImGui::BeginChild(a.getName().c_str()))
			{
				ImGui::SetWindowSize(size);

				float *energy = &a.energy;
				float *friction = &a.friction;
				float *maxSpeed = &a.maxSpeed;
				float CurrAngle = a.CurrAngle;
				sf::Vector2f *position = &a.position;
				sf::Vector2f velocity = a.velocity;

				ImGui::NextColumn();
				if (ImGui::TreeNode("Actor info"))
				{
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
					ImGui::Text("friction: %.3f", *friction);
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
					ImGui::Text("maxSpeed: %.2f", *maxSpeed);
					if (ImGui::BeginPopupContextItem("itemMaxSpeed"))
					{
						if (ImGui::Selectable("Set to zero")) *maxSpeed = 0.0f;
						if (ImGui::Selectable("Set to default")) *maxSpeed = 0.3;
						ImGui::PushItemWidth(200);
						ImGui::DragFloat("#maxSpeed", maxSpeed, 0.01f, 0.01f, 1.5f);
						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
					ImGui::Spacing();
					ImGui::Text("CurrAngle: %.2f", CurrAngle);
					ShowHelpMarker("immutable value");
					ImGui::Spacing();
					ImGui::Text("velocity X = %.3f | Y = %.3f", velocity.x, velocity.y);
					ShowHelpMarker("immutable value");
					ImGui::NextColumn();
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
						if (ImGui::TreeNode("Current animation Image"))
						{
							sf::Sprite spr = a.sprite;
							float size_x = a.localRectangle.width;
							float size_y = a.localRectangle.height;
							ImGui::Text("%.0fx%.0f", size_x, size_y);
							ImGui::Image(*spr.getTexture(), sf::Vector2f(size_x, size_y), (sf::FloatRect)a.localRectangle);
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				ImGui::EndChild();
			}
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
			isSelected = ImGui::IsAnyWindowFocused();
			ImGui::SetWindowSize(size);
			bool Active = a.IsActive;
			sf::Vector2f *position = &a.position;
			auto ObjectType = a.type;
			if (ImGui::TreeNode("Object info"))
			{
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Text("isActive: %i", (int)Active);
				ShowHelpMarker("1 = True; 0 = False");
				ImGui::Spacing();
				ImGui::Text("objectType: %i", (int)ObjectType);
				ShowHelpMarker("None = 0, OEntity, OPawn, OActor");
				ImGui::Text("position X = %.2f | Y = %.2f", position->x, position->y);
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
				ImGui::TreePop();
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
sf::FloatRect Engine::operator*(const sf::FloatRect& rect, float scale)
{
	sf::FloatRect ret;
	ret.width = rect.width * scale;
	ret.height = rect.height * scale;
	ret.left = rect.left * scale;
	ret.top = rect.top * scale;
	return ret;
}
