#include "Object.h"
using namespace Engine;
using namespace sf;
Engine::Entity::Entity(Vector2D POSITION, std::string NAME) : position(POSITION), name(NAME)
{
}

Engine::Entity::Entity(sf::Image & IMAGE, Vector2D POSITION, std::string NAME) : position(POSITION), name(NAME)
{
	texture.loadFromImage(IMAGE);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position.GetSfmlVector());
}

Engine::Entity::Entity(sf::Image & i, sf::IntRect r, Vector2D pos, std::string name)
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

	if (e.type = sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::T)
		{
			if (Pressclock.getElapsedTime().asMilliseconds() > 500)
			{
				showDebugConsole = !showDebugConsole;
				Pressclock.restart();
			}
		}
	}
}

void Engine::Actor::checkClashes(Vector2D pos)
{
	for (int i = 0; i < obj.size(); i++)
	{
		sf::FloatRect Intersection;
		sf::FloatRect objGlobalRect = { obj[i].rect.left, obj[i].rect.top, obj[i].rect.width, obj[i].rect.height };
		auto zzxc = sprite.getGlobalBounds();
		if (sprite.getGlobalBounds().intersects(objGlobalRect, Intersection))
		{
			if (obj[i].name == "barrier")
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
	case Down:  velocity.y =  speed; velocity.x = 0;  break;
	case Left:  velocity.x = -speed; velocity.y = 0;  break;
	case Right: velocity.x =  speed; velocity.y = 0;  break;
	}
	localRectangle = sprite.getTextureRect();
	globalRectangle = sf::FloatRect(position.x - originOffset.x, position.y - originOffset.y, position.x - originOffset.x + localRectangle.width, position.y - originOffset.y + localRectangle.height);

	RotateToMouse(0.2 * time, *window);
	position += velocity * time;
	checkClashes(position);
	objectStatusInfo<float*, float*, float*, float*, float*>(&showDebugConsole, name, std::make_tuple(&energy, &velocity.x, &velocity.y, &maxSpeed, &CurrAngle));
	sprite.setTextureRect(animManager.AnimUpdate(time));
	sprite.setPosition(position.GetSfmlVector());
}

void Engine::Actor::getDamage(float dmg)
{
	lives -= dmg * ((100 - armor) / 100);
}

void Engine::IMGUI::ShowHelpMarker(const char * desc)
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
