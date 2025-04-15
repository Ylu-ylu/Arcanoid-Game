#include"ThreeHitBlock.h"
#include"GameSettings.h"

namespace ArkanoidGame
{
	ThreeHitBlock::ThreeHitBlock(const sf::Vector2f& position)
		:SmoothDestroyableBlock(position, sf::Color::Cyan)
	{
		hitCount = 3;
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
	
