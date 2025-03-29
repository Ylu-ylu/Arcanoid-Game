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


namespace ArkanoidGame
{
    void GameStatePlayingData::Init()
    {
		// Init game resources (terminate if error)
		assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));
		assert(gameOverSoundBuffer.loadFromFile(SOUNDS_PATH + "Death.wav"));
		assert(eatAppleSoundBuffer.loadFromFile(SOUNDS_PATH + "AppleEat.wav"));

		// Init background
		background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEGHT));
		background.setPosition(0.f, 0.f);
		background.setFillColor(sf::Color(0, 0, 0));

		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Yellow);

		inputHintText.setFont(font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::White);
		inputHintText.setString("Use arrow keys to move, ESC to pause");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.5f, 0.f }));

		// Create platform and ball objects and add them to gameObjects vector
		auto platform = std::make_shared<Platform>(sf::Vector2f(SCREEN_WIDTH / 2.f, SCREEN_HEGHT - PLATFORM_HEIGHT / 2.f));
		auto ball = std::make_shared<Ball>(sf::Vector2f(SCREEN_WIDTH / 2.f, SCREEN_HEGHT - PLATFORM_HEIGHT - BALL_SIZE / 2.f));
		
		score = 0;

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

		// Check if gameObjects has at least 2 elements
		if (gameObjects.size() < 2) 
		{
			std::cerr << "Error: Not enough game objects" << std::endl;
			return;
		}

		std::shared_ptr<Platform> platformObj = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
		std::shared_ptr<Ball> ballObj = std::dynamic_pointer_cast<Ball>(gameObjects[1]);

		if (!platformObj || !ballObj) {
			// Handle error: one or both objects are not valid
			if (!platformObj) {
				std::cerr << "Error: platformObj is null" << std::endl;
			}
			if (!ballObj) {
				std::cerr << "Error: ballObj is null" << std::endl;
			}
			return; // Exit the function early to avoid further issues
		}



		auto isCollision = platformObj->CheckCollision(std::shared_ptr<Colladiable>(ballObj));
		auto isCollisionWithSides = platformObj->CheckCollisionWithWindowSides();
		bool needInverseDirX = false;
		bool needInverseDirY = false;

		
		//Check collision with blocks
		bool hasBrokeOneBlock = false;
		//remove-erase idiom
		blocks.erase(
			std::remove_if(blocks.begin(), blocks.end(),
				[ballObj, &hasBrokeOneBlock, &needInverseDirX, &needInverseDirY, this](auto block) 
				{
					if ((!hasBrokeOneBlock) && block->CheckCollision(ballObj))
					{
						hasBrokeOneBlock = true;
						const auto ballPos = ballObj->GetPosition();
						const auto blockRect = block->GetRect();

						GetBallInverse(ballPos, blockRect, needInverseDirX, needInverseDirY);
					}
					return block->IsBroken();
				}),
			blocks.end()
		);
		//Invert direction
		if (needInverseDirX)
		{
			ballObj->InvertDirectionX();
		}
		if (needInverseDirY)
		{
			ballObj->InvertDirectionY();
		}

		Game& game = Application::Instance().GetGame();
		//Check game win
		const bool isGameWin = blocks.empty();
		if (isGameWin)
		{
			game.PushState(GameStateType::GameWin, false);
		}
		else
		{
			//Check game over
			const bool isGameOver = !isCollision && ballObj->GetPosition().y > platformObj->GetSpriteRect().top; ;

			if (isGameOver)
			{
				gameOverSound.play();
				game.PushState(GameStateType::GameOver, false);
			}

		}

		///////////////////////////
		if (collisionCooldown > 0)
		{
			collisionCooldown -= timeDelta;
			collisionProcessed = true;
		}
		else
		{
			collisionProcessed = false;
		}

		if (isCollision && !collisionProcessed)
		{
			eatAppleSound.play();
			score += 1;
			Application::Instance().GetGame().SetFinalScore(score);
			collisionCooldown = 0.2f;  // 200ms cooldown
			const auto ballPos = ballObj->GetPosition();
			const auto platformRect = platformObj->GetSpriteRect();
			GetBallInverse(ballPos, platformRect, needInverseDirX, needInverseDirY);
		}
		if (isCollisionWithSides)
		{
			const auto platformPos = platformObj->GetPosition();
			const auto platformRect = platformObj->GetSpriteRect();
		}
	
		// Update score
		scoreText.setString("Score: " + std::to_string(score));
	}
	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		// Draw background
		window.draw(background);
		
		static auto drawFunc = [&window](auto obj) { obj->Draw(window); };
		// Draw game objects
		std::for_each(gameObjects.begin(), gameObjects.end(), drawFunc);

		//Draw blocks
		std::for_each(blocks.begin(), blocks.end(), drawFunc);

		//Draw Score
		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);
	}

	void GameStatePlayingData::CreateBlocks()
	{		
		// Random number generation setup
		std::random_device rd;
		std::mt19937 gen(rd());
		std::discrete_distribution<> dis({ 75, 10, 15 }); // Random number between 0 and 2

		for (int row = 0; row < BLOCKS_COUNT_ROWS; ++row)
		{
			for (int col = 0; col < BLOCKS_COUNT_IN_ROW; ++col)
			{
				int blockType = dis(gen); // Randomly select block type

				std::shared_ptr<Block> block;
				switch (blockType)
				{
				case 0:
					//75%
					block = std::make_shared<SmoothDestroyableBlock>
						(
						sf::Vector2f({ BLOCK_SHIFT + BLOCK_WIDTH / 2.f + col * (BLOCK_WIDTH + BLOCK_SHIFT),
						100.f + row * (BLOCK_HEIGHT + BLOCK_SHIFT) }));
					break;
				case 1:
					//10%
					block = std::make_shared<UnbreackableBlock>
						(
						sf::Vector2f({ BLOCK_SHIFT + BLOCK_WIDTH / 2.f + col * (BLOCK_WIDTH + BLOCK_SHIFT),
						100.f + row * (BLOCK_HEIGHT + BLOCK_SHIFT) }));
					break;
				case 2:
					//15%
					block = std::make_shared<ThreeHitBlock>						
						(
						sf::Vector2f({ BLOCK_SHIFT + BLOCK_WIDTH / 2.f + col * (BLOCK_WIDTH + BLOCK_SHIFT),
						100.f + row * (BLOCK_HEIGHT + BLOCK_SHIFT) }));
					break;
				}
				blocks.emplace_back(block);
			}
		}
	}

	void GameStatePlayingData::GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseDirX, bool& needInverseDirY)
	{
		if (ballPos.y > blockRect.top + blockRect.height)
		{
			needInverseDirY = true;
		}
		if (ballPos.x < blockRect.left)
		{
			needInverseDirX = true;
		}
		if (ballPos.x > blockRect.left + blockRect.width)
		{
			needInverseDirX = true;
		}
	}

} 


