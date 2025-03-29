#include"ThreeHitBlock.h"
#include"GameSettings.h"

namespace ArkanoidGame
{
	ThreeHitBlock::ThreeHitBlock(const sf::Vector2f& position)
		:SmoothDestroyableBlock(position, sf::Color::Cyan)
	{
		hitCount = 3;
	}

	void ThreeHitBlock::OnHit()
	{
		Super::OnHit();	
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
	
