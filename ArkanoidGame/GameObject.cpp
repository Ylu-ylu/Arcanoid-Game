#include "GameObject.h"
#include "Sprite.h"
#include <assert.h>
#include "GameSettings.h"


namespace ArkanoidGame
{
	GameObject::GameObject(GameObjectType type,const std::string& texturePath, const sf::Vector2f& position, float width, float height) :
		startPosition(position)
	{
		assert(texture.loadFromFile(texturePath));

			switch (type) 
			{
			case GameObjectType::Ball:
				InitSprite(sprite, BALL_SIZE, BALL_SIZE, texture);
				break;
			case GameObjectType::Platform:
				InitSprite(sprite, PLATFORM_WIDTH, PLATFORM_HEIGHT, texture);
				break;	
			case GameObjectType::Block:
				InitSprite(sprite, BLOCK_WIDTH, BLOCK_HEIGHT, texture);
				break;
			default:
				// Default dimensions for other object types
				InitSprite(sprite,width, height, texture);
			}
		
		sprite.setPosition(position);
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