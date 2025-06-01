#include "Block.h"
#include "Sprite.h"
#include "GameSettings.h"
#include <assert.h>
#include "Ball.h"
#include "Bonus.h"

namespace
{
	// id textures
	//const std::string TEXTURE_ID = "block";
	const std::string TEXTURE_ID = "BVlocks";
}

namespace ArkanoidGame
{
	void Block::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{		
		if (type != ColladiableType::None && type != ColladiableType::Overlap)
		{
			// If FragileBlocks bonus is active, force break
			if (FragileBlocksBonus::IsActive())
			{
				hitCount = 0; // Force break				
			}
			else
			{
				// Normal hit processing
				this->hitCount -= hitDamage;
			}
			// Notify observers
			Emit();
		}
		
	}
	Block::Block(const sf::Vector2f& position, const sf::Color& color, int points) :
		GameObject(GameObjectType::Block, SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT), points(points) 
	{
		sprite.setColor(color);
	}	

	ColladiableType Block::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
	{			
			// Get the base collision type
		ColladiableType type = Colladiable::GetCollision(collidableObject);

		// If there's a collision
		if (type != ColladiableType::None)
		{
			// Check if the colliding object is a Ball in FireBall mode
			auto ball = std::dynamic_pointer_cast<Ball>(collidableObject);
			if (ball && ball->IsFireballMode())
			{				
				return ColladiableType::Overlap; // Allow FireBall to pass through
			}

			// For regular collisions, convert Hit to HitInverse
			if (type == ColladiableType::Hit)
			{
				return ColladiableType::HitInverse;
			}
		}
		return type;
	}

	void Block::Update(float timeDelta)
	{
		
		
	}

	bool Block::IsBroken()
	{ 
		return hitCount <= 0;
	}

	int Block::GetPoints() const
	{
		return points;
	}

	bool Block::ShouldBreakImmediately() const
	{
		// Check if FragileBlocks bonus is active
		return FragileBlocksBonus::IsActive(),
			FragileBlocksBonus::IsActive();
	}
	
	void SmoothDestroyableBlock::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{
		Super::OnHit(type, collidableWhith);
		if(hitCount <= 0)
		{
		StartTimer(SETTINGS.BREAK_DELAY);
		}	
	}
	
	SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position, const sf::Color& color, int points)
		: Block(position, color, points)
	{
		
	}

	ColladiableType SmoothDestroyableBlock::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
	{
		if (isTimerStarted_ )
		{
			return ColladiableType::None; // Return a value when the timer is started
		}
		return Super::GetCollision(collidableObject); // Return the result of the base class method otherwis
	}

	void SmoothDestroyableBlock::Update(float timeDelta)
	{
		Super::Update(timeDelta);
		UpdateTimer(timeDelta);
	}

	bool SmoothDestroyableBlock::IsBroken()
	{
		return Super::IsBroken() && !isTimerStarted_;
	}	

	void SmoothDestroyableBlock::EachTickAction(float deltaTime)
	{
		color.a = 255 * currentTime_ / destroyTime_;
		sprite.setColor(color);
	}

	void SmoothDestroyableBlock::FinalAction()
	{
		--hitCount;
		if (IsBroken())
		{
			Emit();
		}
	}


	UnbreackableBlock::UnbreackableBlock(const sf::Vector2f& position, int points)
		: SmoothDestroyableBlock(position, sf::Color::Color(105, 105, 105), points)
	{
		hitDamage = 0;			

	}
	void UnbreackableBlock::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{
		if (FragileBlocksBonus::IsActive())
		{
			// Call the base class's OnHit function to handle the hit
			Block::OnHit(type, collidableWhith);
			Block::ShouldBreakImmediately();

			// If the block is hit and the hit count is 0, start the break delay timer
			if (hitCount <= 0)
			{
				StartTimer(SETTINGS.BREAK_DELAY); // Assuming StartTimer is a valid function
			}
		}
		else
		{
			// If the bonus is not active, do nothing (i.e., behave like an unbreakable block)
			return;
		}
		
	}	
	
}



