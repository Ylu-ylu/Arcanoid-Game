#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include <iostream>

namespace ArkanoidGame
{
	enum class ColladiableType
	{
		None,
		Overlap,
		Hit,
		HitInverse,		
	};
	
	class Colladiable  //:public std::enable_shared_from_this<Colladiable>
	{
	private:
		std::shared_ptr<Colladiable> self;
	protected:
		
		virtual void OnHit(ColladiableType type, std::shared_ptr<Colladiable> collidableWhith) = 0;
	
	public:
		void SetSelf(std::shared_ptr<Colladiable> self_ptr) { self = self_ptr; }
		
		
        virtual bool CheckCollision(const std::shared_ptr<Colladiable> collidable) 
		{
			const ColladiableType type = GetCollision(collidable);
			
			if (type != ColladiableType::None)
			{
				
				OnHit(type, collidable);			
				collidable->OnHit(type, self);
				return true;
			}			
			return false;
		}
		virtual sf::FloatRect GetRect() const = 0;

		virtual ColladiableType GetCollision(const std::shared_ptr<Colladiable> collidable) const
		{			
			return GetColladiableRect().intersects(collidable->GetRect()) ?
				(ColladiableType::Hit) :(ColladiableType::None);			
		}		
		virtual sf::FloatRect GetColladiableRect() const = 0;	
		virtual ~Colladiable() = default;
	};

}