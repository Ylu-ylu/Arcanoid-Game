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

		platform->Init();
		ball->Init();
		score = 0;

		gameObjects.emplace_back(platform);
		gameObjects.emplace_back(ball);

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
				
		auto platformObj = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
		auto ballObj = std::dynamic_pointer_cast<Ball>(gameObjects[1]);

		ballObj->Update(timeDelta);
		platformObj->Update(timeDelta);


		auto isCollision = platformObj->CheckCollisionWhithBall(*ballObj);
		auto isCollisionWithSides = platformObj->CheckCollisionWithWindowSides();
		bool needInverseDirX = false;
		bool needInverseDirY = false;

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
			GetBallInverse(ballPos,  needInverseDirX, needInverseDirY);
		}
		if(isCollisionWithSides)
		{
			const auto platformPos = platformObj->GetPosition();
			const auto platformRect = platformObj->GetRect();
		}
		if (needInverseDirX) 
		{
			ballObj->InvertDirectionX();
		}
		if (needInverseDirY) 
		{
			ballObj->InvertDirectionY();
		}
		if (ballObj->GetPosition().y > gameObjects.front()->GetRect().top)
		{
			gameOverSound.play();
			Application::Instance().GetGame().SetFinalScore(score);
			Application::Instance().GetGame().LooseGame();
		}
		if (ballObj->GetPosition().y + ballObj->GetSize() / 2 > SCREEN_HEGHT)
		{
			// Ball went below the bottom edge - game over
			gameOverSound.play();
		
			Application::Instance().GetGame().SetFinalScore(score);
			Application::Instance().GetGame().LooseGame();
			return; // Stop further processing
		}
		// Update score
		scoreText.setString("Score: " + std::to_string(score));	
	}
	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		// Draw background
		window.draw(background);
		auto ballObj = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
		ballObj->Draw(window);
		auto platformObj = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
		platformObj->Draw(window);		
		
		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);
	}

	void GameStatePlayingData::GetBallInverse(const sf::Vector2f& ballPos,  bool& needInverseDirX, bool& needInverseDirY) 
	{
		if (ballPos.y > SCREEN_HEGHT)
		{
			needInverseDirY = true;
		}
		if (ballPos.x < 0)
		{
			needInverseDirX = true;
		}
		if (ballPos.x > SCREEN_WIDTH)
		{
			needInverseDirX = true;
		}
	}

} 


