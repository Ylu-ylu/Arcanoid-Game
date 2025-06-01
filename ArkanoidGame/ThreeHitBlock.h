 #pragma once
#include <SFML/Graphics.hpp>
#include "Block.h"
#include "Colladiable.h"


namespace ArkanoidGame
{
	class ThreeHitBlock : public SmoothDestroyableBlock
	{
		using Super = SmoothDestroyableBlock;
	public:
		ThreeHitBlock(const sf::Vector2f & position, int points=0);

		bool ShouldBreakImmediately() const override;
	private:

		void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;

		void StageChange();
	};
}
