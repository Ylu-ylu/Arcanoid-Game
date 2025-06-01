#pragma once
#include "Block.h"
#include "ThreeHitBlock.h"
#include "GlassBlock.h"

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace ArkanoidGame
{
	enum class BlockType
	{		
		Simple,
		Unbreackable,
		ThreeHit,
		Glass
	};

	struct Leveles 
	{
		std::vector<std::pair<sf::Vector2i, BlockType>> m_blocks;
	};

	class LevelLoader
	{
	public:
		LevelLoader() { LoadLevelFromFile(); }
		Leveles& GetLevel(int i);
		~LevelLoader() = default;
		int GetLevelCount();

	private:
		void LoadLevelFromFile();
		std::vector<Leveles> levels;

		static BlockType CharToBlockType(char symbol);
	};

}