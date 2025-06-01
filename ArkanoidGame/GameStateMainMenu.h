#pragma once
#include "SFML/Graphics.hpp"
#include "GameStateData.h"
#include "Menu.h"

namespace ArkanoidGame
{
	class Game;

	class GameStateMainMenuData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

	private:
		sf::Font font;
		Menu menu;

		//Background
		sf::Texture backgroundMainMenuTexture;
		sf::Sprite backgroundMainMenuSprite;

		//For Sound
		MenuItem soundSettingsItem;

		MenuItem soundToggleItem;
		MenuItem musicToggleItem;

		bool soundOn = true;
		bool musicOn = true;
	};
}
