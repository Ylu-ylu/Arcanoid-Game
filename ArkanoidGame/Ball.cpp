#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>
#include <algorithm>
#include <random>
#include "randomizer.h"

namespace
{
	// id textures
	const std::string TEXTURE_ID = "ball";
}

namespace ArkanoidGame
{
	Ball::Ball(const sf::Vector2f& position)
		: GameObject(GameObjectType::Ball, TEXTURES_PATH + TEXTURE_ID + ".png", position, BALL_SIZE, BALL_SIZE)
	{	
		GetSize();
		const float angle = 90;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = std::sin(pi / 180.f * angle);		
	}	

	void Ball::Update(float timeDelta) 
	{
		const auto pos = sprite.getPosition() + BALL_SPEED * timeDelta * direction;
		sprite.setPosition(pos);	

		// Add to Ball class	

		if (pos.x - BALL_SIZE / 2.f <= 0 || pos.x + BALL_SIZE / 2.f >= SCREEN_WIDTH) 
		{
			InvertDirectionX();
		}

		if (pos.y - BALL_SIZE / 2.f <= 0 || pos.y + BALL_SIZE / 2.f >= SCREEN_HEGHT)
		{
			InvertDirectionY();
		}
		BallInverse = false;		
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

	void Ball::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{
		if (type == ColladiableType::Hit|| type == ColladiableType::HitInverse)
		{
			lastAngle += random<float>(-5, 5);
			ChangeAngle(lastAngle);
		}
		if(type == ColladiableType::HitInverse&& !BallInverse)
		{
			BallInverse = true;

			bool needInverseDirX = false;
			bool needInverseDirY = false;

			const auto ballPos = GetPosition();
			const auto blockRect = collidableWhith->GetRect();

			GetBallInverse(ballPos, blockRect, needInverseDirX, needInverseDirY);

			if (needInverseDirX)
			{
				InvertDirectionX();
			}
			if (needInverseDirY)
			{
				InvertDirectionY();
			}
		}		
	}

	void Ball::GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseDirX, bool& needInverseDirY)
	{
		if (ballPos.y > blockRect.top + blockRect.height)
		{
			needInverseDirY = true;
		}
		if (ballPos.x < blockRect.left)
		{
			needInverseDirX = true;
		}
		if (ballPos.x > blockRect.left + blockRect.width)
		{
			needInverseDirX = true;
		}
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