#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>
#include <algorithm>
#include <random>

namespace
{
	// id textures
	const std::string TEXTURE_ID = "ball";
}

namespace ArkanoidGame
{
	Ball::Ball(const sf::Vector2f& position)
		: GameObject(TEXTURES_PATH + TEXTURE_ID + ".png", position,BALL_SIZE, BALL_SIZE)
	{
		const float angle = 90;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = std::sin(pi / 180.f * angle);
	}

	void Ball::Init()
	{
		assert(texture.loadFromFile(TEXTURES_PATH + TEXTURE_ID + ".png"));
		InitSprite(sprite, BALL_SIZE, BALL_SIZE, texture);
		SetSpriteRandomPosition(sprite, { 0, 0, SCREEN_WIDTH, SCREEN_HEGHT-SCREEN_HEGHT/2 }, {});
	}

	void Ball::Update(float timeDelta)
	{
		const auto pos = sprite.getPosition() + BALL_SPEED * timeDelta * direction;
		sprite.setPosition(pos);

		if (pos.x - BALL_SIZE / 2.f <= 0 || pos.x + BALL_SIZE / 2.f >= SCREEN_WIDTH) 
		{
			direction.x *= -1;
		}

		if (pos.y - BALL_SIZE / 2.f <= 0 || pos.y + BALL_SIZE / 2.f >= SCREEN_HEGHT)
		{
			direction.y *= -1;
		}
		
	}
	void Ball::Draw(sf::RenderWindow& window) 
	{
		DrawSprite(sprite, window);
	}

	bool Ball::CheckCollisionWhithPlatform(const Platform& platform)
	{
		return platform.GetRect().intersects(platform.GetRect());
	}
	void Ball::InvertDirectionX()
	{
		direction.x *= -1;
	}

	void Ball::InvertDirectionY()
	{
		direction.y *= -1;
	}

	float Ball::GetSize() const
	{
		return BALL_SIZE;
	}

	void Ball::ChangeAngle(float angle)
	{
		lastAngle = angle;
		const auto pi = std::acos(-1.f);
		direction.x = (angle / abs(angle)) * std::cos(pi / 180.f * angle);
		direction.y = -1 * abs(std::sin(pi / 180.f * angle));
	}

	void Ball::restart()
	{
		GameObject::restart();
		const float angle = 90;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = std::sin(pi / 180.f * angle);
	}
}