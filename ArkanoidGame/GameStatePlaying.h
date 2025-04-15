#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameStateData.h"
#include "GlassBlock.h"


namespace ArkanoidGame
{
	class Game;
	class Platform;
	class Ball;
	class GameObject;
	class Block;
	
	class GameStatePlayingData : public GameStateData, public std::enable_shared_from_this<GameStatePlayingData>
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

	private:
		void CreateBlocks();		
		
		 bool collisionProcessed = false;  // Flag to prevent multiple collision counts
         float collisionCooldown = 0.0f;   // Cooldown timer for collisions

		// Resources
		sf::Font font;
		sf::SoundBuffer eatAppleSoundBuffer;
		sf::SoundBuffer gameOverSoundBuffer;

		// Game data
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		std::vector<std::shared_ptr<Block>> blocks;

		//Score
		int score = 0; // Track player's score

		// UI data
		sf::Text scoreText;
		sf::Text inputHintText;
		sf::RectangleShape background;

		// Sounds
		sf::Sound gameOverSound;
		sf::Sound eatAppleSound;
	};
}
