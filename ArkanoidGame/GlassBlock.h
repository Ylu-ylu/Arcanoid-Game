#pragma once
#include <SFML/Graphics.hpp>
#include "Block.h"
#include "Colladiable.h"


namespace ArkanoidGame
{
	class GlassBlock :public SmoothDestroyableBlock
	{
		using Super = SmoothDestroyableBlock;
	public:
		GlassBlock(const sf::Vector2f& position, int points=0);

	private:

		void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;

		virtual ColladiableType GetCollision(const std::shared_ptr<Colladiable> collidable) const override;
	};

	
}