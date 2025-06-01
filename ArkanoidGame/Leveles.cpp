#include "Leveles.h"
#include <assert.h>
#include <fstream>
#include <string>
#include "GameSettings.h"






namespace ArkanoidGame
{
	Leveles& LevelLoader::GetLevel(int i)
	{
		return levels.at(i);
	}

	int LevelLoader::GetLevelCount()
	{
		return levels.size();
	}

	void LevelLoader::LoadLevelFromFile()
	{
		std::string filepath = SETTINGS.LEVELS_CONFIG_PATH;
		std::string line;
		std::ifstream file(filepath);
		int y = 0;
		while (getline(file, line))
		{
			if (line.rfind("level", 0) == 0)
			{
				auto level = std::stoi(line.substr(6, line.size() - 6));
				levels.emplace_back(Leveles());
		          y = 0;
			}
			else
			{
				int x = 0;
				for (char c : line)
				{
					if (c != ' ')
					{
						levels.back().m_blocks.emplace_back(std::make_pair(sf::Vector2i(x, y), CharToBlockType(c)));
					}
					++x;
				}
			}
			++y;
		}
		file.close();
	}

	BlockType LevelLoader::CharToBlockType(char symbol)
	{
		BlockType block;
		switch (symbol)
		{
		case '0':
			block = BlockType::Simple;
			break;
		case '1':
			block = BlockType::Unbreackable;
			break;
		case '2':
			block = BlockType::ThreeHit;
			break;
		case '3':
			block = BlockType::Glass;
			break;
		default:
			assert(false);
			break;
		}		
		return block;
	}

}