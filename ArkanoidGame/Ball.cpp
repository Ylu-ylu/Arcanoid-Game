#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>
#include <algorithm>
#include <random>
#include "randomizer.h"
#include "Block.h"


namespace
{
	// id textures
	const std::string TEXTURE_ID = "ball";
}

namespace ArkanoidGame
{

	Ball::Ball(const sf::Vector2f& position)
		: GameObject(GameObjectType::Ball, SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, SETTINGS.BALL_SIZE, SETTINGS.BALL_SIZE)
	{			
		GetSize();
		const float angle = 90;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = std::sin(pi / 180.f * angle);
	}	

	void Ball::Update(float timeDelta) 
	{
		const auto pos = sprite.getPosition() + SETTINGS.BALL_SPEED *speedMultiplier* timeDelta * direction;
		sprite.setPosition(pos);			

		if (pos.x - SETTINGS.BALL_SIZE / 2.f <= 0 || pos.x + SETTINGS.BALL_SIZE / 2.f >= SETTINGS.SCREEN_WIDTH)
		{
			InvertDirectionX();
		}

		if (pos.y - SETTINGS.BALL_SIZE / 2.f <= 0 || pos.y + SETTINGS.BALL_SIZE / 2.f >= SETTINGS.SCREEN_HEGHT)
		{
			InvertDirectionY();
		}	
		
		Emit();
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
		return SETTINGS.BALL_SIZE;
	}

	void Ball::SetDefaultState()
	{
		defaultTexture.loadFromFile(SETTINGS.TEXTURES_PATH + "ball.png");
		// Visual changes
		sprite.setTexture(defaultTexture, true); 
		sprite.setColor(sf::Color::White);
		sprite.setScale(1.5f, 1.5f);

		// Gameplay changes
		SetFireballMode(false);
		SetSpeedMultiplier(1.f);		
	}

	void Ball::SetFireballState()
	{
		fireballTexture.loadFromFile(SETTINGS.TEXTURES_PATH + "FireBall.png");

		// Visual changes
		sprite.setTexture(fireballTexture, true);  // true = reset texture rect
		sprite.setColor(sf::Color(255, 100, 0));   // Orange tint
		sprite.setScale(0.25f, 0.25f);              // Slightly larger

		// Gameplay changes
		SetFireballMode(true);
		SetSpeedMultiplier(1.5f);		
	}

	void Ball::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{			
		
		// If in fireball mode and hitting a block, don't bounce off it
		if (fireballMode && collidableWhith &&
			dynamic_cast<Block*>(collidableWhith.get()) != nullptr) 
		{
			return;						
		}
		// If we get here, it's a normal collision (not fireball-block)
		if (type == ColladiableType::Hit || type == ColladiableType::HitInverse)
		{
			lastAngle += random<float>(-5, 5);
			ChangeAngle(lastAngle);
		}

		if (type == ColladiableType::HitInverse)
		{
			bool needInverseDirX = true;
			bool needInverseDirY = true;

			const auto ballPos = GetPosition();

			if (collidableWhith)
			{
				const auto blockRect = collidableWhith->GetRect();
				GetBallInverse(ballPos, blockRect, needInverseDirX, needInverseDirY);
			}

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

	ColladiableType Ball::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
	{
		// If in fireball mode and the collidable is a Block, return Overlap instead of Hit
		if (fireballMode && collidableObject)
		{
			Block* blockPtr = dynamic_cast<Block*>(collidableObject.get());
			if (blockPtr) 
			 {
				// Return Overlap type to prevent bouncing
				return ColladiableType::Overlap;
			}
		}

		// Otherwise use the standard collision detection
		return Colladiable::GetCollision(collidableObject);
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
		GameObject::sprite.setPosition(SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEGHT / 2.f);
		const float angle = 90;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = std::sin(pi / 180.f * angle);		

		// Reset to default state when restarting
		SetDefaultState();
	}	
}