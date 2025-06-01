#pragma once
#include "SFML/Graphics.hpp"
#include "Platform.h"
#include "GameObject.h"
#include "Colladiable.h"
#include "Application.h"
#include "IObserver.h"


namespace ArkanoidGame
{
	class Ball :public GameObject, public Colladiable, public IObservable
	{	
		
	public:
		Ball(const sf::Vector2f& position);
		
		~Ball() = default;
		void Update(float timeDelta);

		sf::FloatRect GetColladiableRect() const override { return GetRect();};

		void InvertDirectionX();
		void InvertDirectionY();		
		
		virtual sf::FloatRect GetRect() const { return GetSpriteRect();}		
		
		ColladiableType GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;

		void ChangeAngle(float x);
		void restart() override;		
		float GetSize() const;

		int playersLives = SETTINGS.PLAYER_LIVES;


		//For FireBall
		sf::Sprite& GetSprite() { return sprite; }

		// Fireball bonus properties
		void SetFireballMode(bool mode) { fireballMode = mode; }
		bool IsFireballMode() const { return fireballMode; }

		// Speed multiplier for bonuses
		void SetSpeedMultiplier(float multiplier) { speedMultiplier = multiplier; }
		float GetSpeedMultiplier() const { return speedMultiplier; }

		// New methods for state changes
		void SetDefaultState();
		void SetFireballState();
	
	protected:
		virtual  void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;
	private:		
		void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseDirX, bool& needInverseDirY);

		sf::Vector2f direction;		
		float lastAngle = 90.f;
		ColladiableType type;	

		bool fireballMode = false;
		float speedMultiplier = 1.0f;

		// Add texture management
		 sf::Texture defaultTexture;
		 sf::Texture fireballTexture;
		
	};
}


