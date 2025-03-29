#include "Platform.h"
#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <algorithm>
#include <assert.h>



namespace
{
	const std::string TEXTURE_ID = "platform";
}

namespace ArkanoidGame
{
	Platform::Platform(const sf::Vector2f& position)
		: GameObject(GameObjectType::Platform, TEXTURES_PATH + TEXTURE_ID + ".png", position, PLATFORM_WIDTH, PLATFORM_HEIGHT)
	{
	}
	void Platform::Update(float timeDelta)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Move(-timeDelta * PLATFORM_SPEED);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Move(timeDelta *PLATFORM_SPEED);
		}
	}

	bool Platform::GetCollision(std::shared_ptr<Colladiable> collidable) const
	{
		auto ball = std::static_pointer_cast<Ball>(collidable);
		if (!ball) return false;

		auto sqr = [](float x) 
			{
			return x * x;
			};
		const auto rect = sprite.getGlobalBounds();
		const auto ballPos = ball->GetPosition();
		if (ballPos.x < rect.left) 
		{
			return sqr(ballPos.x - rect.left) + sqr(ballPos.y - rect.top) < sqr(BALL_SIZE / 2.0);
		}

		if (ballPos.x > rect.left + rect.width) 
		{
			return sqr(ballPos.x - rect.left - rect.width) + sqr(ballPos.y - rect.top) < sqr(BALL_SIZE / 2.0);
		}

		return std::fabs(ballPos.y - rect.top) <= BALL_SIZE / 2.0;
	}

	bool Platform::CheckCollision(std::shared_ptr<Colladiable> collidable)
	{
		auto ball = std::static_pointer_cast<Ball>(collidable);
		if (!ball)
			return false;

		if (GetCollision(ball)) 
		{
			auto rect = GetSpriteRect();
			auto ballPosInOlatform = (ball->GetPosition().x - (rect.left + rect.width / 2)) / (rect.width / 2);
			ball->ChangeAngle(90 - 20 * ballPosInOlatform);
			return true;
		}
		return false;
	}

	void Platform::Move(float speed)
	{
		auto position = sprite.getPosition();
		position.x = std::min(std::max(position.x + speed, PLATFORM_WIDTH / 2.f), SCREEN_WIDTH - PLATFORM_WIDTH / 2.f);
		sprite.setPosition(position);
	}
			
	bool Platform::CheckCollisionWithWindowSides() const
	{
		const auto position = sprite.getPosition();
		const auto halfWidth = PLATFORM_WIDTH / 2.f;

		// Check collision with the left side of the window
		if (position.x - halfWidth <= 0.f)
		{
			return true;
		}
		// Check collision with the right side of the window
		if (position.x + halfWidth >= SCREEN_WIDTH)
		{
			return true;
		}
		return false;
	}
}