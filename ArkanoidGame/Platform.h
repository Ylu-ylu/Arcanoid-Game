#pragma once  
#include "SFML/Graphics.hpp"  
#include "GameObject.h"

namespace ArkanoidGame  
{  
	class Ball;
	
class Platform: public GameObject
{  
public:  
	Platform(const sf::Vector2f& position);  

	void Init();  
	void Update(float timeDelta);  
	void Draw(sf::RenderWindow& window);  
	void Move(float speed);  

	bool CheckCollisionWhithBall(Ball & ball) const;  
	sf::FloatRect GetRect() const { return sprite.getGlobalBounds(); }  
	const sf::Vector2f& GetPosition() const { return sprite.getPosition(); } 

	bool CheckCollisionWithWindowSides() const;  
	
private:  
	sf::Texture texture;  
	sf::Sprite sprite;  
};  
}