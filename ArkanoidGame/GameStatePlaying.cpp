#include "GameStatePlaying.h"
#include "Application.h"
#include "GameSettings.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>
#include <sstream>
#include "Platform.h"
#include "Ball.h"
#include <memory>
#include <algorithm>
#include <vector>
#include "GameObject.h"
#include "Block.h"
#include "ThreeHitBlock.h"
#include <iostream>
#include "randomizer.h"
#include <random>
#include "Bonus.h"


namespace ArkanoidGame
{
	void GameStatePlayingData::Init()
    {
		BonusManager::Instance().Clear();
		
		// Init game resources (terminate if error)
		assert(font.loadFromFile(SETTINGS.FONTS_PATH + "SF Distant Galaxy Italic.ttf"));
		assert(gameOverSoundBuffer.loadFromFile(SETTINGS.SOUNDS_PATH + "Death.wav"));
		assert(eatAppleSoundBuffer.loadFromFile(SETTINGS.SOUNDS_PATH + "AppleEat.wav"));
		
		//Heart texture
		assert(heartTexture.loadFromFile(SETTINGS.TEXTURES_PATH + "heart.png"));

		for (int i = 0; i < SETTINGS.PLAYER_LIVES; i++)
		{
			sf::Sprite sprite;
			InitSprite(sprite, 63.f, 50.f, heartTexture);
			sprite.setScale(35.f / heartTexture.getSize().x, 30.f / heartTexture.getSize().y);
			sprite.setPosition(683.f + i * 40.f, 27.f);
			heartSprites.push_back(std::move(sprite));
		}

		//factories Init
		factories.emplace(BlockType::Simple, std::make_unique<SimpleBlockFactory>());
		factories.emplace(BlockType::Unbreackable, std::make_unique<UnbreackableBlockFactory>());
		factories.emplace(BlockType::ThreeHit, std::make_unique<ThreeHitBlockFactory>());
		factories.emplace(BlockType::Glass, std::make_unique<GlassBlockFactory>());
		
		// Init background		

		assert(backgroundTexture.loadFromFile(SETTINGS.TEXTURES_PATH + "Play_background.png"));
		backgroundSprite.setTexture(backgroundTexture);
		backgroundSprite.setColor(sf::Color(255, 255, 255, 168)); // Set alpha to 50% (128/255)
		backgroundSprite.setPosition(0, 0); // Set position to top-left corner of the screen
		backgroundSprite.setScale((float)SETTINGS.SCREEN_WIDTH / backgroundTexture.getSize().x, (float)SETTINGS.SCREEN_HEGHT / backgroundTexture.getSize().y); // Scale to cover the entire screen

		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Yellow);

		inputHintText.setFont(font);
		inputHintText.setCharacterSize(20);
		inputHintText.setFillColor(sf::Color::White);
		inputHintText.setString("Use arrow keys to move, ESC to pause");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.29f, -0.15f }));

		// Create platform and ball objects and add them to gameObjects vector
		auto platform = std::make_shared<Platform>(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEGHT - SETTINGS.PLATFORM_HEIGHT / 2.f));
		auto ball = std::make_shared<Ball>(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEGHT - SETTINGS.PLATFORM_HEIGHT - (SETTINGS.BALL_SIZE) / 2.f));
						
		ball->SetSelf(ball);
		
		ball->AddObserver(std::weak_ptr<IObserver>(shared_from_this()));

		gameObjects.emplace_back(platform);
		gameObjects.emplace_back(ball);

		CreateBlocks();

		// Init sounds
		gameOverSound.setBuffer(gameOverSoundBuffer);
		eatAppleSound.setBuffer(eatAppleSoundBuffer);
    }

	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PauseGame();
			}
		}
	}

	void GameStatePlayingData::Update(float timeDelta)
	{			
		static auto updateFunctor = [timeDelta](auto obj) { obj->Update(timeDelta); };

		std::for_each(gameObjects.begin(), gameObjects.end(), updateFunctor);
		std::for_each(blocks.begin(), blocks.end(), updateFunctor);
		
		std::shared_ptr<Platform> platformObj = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
		std::shared_ptr<Ball> ballObj = std::dynamic_pointer_cast<Ball>(gameObjects[1]);

		//Update Bonus
		if (platformObj && ballObj)
		{
			BonusManager::Instance().UpdateBonuses(timeDelta, ballObj, platformObj);
		}		
		
		auto isCollision = platformObj->CheckCollision(std::shared_ptr<Colladiable>(ballObj));	

		auto isCollisionWithSides = platformObj->CheckCollisionWithWindowSides();
		Game& game = Application::Instance().GetGame();
		
		if (collisionCooldown > 0.0f) 
		{
			collisionCooldown -= timeDelta;
			if (collisionCooldown <= 0.0f) {
				collisionProcessed = false;  // Reset only when cooldown expires
				collisionCooldown = 0.0f;    // Prevent negative values
			}
		}
		else {
			collisionProcessed = false;      // Ensure reset when no cooldown
		}

		//Check collision with blocks
		bool hasBrokeOneBlock = false;
	
		blocks.erase(
			std::remove_if(blocks.begin(), blocks.end(),
				[ballObj, &hasBrokeOneBlock, this, &game](auto block)
				{					
					
					hasBrokeOneBlock = true;
					if (block == nullptr) 
					{
						return true; // or return false, depending on your logic
					}

					if (block->CheckCollision(ballObj))
					{
						if (!block->IsBroken()) // Update score for every hit
						{
							score += block->GetPoints(); // Update score according to block type
							game.SetFinalScore(score); // Update final score
							if (Application::Instance().GetGame().IsSoundEnabled()) {
								eatAppleSound.play();
							}
						}					
						
						BonusManager::Instance().TrySpawnBonus(block->GetPosition());
					}

					return block->IsBroken();
					
				}),
			blocks.end()
		);
		
		
	
		//Check game win
		const bool isGameWin = blocks.empty();
		if (isGameWin)
		{			
			game.LoadNextLevel();
		}
		else
		{
			//Check game over
			const bool isGameOver = !isCollision && ballObj->GetPosition().y > platformObj->GetSpriteRect().top; ;

			if (isGameOver)
			{
				if (Application::Instance().GetGame().IsSoundEnabled())
				{
					gameOverSound.play();
				}		

				game.LooseGame();
			}
		}
		
		if (isCollisionWithSides)
		{
			const auto platformPos = platformObj->GetPosition();
			const auto platformRect = platformObj->GetSpriteRect();
		}
	
		// Update score
		score += points;
		scoreText.setString("Score: " + std::to_string(score));
	}
	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		// Draw background
		window.draw(sf::Sprite(backgroundSprite));

		static auto drawFunc = [&window](auto obj) { obj->Draw(window); };
		// Draw game objects
		std::for_each(gameObjects.begin(), gameObjects.end(), drawFunc);

		//Draw blocks
		std::for_each(blocks.begin(), blocks.end(), drawFunc);

		// Draw bonuses
		BonusManager::Instance().DrawBonuses(window);

		//Draw Score
		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);

		DrawSprites(heartSprites.begin(), heartSprites.end(), window);
	}

	void GameStatePlayingData::LoadNextLevel()
	{
		if (currentLevel >= levelloader.GetLevelCount() - 1) {
			Game& game = Application::Instance().GetGame();

			game.WinGame();
		}
		else
		{
			std::shared_ptr <Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
			std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
			platform->restart();
			ball->restart();

			blocks.clear();
			++currentLevel;
			CreateBlocks();
		}
		// Clear any active bonuses when loading a new level
		BonusManager::Instance().Clear();

	}

	void GameStatePlayingData:: Notify(std::shared_ptr<IObservable> observable, int points)
	{		
		if (auto block = std::dynamic_pointer_cast<Block>(observable))
		{
			--breakableBlockCount;
			Game& game = Application::Instance().GetGame();
			
			if (breakableBlockCount == 0) 
			{
				game.LoadNextLevel();
			}
		}
		else if (auto ball = std::dynamic_pointer_cast<Ball>(observable))
		{
			
			
			if (ball->GetPosition().y > gameObjects.front()->GetSpriteRect().top)
			{
				
				--ball->playersLives;
				if ( ball->playersLives<=3 && heartSprites.size() >0)
				{
					if (!heartSprites.empty()) // Add this check
					{
						heartSprites.pop_back();
					}
					if (Application::Instance().GetGame().IsSoundEnabled()) {
						gameOverSound.play();
					}					
					ball->restart();

				}
				else if (ball->playersLives == 0)
				{
					if (heartSprites.size() == 1)
					{
						heartSprites.pop_back();
					}
								
					if (Application::Instance().GetGame().IsSoundEnabled()) {
						gameOverSound.play();
					}
					Application::Instance().GetGame().LooseGame();
				}							
				
			}
		}

	}	

	void GameStatePlayingData::CreateBlocks()
	{		
		
		for (const auto& pair : factories)
		{
			pair.second->ClearCounter();
		}
		auto& settings = SETTINGS;

		auto self = weak_from_this();
		auto level = levelloader.GetLevel(currentLevel);		

		for (auto pairPosBlockType:level.m_blocks)
		{
			auto blockType = pairPosBlockType.second;
		
			sf::Vector2i pos= pairPosBlockType.first;

			sf::Vector2f position
			{
				(float)(settings.BLOCK_SHIFT + settings.BLOCK_WIDTH / 2.f + pos.x * (settings.BLOCK_WIDTH + settings.BLOCK_SHIFT)),
				 (float)((float)pos.y * (settings.BLOCK_HEIGHT + settings.BLOCK_SHIFT)-50.f)
			};

			blocks.emplace_back(factories.at(blockType)->CreateBlock(position));
			blocks.back()->AddObserver(self);
		}

		int breakableCount = 0;
		for (const auto& pair : factories)
		{
			
			breakableBlockCount += pair.second->GetCReatedBreackableBlockCount();
		}
	
	}

	void GameStatePlayingData::ChooseRandomModifierToInit(BonusType bonusType, const sf::Vector2f& position)
	{
		// Choose random bonus type
		int randomModifier = random<int>(1, 2); // 1 or 2
		
		switch (randomModifier)
		{
		case 1:
			bonusType = BonusType::Fireball;
			break;
		case 2:
			bonusType = BonusType::FragileBlocks;
			break;
		default:
			bonusType = BonusType::Fireball;
			break;
		}

		// Create and add the bonus using BonusManager
		BonusManager::Instance().SpawnSpecificBonus(bonusType, position);

	}	

} 


