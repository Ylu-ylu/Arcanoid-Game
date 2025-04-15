#pragma once  
#include "SFML/Graphics.hpp"  
#include "GameObject.h"
#include "Colladiable.h"


namespace ArkanoidGame
{
	class Ball;

	class Platform : public GameObject, public Colladiable
	{
	public:
		Platform(const sf::Vector2f& position);
		
		~Platform() = default;
		void Update(float timeDelta);

		virtual ColladiableType GetCollision(std::shared_ptr<Colladiable> collidable) const override;
		virtual void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override {}
		virtual bool CheckCollision(std::shared_ptr<Colladiable> collidable) override;
		virtual sf::FloatRect GetRect() const { return GetSpriteRect(); }
		sf::FloatRect GetColladiableRect() const override { return GetRect(); };

		const sf::Vector2f& GetPosition() const { return sprite.getPosition(); }

		bool CheckCollisionWithWindowSides() const;

	private:
		void Move(float speed);
	};

}