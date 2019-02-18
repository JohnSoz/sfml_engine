#include "Actor.h"
#include "Math.h"
using sf::Keyboard;

void Engine::Actor::RotateToMouse(float speed, sf::RenderWindow& window)
{
	//LastAngle = sprite.getRotation();

	//sf::Vector2f posMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
	//float a = posMouse.x - sprite.getPosition().x;
	//float v = posMouse.y - sprite.getPosition().y;

	//Radian = atan2f(v, a);
	//CurrAngle = (atan2f(v, a)) * 180 / PI;
	//if (CurrAngle > 180) CurrAngle -= 360;
	//if (CurrAngle < -180) CurrAngle += 360;
	//posMouse.x = CurrAngle - LastAngle;
	//posMouse.y = 360 + posMouse.x;
	//if (posMouse.y > 360) posMouse.y -= 720;
	//if (abs(posMouse.x) > abs(posMouse.y)) posMouse.x = posMouse.y;

	//if (CurrAngle != LastAngle)
	//{
	//	if (abs(posMouse.x) < speed)
	//	{
	//		LastAngle = CurrAngle;
	//	}
	//	else
	//	{
	//		if (posMouse.x > 0)
	//			LastAngle += speed;
	//		else
	//			LastAngle -= speed;
	//	}
	//}

	//float lastradian = LastAngle * PI / 180;
	//if (lastradian <= 1 && lastradian >= -2.5)
	//	PointOfFire.setPosition(sf::Vector2f(position.x + cosf(lastradian - 0.1) * 46, position.y + sinf(lastradian - 0.1) * 46));
	//else
	//	PointOfFire.setPosition(sf::Vector2f(position.x + cosf(lastradian - 0.05) * 46, position.y + sinf(lastradian - 0.05) * 46));

	////////////////////////////////////////////////rotate debug rectangle////////////////////////
	//auto x = globalRectangle.left;
	//auto y = globalRectangle.top;
	//auto w = globalRectangle.width;
	//auto h = globalRectangle.height;

	//float centerX = position.x;
	//float centerY = position.y;

	//auto newX = centerX + (x - centerX) * cos(lastradian) - (y - centerY) * sin(lastradian); //x,y
	//auto newY = centerY + (y - centerY) * cos(lastradian) + (x - centerX) * sin(lastradian); //y,x
	//auto newW = centerX + (w - centerX) * cos(lastradian) - (h - centerY) * sin(lastradian); //w,h
	//auto newH = centerY + (h - centerY) * cos(lastradian) + (w - centerX) * sin(lastradian); //h,w

	//auto newX2 = centerX + (x - centerX) * cos(lastradian) - (h - centerY) * sin(lastradian); //x,h
	//auto newY2 = centerY + (h - centerY) * cos(lastradian) + (x - centerX) * sin(lastradian); //h,x
	//auto newW2 = centerX + (w - centerX) * cos(lastradian) - (y - centerY) * sin(lastradian); //w,y
	//auto newY3 = centerY + (y - centerY) * cos(lastradian) + (w - centerX) * sin(lastradian); //y,w

	//debugRectangle = sf::FloatRect(newX2, newY2, newW2, newY3);
	//globalRectangle = sf::FloatRect(newX, newY, newW, newH);

	//if (window.hasFocus())
	//	sprite.setRotation(LastAngle);
}

//Engine::Bullet * Engine::Actor::shotUpdate(Level & lvl)
//{
//	//if (!inv.getShowGui())
//	//	if (isShoot && !Engine::VStaticContainer::ShowDebugWindow&& ImGui::GetIO().MetricsRenderWindows < 2)
//	//	{
//	//		auto item = inventory.getCurrItem<Gun>();
//	//		if (gunClock.getElapsedTime().asMilliseconds() > item->getRate())
//	//		{
//	//			animManager.SetCurrAnimation(animManager.GetAnimationByName("handGunShoot"));
//	//			updateSprite();
//	//			gunClock.restart();
//
//	//			sf::Image i;
//	//			i.loadFromFile("Data/images/bullet.png");
//	//			return new Engine::Bullet(i, sf::IntRect(0, 0, 16, 16), getPointOfFire(), "Bullet", Radian, item->getDamage(), lvl, name);
//	//		}
//	//	}
//	return nullptr;
//}

void Engine::Actor::updateSprite()
{
	auto currAnim = animManager.GetCurrAnimation<AnimationXml>();
	localRectangle = currAnim->rect;
	originOffset = currAnim->origin;
	scale = currAnim->scale;
	sprite.setTexture(currAnim->texture);
	sprite.setScale(scale, scale);
	sprite.setOrigin(originOffset);
	sprite.setTextureRect(localRectangle);
	sprite.setPosition(position);
}

Engine::Actor::Actor(sf::Image & IMAGE, sf::Vector2f POSITION, std::string NAME, sf::RenderWindow & w, Level & lvl) : Entity(IMAGE, POSITION, NAME)
{
	type = OActor;
	direction = Left;
	globalRectangle = sf::FloatRect(position.x, position.y, position.x + localRectangle.width, position.y + localRectangle.top);
	obj = lvl.GetAllObjects();
	window = &w;
	//PointOfFire.setPosition(position);
	animManager.LoadAnimation_x("TestAnimation.xml");
	auto object_level = *std::find_if(obj.begin(), obj.end(), [](auto Obj) { return (Obj.name == "PlayerSpawn"); });
	position = sf::Vector2f(object_level.rect.left + object_level.rect.width / 2, object_level.rect.top + object_level.rect.height);
	updateSprite();
}

void Engine::Actor::handleEvent(sf::Event & e)
{
	/*if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
	{
		isShoot = true;
	}
	else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
	{
		isShoot = false;
	}*/
	/*if (inv.getShowGui())
		inv.handleEvent(e);*/
}

void Engine::Actor::isKeyPressed()
{
	/*if (Keyboard::isKeyPressed(Keyboard::W))
	{
		isWalk = true;
		if (direction != Up) speed -= speed * 0.8;
		direction = Direction::Up;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::S))
	{
		isWalk = true;
		if (direction != Down) speed -= speed * 0.8;
		direction = Direction::Down;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::A))
	{
		isWalk = true;
		if (direction != Left) speed -= speed * 0.8;
		direction = Direction::Left;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk = true;
		if (direction != Right) speed -= speed * 0.8;
		direction = Direction::Right;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else
	{
		isWalk = false;
		(speed > 0) ? speed -= friction : speed = 0;
	}*/
	/*if (Keyboard::isKeyPressed(Keyboard::I))
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			inv.getShowGui() = (inv.getShowGui() == true) ? false : true;
			inv.activateOrDisabled();
			Pressclock.restart();
		}

	if (Keyboard::isKeyPressed(Keyboard::Num1))
		inventory.setItemByIndex(0);

	if (Keyboard::isKeyPressed(Keyboard::Num2))
		inventory.setItemByIndex(1);*/

}


void Engine::Actor::checkClashes(float time)
{
	//collision::SAT sat;
	//for (auto& i : obj)
	//{
	//	auto playerRect = Rectangle::fromSfmlRect(sprite.getGlobalBounds());
	//	auto objectRect = Rectangle::fromSfmlRect(i.rect);
	//	if (i.name == "barrier")
	//	{
	//		sf::Sprite spr;
	//		spr.setTextureRect(sf::IntRect(0, 0, i.rect.width, i.rect.height));
	//		spr.setPosition(i.rect.left, i.rect.top);
	//		if (sat.collision(&sprite, &spr))
	//		{
	//			isCollision = true;
	//			if (abs(speed) > 0)
	//			{
	//				auto offset = Rectangle::GetIntersectionDepth(playerRect, objectRect);
	//				isWalk = false;
	//				if (offset.x == 0)
	//					offset.y *= time * 0.2;
	//				else
	//					offset.x *= time * 0.2;
	//				position = position + offset.GetSfmlVector();
	//			}
	//		}
	//	}
	//}
}

void Engine::Actor::CollisionUpdate(Entity * entity)
{
	auto playerRect = Rectangle::fromSfmlRect(sprite.getGlobalBounds());
	auto objectRect = Rectangle::fromSfmlRect(entity->getRect());
	auto offset = Rectangle::GetIntersectionDepth(playerRect, objectRect);
	position = position + offset.GetSfmlVector();
}

void Engine::Actor::update(float time)
{
//	if (inv.getShowGui() == false)
//	{
//		/*switch (direction)
//		{
//		case Up:    velocity.y = -speed; velocity.x = 0;  break;
//		case Down:  velocity.y = speed; velocity.x = 0;  break;
//		case Left:  velocity.x = -speed; velocity.y = 0;  break;
//		case Right: velocity.x = speed; velocity.y = 0;  break;
//		}
//*/
//		localRectangle = sprite.getTextureRect();
//		sf::Vector2f pos = { position.x - originOffset.x * scale, position.y - originOffset.y * scale };
//		globalRectangle = sf::FloatRect(pos.x, pos.y, pos.x + localRectangle.width * scale, pos.y + localRectangle.height * scale);
//		RotateToMouse(0.2 * time, *window);
//		position += velocity * time;
//		checkClashes(time);
//		if (animManager.GetCurrAnimation<AnimationXml>()->name == "handGunShoot")
//		{
//			sprite.setTextureRect(animManager.AnimUpdate(time));
//			if (animManager.GetCurrAnimation<AnimationXml>()->name == "handGunShoot" && animManager.GetCurrAnimation<AnimationXml>()->state == AEnd)
//			{
//				animManager.GetCurrAnimation<AnimationXml>()->state = APause;
//				animManager.SetCurrAnimation(animManager.GetAnimationByName("nandGunMove"));
//				updateSprite();
//			}
//		}
//		else
//			if (isWalk)
//				sprite.setTextureRect(animManager.AnimUpdate(time));
//
//		sprite.setPosition(position);
//	}
//	inventory.update();
//	dw_a.draw("Actor", true);
//	dw_o.draw("Object");
}

void Engine::Actor::start()
{
	//starts when Game::state == Engine::StartGame
}


void Engine::DebugWindow::actorInfo(bool* open, Actor & a)
{
	if (*open)
	{
		if (ImGui::Begin(a.getName().c_str(), open, ImGuiWindowFlags_NoSavedSettings))
		{
			/*if (ImGui::BeginChild(a.getName().c_str()))
			{
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::SetWindowSize(size);*/
				/*float* energy = &a.energy;
				float* friction = &a.friction;
				float* maxSpeed = &a.maxSpeed;
				float CurrAngle = a.CurrAngle;
				bool Walk = a.isWalk;
				sf::Vector2f* position = &a.position;
				sf::Vector2f velocity = a.velocity;

				ImGui::NextColumn();
				if (ImGui::TreeNode("Actor info"))
				{
					ImGui::Separator();
					ImGui::Text("energy: %.3f", *energy);
					if (ImGui::BeginPopupContextItem("itemEnergy"))
					{
						if (ImGui::Selectable("Set to zero")) * energy = 0.0f;
						if (ImGui::Selectable("Set to default")) * energy = 0.005;
						ImGui::PushItemWidth(200);
						ImGui::DragFloat("#energy", energy, 0.001f, 0.001f, 0.09f);
						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
					ImGui::Separator();
					ImGui::Text("friction: %.3f", *friction);
					if (ImGui::BeginPopupContextItem("itemFriction"))
					{
						if (ImGui::Selectable("Set to zero")) * friction = 0.0f;
						if (ImGui::Selectable("Set to default")) * friction = 0.005;
						ImGui::PushItemWidth(200);
						ImGui::DragFloat("#energy", friction, 0.001f, 0.001f, 0.09f);
						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
					ImGui::Separator();
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
					ImGui::Separator();
					ImGui::Text("maxSpeed: %.2f", *maxSpeed);
					if (ImGui::BeginPopupContextItem("itemMaxSpeed"))
					{
						if (ImGui::Selectable("Set to zero")) * maxSpeed = 0.0f;
						if (ImGui::Selectable("Set to default")) * maxSpeed = 0.3;
						ImGui::PushItemWidth(200);
						ImGui::DragFloat("#maxSpeed", maxSpeed, 0.01f, 0.01f, 1.5f);
						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
					ImGui::Separator();
					ImGui::Text("isWalk: %i", Walk);
					ShowHelpMarker("immutable value");
					ImGui::Separator();
					ImGui::Text("CurrAngle: %.2f", CurrAngle);
					ShowHelpMarker("immutable value");
					ImGui::Separator();
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
			}
			ImGui::EndChild();*/
		}
		ImGui::End();

	}
	else
		return;
}

void Engine::DebugWindow::ShowHelpMarker(const char* desc)
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

void Engine::DebugWindow::objectInfo(bool* open, Object & a)
{
	//isSelected = *open;
	if (*open)
	{
		if (ImGui::Begin(a.getName().c_str(), open, ImGuiWindowFlags_NoSavedSettings))
		{
			if (ImGui::Button("PlayerMovement"))
				isSelected = !isSelected;
			ImGui::Spacing();
			ImGui::SetWindowSize(size);
			bool Active = a.IsActive;
			sf::Vector2f * position = &a.position;
			auto ObjectType = a.type;
			if (ImGui::TreeNode("Object info"))
			{
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Text("isActive: %i", (int)Active);
				ShowHelpMarker("1 = True; 0 = False");
				ImGui::Separator();
				ImGui::Text("objectType: %i", (int)ObjectType);
				ShowHelpMarker("None = 0, OEntity, OPawn, OActor");
				ImGui::Separator();
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



sf::Vector2f Engine::operator+(const sf::Vector2f & rect, float scale)
{
	return sf::Vector2f(rect.x + scale, rect.y + scale);
}

