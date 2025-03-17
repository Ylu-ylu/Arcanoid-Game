#pragma once
#include "SFML/Graphics.hpp"
#include "Platform.h"
#include "GameObject.h"


namespace ArkanoidGame
{
	class Ball:public GameObject
	{
	public:
		Ball(const sf::Vector2f& position);
		~Ball() = default;

		void Init();
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);
		bool CheckCollisionWhithPlatform(const Platform& platform);
		const sf::Vector2f& GetPosition() const { return sprite.getPosition(); }
		void InvertDirectionX();
		void InvertDirectionY();
		void ChangeAngle(float x);
		void restart() override;		
		float GetSize() const;
	private:
				
		sf::Vector2f direction;
		sf::Sprite sprite;
		sf::Texture texture;
		float lastAngle = 90.f;
	};
}


