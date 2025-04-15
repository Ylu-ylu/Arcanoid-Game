#pragma once
#include "SFML/Graphics.hpp"
#include "Platform.h"
#include "GameObject.h"
#include "Colladiable.h"
#include "Application.h"


namespace ArkanoidGame
{
	class Ball:public GameObject, public Colladiable
	{		
	public:
		Ball(const sf::Vector2f& position);
		
		~Ball() = default;
		void Update(float timeDelta);

		sf::FloatRect GetColladiableRect() const override { return GetRect();};

		void InvertDirectionX();
		void InvertDirectionY();		
		
		virtual sf::FloatRect GetRect() const { return GetSpriteRect();}		
		
		void ChangeAngle(float x);
		void restart() override;		
		float GetSize() const;
	
	protected:
		virtual  void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;
	private:		
		void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseDirX, bool& needInverseDirY);

		sf::Vector2f direction;		
		float lastAngle = 90.f;
		ColladiableType type;	

		bool BallInverse = false;
	};
}


