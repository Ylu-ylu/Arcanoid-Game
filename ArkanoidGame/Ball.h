#pragma once
#include "SFML/Graphics.hpp"
#include "Platform.h"
#include "GameObject.h"
#include "Colladiable.h"


namespace ArkanoidGame
{
	class Ball:public GameObject, public Colladiable
	{
	public:
		Ball(const sf::Vector2f& position);
		~Ball() = default;
		void Update(float timeDelta);

		void InvertDirectionX();
		void InvertDirectionY();		
		
		//bool CheckCollisionWhithPlatform(const Platform& platform);
		virtual sf::FloatRect GetRect() const { return GetSpriteRect(); }

		virtual bool GetCollision(std::shared_ptr<Colladiable> collidable) const override;
		
		void ChangeAngle(float x);

		void restart() override;		
		float GetSize() const;
	
	protected:
		virtual  void OnHit() override;
	private:		
				
		sf::Vector2f direction;		
		float lastAngle = 90.f;
	};
}


