#include "BlockFactory.h"
#include"Block.h"
#include"GlassBlock.h"
#include"ThreeHitBlock.h"

namespace ArkanoidGame
{
	
	int BlockFactory::GetCReatedBreackableBlockCount()
	{
		return  createdBreackableBlockCount;
	}

	void BlockFactory::ClearCounter()
	{
		createdBreackableBlockCount = 0;
	}

	std::shared_ptr<Block> SimpleBlockFactory::CreateBlock(const sf::Vector2f& position)
	{
		++createdBreackableBlockCount;	
		
		return std::make_shared<SmoothDestroyableBlock>(position, sf::Color::Green, 10);
	}


	std::shared_ptr<Block> ThreeHitBlockFactory::CreateBlock(const sf::Vector2f& position)
	{
		++createdBreackableBlockCount;
		return std::make_shared<ThreeHitBlock>(position,30);		
		
	}

	std::shared_ptr<Block> UnbreackableBlockFactory::CreateBlock(const sf::Vector2f& position)
	{	
		return std::make_shared<UnbreackableBlock>(position,0);
	}

	std::shared_ptr<Block> GlassBlockFactory::CreateBlock(const sf::Vector2f& position)
	{
		++createdBreackableBlockCount;
		return std::make_shared<GlassBlock>(position,1);
	}

}


