#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Colladiable.h"
#include "IDelayedAction.h"
#include "IObserver.h"

namespace ArkanoidGame
{
	
	class Block :public GameObject, public Colladiable, public IObservable
	{
	
	protected:
		virtual  void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;
		int hitCount = 1;
		int hitDamage = 1;
		int points = 0;

	public:
		Block(const sf::Vector2f& position, const sf::Color& color = sf::Color::Green, int points=0);
		virtual ~Block() override= default;
		sf::FloatRect GetColladiableRect() const override { return GetRect(); };

		ColladiableType GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
		virtual void Update(float timeDelta) override;
		virtual bool IsBroken();

		virtual sf::FloatRect GetRect() const override { return GetSpriteRect(); }



		int GetPoints() const;
		// Public wrapper for Emit
	    void EmitEvent() { Emit(); }

		//For FriedgeBlocks
		virtual bool ShouldBreakImmediately() const;
	};

	class SmoothDestroyableBlock : public Block, public IDelayedAction
	{
		using Super = Block;
	protected:
		 void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;
		sf::Color color;
		
	public:
		SmoothDestroyableBlock(const sf::Vector2f& position, const sf::Color& color , int points = 0);
		~SmoothDestroyableBlock() override = default;
		
		//Block
		virtual ColladiableType GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
		virtual void Update(float timeDelta) override;
		virtual bool IsBroken() override;

		//IDelayedAction		
		void EachTickAction(float deltaTime) override;
		void FinalAction() override;
		
	};

	class UnbreackableBlock :  public SmoothDestroyableBlock
	{
	public:
		UnbreackableBlock(const sf::Vector2f& position, int points = 0);
		void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) override;			
	};

}
