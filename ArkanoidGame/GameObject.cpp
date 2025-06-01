#include "GameObject.h"
#include "Sprite.h"
#include <assert.h>
#include "GameSettings.h"


namespace ArkanoidGame
{
	GameObject::GameObject(GameObjectType type,const std::string& texturePath, const sf::Vector2f& position, float width, float height) :
		startPosition(position),
		colladiableType(ColladiableType::Hit) // Initialize colladiableType here
	{
		assert(texture.loadFromFile(texturePath));

			switch (type) 
			{
			case GameObjectType::Ball:
				InitSprite(sprite, SETTINGS.BALL_SIZE, SETTINGS.BALL_SIZE, texture);
				
				break;
			case GameObjectType::Platform:
				InitSprite(sprite, SETTINGS.PLATFORM_WIDTH, SETTINGS.PLATFORM_HEIGHT, texture);
			
				break;	
			case GameObjectType::Block:
				InitSprite(sprite, SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT, texture);
				break;
			case GameObjectType::FireBall:
				InitSprite(sprite, SETTINGS.BONUS_SIZE, SETTINGS.BONUS_SIZE, texture);
				break;

			case GameObjectType::FragileBlocks:
					InitSprite(sprite, SETTINGS.BONUS_SIZE, SETTINGS.BONUS_SIZE, texture);
					break;
			default:
				// Default dimensions for other object types
				InitSprite(sprite,width, height, texture);
			}
		
		sprite.setPosition(position);
	}
	sf::FloatRect GameObject::GetColladiableRect() const 
	{
		return sprite.getGlobalBounds();
	}
	void GameObject::Draw(sf::RenderWindow& window)
	{
		DrawSprite(sprite, window);
	}
	
	void GameObject::restart()
	{
		sprite.setPosition(startPosition);
	}
}