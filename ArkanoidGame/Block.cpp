#include "Block.h"
#include "Sprite.h"
#include "GameSettings.h"
#include <assert.h>

namespace
{
	// id textures
	const std::string TEXTURE_ID = "block";
}

namespace ArkanoidGame
{
	void Block::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{
		hitCount-= hitDamage;
	}
	Block::Block(const sf::Vector2f& position, const sf::Color& color) :
		GameObject(GameObjectType::Block, TEXTURES_PATH + TEXTURE_ID + ".png", position, BLOCK_WIDTH, BLOCK_HEIGHT)
	{
		sprite.setColor(color);
	}	

	ColladiableType Block::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
	{
		ColladiableType type = Colladiable::GetCollision(collidableObject);
			if (type == ColladiableType::Hit)
			{
				return ColladiableType::HitInverse;
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
	
	void SmoothDestroyableBlock::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{
		Super::OnHit(type, collidableWhith);
		if(hitCount <= 0)
		{
		StartTimer(BREAK_DELAY);
		}	

	}

	SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position, const sf::Color& color)
		: Block(position, color)
		, color(color)	
	{
		
	}

	ColladiableType SmoothDestroyableBlock::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
	{
		if (isTimerStarted_)
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


	UnbreackableBlock::UnbreackableBlock(const sf::Vector2f& position)
		: Block(position, sf::Color::Color(105, 105, 105))
	{
		hitDamage=0;
	}
	
}



