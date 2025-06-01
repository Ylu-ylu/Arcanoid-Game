#include"ThreeHitBlock.h"
#include"GameSettings.h"
#include "Bonus.h"

namespace ArkanoidGame
{
	ThreeHitBlock::ThreeHitBlock(const sf::Vector2f& position, int points)
		:SmoothDestroyableBlock(position, sf::Color::Cyan, points)
	{
		hitCount = 3;
	}

	bool ThreeHitBlock::ShouldBreakImmediately() const
	{
		// Even ThreeHitBlock breaks in one hit if FragileBlocks is active
		if (FragileBlocksBonus::IsActive())
			return true;

		// Otherwise, use normal logic
		return hitCount <= 0;
	}

	void ThreeHitBlock::OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith)
	{
		Super::OnHit(type, collidableWhith);	
		StageChange();
			
	}

	void ThreeHitBlock::StageChange()
	{
		if (hitCount == 2)
			sprite.setColor(sf::Color::Red);
		if (hitCount < 2) {
			sprite.setColor(sf::Color::Green);
			color = sf::Color::Green;
		}
	}
}
	
