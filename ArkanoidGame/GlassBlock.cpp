#include "GlassBlock.h"


namespace ArkanoidGame
{
	GlassBlock::GlassBlock(const sf::Vector2f& position):
		Super(position, sf::Color{ 100, 149, 237, 120})
	{

	}
	void GlassBlock::OnHit(ColladiableType type,  std::shared_ptr<Colladiable> collidableWhith)
	{
		Super::OnHit(type, collidableWhith);
		
	}

	ColladiableType GlassBlock::GetCollision(const std::shared_ptr<Colladiable> collidable) const
	{
		ColladiableType type = Super::GetCollision(collidable);
		if (type == ColladiableType::HitInverse)
		{
			return ColladiableType::Overlap;
		}
		return type;
	}
	
}


