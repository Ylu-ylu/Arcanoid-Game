#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Colladiable.h"
#include "Platform.h"
#include "Ball.h"
#include "GameObject.h"


namespace ArkanoidGame
{
	// Forward declarations
	class Ball;
	class Platform;

	// Enum for bonus types
	enum class BonusType
	{
		Fireball,
		FireBall_Bonus,
		FragileBlocks,
		WidePlatform,
		Default
	};

	class Bonus : public GameObject, public Colladiable
	{
	public:
		Bonus(const sf::Vector2f& position, BonusType type, const std::string& texturePath);
		~Bonus() = default;

		void Update(float timeDelta) override;
		bool IsCollected() const { return collected; }

		sf::FloatRect GetColladiableRect() const override { return GetRect(); }
		sf::FloatRect GetRect() const override { return GetSpriteRect(); }
		ColladiableType GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
	
		//Bonus
		virtual void ApplyEffect(std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform = nullptr) = 0;		
		virtual void RemoveEffect(std::shared_ptr<Ball> ball) = 0;

		//Check collision whith platform
		virtual bool CheckCollision(std::shared_ptr<Platform> platform);

		//Get Duration
		float GetDuration() const { return duration; }

		float GetSize() const;

		std::string getTextureId(BonusType type);

	protected:
		void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWith) override {}
		bool collected = false;
		float duration = SETTINGS.BONUS_DURATION;		

	};

	//Bonus FireBall
	class FireBallBonus : public Bonus
	{
	public:
		FireBallBonus(const sf::Vector2f& position);

		void ApplyEffect(std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform) override;
		void RemoveEffect(std::shared_ptr<Ball> ball) override;

	private:
		bool penetratesBlocks = true;
	};


	class FragileBlocksBonus : public Bonus
	{
	public:
		FragileBlocksBonus(const sf::Vector2f& position);

		void ApplyEffect(std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform) override;
		void RemoveEffect(std::shared_ptr<Ball> ball) override;

	private:
		static bool fragileBlocksActive;

	public:
		// Static accessor for checking if bonus is active
		static bool IsActive() { return fragileBlocksActive; }
	};


	class WidePlatformBonus : public Bonus
	{
	public:
		WidePlatformBonus(const sf::Vector2f& position);

		void ApplyEffect(std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform) override;
		void RemoveEffect(std::shared_ptr<Ball> ball) override;

	private:
		// Stored platform width multiplier
		const float widthMultiplier = 2.0f;
		// Flag to track if the bonus is active
		static bool widePlatformActive;
		// Store platform reference for effect removal
		std::weak_ptr<Platform> affectedPlatform;
	public:
		// Static accessor for checking if bonus is active
		static bool IsActive() { return widePlatformActive; }
	};

	//Create Bonus Factory
	class BonusFactory
	{
	public:
		static std::shared_ptr <Bonus> CreateBonus(BonusType type, const sf::Vector2f& position);
	};

	//Mange Bonus
	class BonusManager
	{
	public:	
       
	static BonusManager& Instance()
		{
			static BonusManager instance;
			return instance;
		};		

		void AddBonus(std::shared_ptr<Bonus> bonus);
		void UpdateBonuses(float timeDelta, std::shared_ptr<Ball> ball, std::shared_ptr <Platform> platform);
		void DrawBonuses(sf::RenderWindow& window);
		void Clear();
	
		bool TrySpawnBonus(const sf::Vector2f& position);

		void SpawnSpecificBonus(BonusType type, const sf::Vector2f& position);
	private:
		BonusManager() = default;

		std::vector<std::shared_ptr<Bonus>> activeBonuses;
		std::vector<std::pair<std::shared_ptr<Bonus>, float>> activeEffects;			
	};

}