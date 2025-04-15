#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Colladiable.h"
#include "IDelayedAction.h"

namespace ArkanoidGame
{
	class Block:public GameObject, public Colladiable
	{
	
	protected:
		virtual  void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;
		int hitCount = 1;
		int hitDamage = 1;

	public:
		Block(const sf::Vector2f& position, const sf::Color& color = sf::Color::Green);
		virtual ~Block() override= default;
		sf::FloatRect GetColladiableRect() const override { return GetRect(); };

		ColladiableType GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
		virtual void Update(float timeDelta) override;
		virtual bool IsBroken();

		virtual sf::FloatRect GetRect() const override { return GetSpriteRect(); }

	};

	class SmoothDestroyableBlock : public Block, public IDelayedAction
	{
		using Super = Block;
	protected:
		virtual  void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;
		sf::Color color;
		
	public:
		SmoothDestroyableBlock(const sf::Vector2f& position, const sf::Color& color = sf::Color::Green);
		~SmoothDestroyableBlock() override = default;
		
		//Block
		virtual ColladiableType GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
		virtual void Update(float timeDelta) override;
		virtual bool IsBroken() override;

		//IDelayedAction		
		void EachTickAction(float deltaTime) override;
		 void FinalAction() override {};
	};

	class UnbreackableBlock : public Block
	{
	public:
		UnbreackableBlock(const sf::Vector2f& position);	
		void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override {};
	};

}
