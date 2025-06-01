#pragma once
#include <string>

namespace ArkanoidGame
{	
		
	class GameWorld
	{
	private:
		GameWorld() = default;
	public:
		static GameWorld& Instance()
		{
			static GameWorld gameWorld;
			return gameWorld;
		}

		// Resources path
		const std::string RESOURCES_PATH = "Resources/";
		const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
		const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
		const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";
		const std::string LEVELS_CONFIG_PATH = RESOURCES_PATH + "levels.config";


		const unsigned int SCREEN_WIDTH = 800;
		const unsigned int SCREEN_HEGHT = 600;
		const float TIME_PER_FRAME = 1.f / 60.f; // 60 fps

		const unsigned int BALL_SIZE = 20;
		const unsigned int BALL_SPEED = 500;


		const unsigned int PLATFORM_WIDTH = 80;
		const unsigned int PLATFORM_HEIGHT = 20;
		const float PLATFORM_SPEED = 390.f;

		const int MAX_RECORDS_TABLE_SIZE = 5;
		
		const char*  PLAYER_NAME;		
		const std::string GAME_NAME = "Arcanoid";

		const unsigned int BLOCKS_COUNT_ROWS = 4;
		const unsigned int BLOCKS_COUNT_IN_ROW = 15;
		const unsigned int BLOCK_SHIFT = 1.5f;
		const unsigned int BLOCK_WIDTH = (SCREEN_WIDTH - (BLOCKS_COUNT_IN_ROW + 1) * BLOCK_SHIFT) / (BLOCKS_COUNT_IN_ROW);
		const unsigned int BLOCK_HEIGHT = 25;
		const unsigned int BREAK_DELAY = 1.0f;
		
		//Player lives
		const int PLAYER_LIVES = 3;

		//Bonus	
		const std::string BONUS_TEXTURES_PATH = RESOURCES_PATH + "Textures/";
		const float BONUS_SPEED = 150.f;
		const float BONUS_SPAWN_CHANCE = 0.1f; // 10% chance
		const float BONUS_DURATION = 5.0f;  // Added duration
		const unsigned int BONUS_SIZE = 15;

		//FireBall
		const float FIREBALL_SPEED_MULTIPLIER = 1.5f;
	};

}
#define SETTINGS ArkanoidGame::GameWorld::Instance()
