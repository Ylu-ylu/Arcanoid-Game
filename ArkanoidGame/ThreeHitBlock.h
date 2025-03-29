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
		ThreeHitBlock(const sf::Vector2f & position);

	private:

		void OnHit() override;

		void StageChange();
	};
}
