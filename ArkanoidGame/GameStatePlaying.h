#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameStateData.h"
#include "GlassBlock.h"
#include "BlockFactory.h"
#include <unordered_map>
#include "Leveles.h"
#include "IObserver.h"


namespace ArkanoidGame
{
	class Game;
	class Platform;
	class Ball;
	class GameObject;
	class Block;
	enum class BonusType;
	
	class GameStatePlayingData : public GameStateData,
		public  IObserver,
		public  std::enable_shared_from_this<GameStatePlayingData>
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;
		void LoadNextLevel();

		void Notify(std::shared_ptr<IObservable> observable, int points);	
		

	private:
		void CreateBlocks();	

		void ChooseRandomModifierToInit(BonusType bonusType, const sf::Vector2f& position);
		
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
		int points = 0;
		// UI data
		sf::Text scoreText;
		sf::Text inputHintText;
		sf::RectangleShape background;

		// Sounds
		sf::Sound gameOverSound;
		sf::Sound eatAppleSound;

		//Blocks Creator
		std::unordered_map<BlockType, std::shared_ptr<BlockFactory>> factories;
		int breakableBlockCount = 0;
		int unbreakableBlockCount = 0;

		//Leveles
		LevelLoader levelloader;
		int currentLevel = 0;

		//Heart
		sf::Texture heartTexture;
		std::vector <sf::Sprite> heartSprites;
		sf::Texture backgroundTexture;
		sf::Sprite backgroundSprite;

	};
}
