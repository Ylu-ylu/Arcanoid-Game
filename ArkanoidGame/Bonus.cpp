#include "Bonus.h"
#include "GameSettings.h"
#include "randomizer.h"
#include <iostream>
#include "Sprite.h"
#include <assert.h>
#include "GameStatePlaying.h"


namespace
{
    namespace
    {
        // id textures
        namespace TEXTURE_ID 
        {
            const std::string TEXTURE_ID1 = "FireBall_Bonus";
            const std::string TEXTURE_ID2 = "FireBall";
            const std::string TEXTURE_ID3 = "ball";
            const std::string TEXTURE_ID4 = "FragileBlocks_Bonus";
			const std::string TEXTURE_ID5 = "BonusWidePlatform";
        };       
    }
    
}

namespace ArkanoidGame
{    
    std::string Bonus::getTextureId(BonusType type)
    {
        switch (type)
        {
        case BonusType::Fireball:
            return TEXTURE_ID::TEXTURE_ID2;
            break;
        case BonusType::FireBall_Bonus:
                return TEXTURE_ID::TEXTURE_ID1;
                break;
        case BonusType::FragileBlocks:
            return TEXTURE_ID::TEXTURE_ID4;
            break;
        case BonusType::WidePlatform:
			return TEXTURE_ID::TEXTURE_ID5;
            break;
        default:
            break;            
        }         
        
        return std::string();
    }

    Bonus::Bonus(const sf::Vector2f& position, BonusType type, const std::string& texturePath)
        : GameObject(GameObjectType::FireBall, SETTINGS.TEXTURES_PATH + getTextureId(type) + ".png", position, SETTINGS.BONUS_SIZE, SETTINGS.BONUS_SIZE)
    {
        

    }

    void Bonus::Update(float timeDelta)
    {

        if (!collected)
        {
            // Move bonus downward
            sf::Vector2f pos = sprite.getPosition();
            pos.y += SETTINGS.BONUS_SPEED * timeDelta;
            sprite.setPosition(pos);

            // Check if bonus has gone off screen
            if (pos.y > SETTINGS.SCREEN_HEGHT)
            {
                collected = true; // Mark for removal
            }
        }
    }

    ColladiableType Bonus::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
    {
        return Colladiable::GetCollision(collidableObject);
    }

    bool Bonus::CheckCollision(std::shared_ptr<Platform> platform)
    {
        if (collected) return false;

        // Check collision with platform
        sf::FloatRect bonusRect = GetColladiableRect();
        sf::FloatRect platformRect = platform->GetColladiableRect();

        // Check if bonus bottom overlaps with platform top
        if (bonusRect.intersects(platformRect))
        {
            collected = true;
            return true;
        }

        return false;
    }

    float Bonus::GetSize() const
    {
        return SETTINGS.BONUS_SIZE;
    }   

    FireBallBonus::FireBallBonus(const sf::Vector2f& position)
        : Bonus(position, BonusType::Fireball, SETTINGS.RESOURCES_PATH + getTextureId(BonusType::FireBall_Bonus) + ".png")
    {
        // Make the bonus more visible
        sprite.setScale(0.25f, 0.25f);      
    }

    void FireBallBonus::ApplyEffect(std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform)
    {
        if (ball)
        {
            // Simply call the Ball's method to change state
            ball->SetFireballState();            
        }
    }

    void FireBallBonus::RemoveEffect(std::shared_ptr<Ball> ball)
    {
        // Simply call the Ball's method to reset state
        ball->SetDefaultState();       
    }
      
    std::shared_ptr<Bonus> BonusFactory::CreateBonus(BonusType type, const sf::Vector2f& position)
    {
        switch (type)
        {
        case BonusType::Fireball:
            return std::make_shared<FireBallBonus>(position);
        case BonusType::FragileBlocks:
            return std::make_shared<FragileBlocksBonus>(position);
        case BonusType::WidePlatform:
            return std::make_shared<WidePlatformBonus>(position);
        default:
            throw std::runtime_error("Unknown bonus type");
        }              
        
    }

    // BonusManager implementation
    void BonusManager::AddBonus(std::shared_ptr<Bonus> bonus)
    {
        activeBonuses.push_back(bonus);
    }

    void BonusManager::UpdateBonuses(float timeDelta, std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform)
    {   
        std::vector<std::shared_ptr<Bonus>> remainingBonuses;
        std::vector<std::pair<std::shared_ptr<Bonus>, float>> effectsToAdd;
       
       
        for (auto& bonus : activeBonuses)
        {
            if (!bonus) continue; // Safety check for null bonus

            bonus->Update(timeDelta);

            bool collectedThisFrame = false;
            if (platform && bonus->CheckCollision(platform))
            {
                if (ball) bonus->ApplyEffect(ball, platform);
                
                effectsToAdd.push_back({ bonus, bonus->GetDuration() });
                collectedThisFrame = true;
            }

            if (!collectedThisFrame && !bonus->IsCollected())
            {
                remainingBonuses.push_back(bonus);
            }
        }
        activeBonuses = std::move(remainingBonuses);

        
        for (const auto& effect : effectsToAdd) {
            activeEffects.push_back(effect);
        }

        std::vector<std::pair<std::shared_ptr<Bonus>, float>> remainingEffects;
        for (auto& effectPair : activeEffects)
        {
            std::shared_ptr<Bonus>& bonusPtr = effectPair.first;
            float& duration = effectPair.second;

            if (!bonusPtr) continue;

            duration -= timeDelta;

            if (duration > 0)
            {
                remainingEffects.push_back(effectPair);
            }
            else
            {
                if (ball) bonusPtr->RemoveEffect(ball);
            }
        }
        activeEffects = std::move(remainingEffects);
    }

    void BonusManager::DrawBonuses(sf::RenderWindow& window)
    {
        // Remove null pointers from activeBonuses
        activeBonuses.erase(
            std::remove_if(activeBonuses.begin(), activeBonuses.end(),
                [](const std::shared_ptr<Bonus>& bonus) {
                    return bonus == nullptr;
                }),
            activeBonuses.end()
        );        
        for (auto& bonus : activeBonuses)
        {
            bonus->Draw(window);
        }
    }

    void BonusManager::Clear()
    {
        for (auto& effect : activeEffects)
        {
            // Just to be safe, remove all effects
            if (effect.first)
            {
                effect.first->RemoveEffect(nullptr);
            }
        }
        activeBonuses.clear();
        activeEffects.clear();
    }

    bool BonusManager::TrySpawnBonus(const sf::Vector2f& position)
    {  
        // Random chance to spawn bonus (10%)
        float chance = random<float>(0.0f, 1.0f);       

        if (chance <= SETTINGS.BONUS_SPAWN_CHANCE)
        {
            // Randomly choose a bonus type
            int randomBonusType = random<int>(0, 2); // 0-2 for three bonus types
            BonusType selectedType;

            switch (randomBonusType)
            {
            case 0:
                selectedType = BonusType::Fireball;               
                break;
            case 1:
                selectedType = BonusType::FragileBlocks;
                
                break;
            case 2:
                selectedType = BonusType::WidePlatform;
               
                break;
            }

            auto bonus = BonusFactory::CreateBonus(selectedType, position);
            AddBonus(bonus);
            return true;
        }       
        return false;
    }

    void BonusManager::SpawnSpecificBonus(BonusType type, const sf::Vector2f& position)
    {
       
        auto bonus = BonusFactory::CreateBonus(type, position);
        AddBonus(bonus);    
    }


	//FrigeBlocksBonus
    // Initialize static member
    bool FragileBlocksBonus::fragileBlocksActive = false;

    FragileBlocksBonus::FragileBlocksBonus(const sf::Vector2f& position)
        : Bonus(position, BonusType::FragileBlocks, SETTINGS.BONUS_TEXTURES_PATH + getTextureId(BonusType::FragileBlocks)+".png")
    {     

       
    }

    void FragileBlocksBonus::ApplyEffect(std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform)
    {      

        // Set global flag
        fragileBlocksActive = true;

        // Visual feedback on the ball
        if (ball) {
            ball->GetSprite().setColor(sf::Color(0, 255, 255, 200)); // Cyan tint
        }
    }

    void FragileBlocksBonus::RemoveEffect(std::shared_ptr<Ball> ball)
    {    

        // Reset global flag
        fragileBlocksActive = false;

        // Reset ball appearance
        if (ball) {
            ball->GetSprite().setColor(sf::Color::White);
        }
    }

    // Initialize the static member of WidePlatformBonus
    bool WidePlatformBonus::widePlatformActive = false;

    WidePlatformBonus::WidePlatformBonus(const sf::Vector2f& position)
		: Bonus(position, BonusType::WidePlatform, SETTINGS.BONUS_TEXTURES_PATH + getTextureId(BonusType::WidePlatform) + ".png")
	{
		
	}

   

            void WidePlatformBonus::ApplyEffect(std::shared_ptr<Ball> ball, std::shared_ptr<Platform> platform)
            {           

                // Mark the bonus as active
                widePlatformActive = true;

                // Check if the platform is already affected
                if (auto platform = affectedPlatform.lock())
                {
                    // Already applied to a platform, do not reapply
                    return;
                }                

                if (platform)
                {
                    // Store the platform for later reference
                    affectedPlatform = platform;

                    // Get the sprite and scale it
                    sf::Sprite& platformSprite = platform->GetSprite();
                    sf::Vector2f currentScale = platformSprite.getScale();
                    platformSprite.setScale(currentScale.x * widthMultiplier, currentScale.y);

                    // Add visual feedback
                    platformSprite.setColor(sf::Color(200, 255, 200)); // Light green tint
                }              
            }

            void WidePlatformBonus::RemoveEffect(std::shared_ptr<Ball> ball)
            {              

                // Reset the bonus flag
                widePlatformActive = false;

                // Get the platform from the weak_ptr
                if (auto platform = affectedPlatform.lock())
                {
                    // Reset the platform size
                    auto& platformSprite = platform->GetSprite();
                    sf::Vector2f currentScale = platformSprite.getScale();
                    platformSprite.setScale(currentScale.x / widthMultiplier, currentScale.y);

                    // Reset the platform color
                    platformSprite.setColor(sf::Color::White);
                }
            }           

}


