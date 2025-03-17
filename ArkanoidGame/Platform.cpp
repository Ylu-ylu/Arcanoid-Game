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
		: GameObject(TEXTURES_PATH + TEXTURE_ID + ".png", position, PLATFORM_WIDTH, PLATFORM_HEIGHT)
	{
	}
	void Platform::Init()
	{
		assert(texture.loadFromFile(TEXTURES_PATH + TEXTURE_ID + ".png"));
		InitSprite(sprite, PLATFORM_WIDTH, PLATFORM_HEIGHT, texture);
		sprite.setPosition(SCREEN_WIDTH/2.f, SCREEN_HEGHT- PLATFORM_HEIGHT/2.f);
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
	void Platform::Draw(sf::RenderWindow& window) 
	{
		DrawSprite(sprite, window);
	}
	bool Platform::CheckCollisionWhithBall( Ball& ball) const
	{// Get the platform's rectangle
		sf::FloatRect platformRect = GetRect();

		// Get the ball's bounding circle
		sf::Vector2f ballPosition = ball.GetPosition();
		float ballRadius = ball.GetSize() / 2.0f;
		sf::FloatRect ballRect(
			ballPosition.x - ballRadius,
			ballPosition.y - ballRadius,
			ballRadius * 2,
			ballRadius * 2
		);

		// Check if the rectangles intersect
		if (platformRect.intersects(ballRect)) 
		{
			// Ball is colliding with platform

			// Calculate where on the platform the ball hit (from -1.0 to 1.0)
			float hitPosition = (ballPosition.x - (platformRect.left + platformRect.width / 2)) / (platformRect.width / 2);

			// Clamp the hit position to [-1, 1] range
			hitPosition = std::max(-1.0f, std::min(hitPosition, 1.0f));

			// Change the ball's angle based on where it hit
			// This will give different angles depending on where the ball hits the platform
			// Middle = straight up, edges = more horizontal angle
			float angle = hitPosition * 60.0f; // 60 degrees max deflection

			// Update the ball's direction
			ball.ChangeAngle(angle);

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