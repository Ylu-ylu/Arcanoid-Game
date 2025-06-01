#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>

namespace ArkanoidGame
{
	void GameStateMainMenuData::Init()
	{
		//Background
		assert(backgroundMainMenuTexture.loadFromFile(SETTINGS.TEXTURES_PATH + "MainMenu_BackGround2.png"));
		backgroundMainMenuSprite.setTexture(backgroundMainMenuTexture);
		backgroundMainMenuSprite.setColor(sf::Color(255, 255, 255, 255)); // Set alpha to 50% (128/255)
		backgroundMainMenuSprite.setPosition(0, 0); // Set position to top-left corner of the screen
		backgroundMainMenuSprite.setScale((float)SETTINGS.SCREEN_WIDTH / backgroundMainMenuTexture.getSize().x, (float)SETTINGS.SCREEN_HEGHT / backgroundMainMenuTexture.getSize().y); // Scale to cover the entire screen
			
		
		assert(font.loadFromFile(SETTINGS.RESOURCES_PATH + "Fonts/Roboto- Galaxy Alternate Italic.ttf"));

		MenuItem startGame;
		startGame.text.setString("Start Game");
		startGame.text.setFont(font);
		startGame.text.setCharacterSize(40);
		startGame.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().StartGame();
			};
		
		MenuItem recordsItem;
		recordsItem.text.setString("Records");
		recordsItem.text.setFont(font);
		recordsItem.text.setCharacterSize(40);
		recordsItem.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().ShowRecords();
			};

		MenuItem yesItem;
		yesItem.text.setString("Yes");
		yesItem.text.setFont(font);
		yesItem.text.setCharacterSize(24);
		yesItem.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().QuitGame();
			};

		MenuItem noItem;
		noItem.text.setString("No");
		noItem.text.setFont(font);
		noItem.text.setCharacterSize(24);
		noItem.onPressCallback = [this](MenuItem&) {
			menu.GoBack();
			};


		// --- Sound Settings Menu ---
		MenuItem soundSettingsItem;
		soundSettingsItem.text.setString("Sound Settings");
		soundSettingsItem.text.setFont(font);
		soundSettingsItem.text.setCharacterSize(40);

		MenuItem soundOnItem;
		soundOnItem.text.setString("On");
		soundOnItem.text.setFont(font);
		soundOnItem.text.setCharacterSize(32);
		
		soundOnItem.onPressCallback = [](MenuItem&) {			
			Application::Instance().GetGame().SetSoundEnabled(true);
			};

		MenuItem soundOffItem;
		soundOffItem.text.setString("Off");
		soundOffItem.text.setFont(font);
		soundOffItem.text.setCharacterSize(32);
		
		soundOffItem.onPressCallback = [](MenuItem&) {			
			Application::Instance().GetGame().SetSoundEnabled(false);
			};

		soundSettingsItem.childrenOrientation = Orientation::Horizontal;
		soundSettingsItem.childrenAlignment = Alignment::Middle;
		soundSettingsItem.childrenSpacing = 30.f;
		soundSettingsItem.childrens.push_back(soundOnItem);
		soundSettingsItem.childrens.push_back(soundOffItem);

		MenuItem exitGameItem;
		exitGameItem.text.setString("Exit Game");
		exitGameItem.text.setFont(font);
		exitGameItem.text.setCharacterSize(40);
		exitGameItem.hintText.setString("Are you sure?");
		exitGameItem.hintText.setFont(font);
		exitGameItem.hintText.setCharacterSize(48);
		exitGameItem.hintText.setFillColor(sf::Color::Red);
		exitGameItem.childrenOrientation = Orientation::Horizontal;
		exitGameItem.childrenAlignment = Alignment::Middle;
		exitGameItem.childrenSpacing = 10.f;
		exitGameItem.childrens.push_back(yesItem);
		exitGameItem.childrens.push_back(noItem);

		MenuItem mainMenu;
		mainMenu.hintText.setString("Arkanoid Game");
		mainMenu.hintText.setFont(font);
		mainMenu.hintText.setCharacterSize(62);
		mainMenu.hintText.setFillColor(sf::Color(235, 238, 247));

		mainMenu.childrenOrientation = Orientation::Vertical;
		mainMenu.childrenAlignment = Alignment::Middle;
		mainMenu.childrenSpacing = 10.f;
		mainMenu.childrens.push_back(startGame);
		mainMenu.childrens.push_back(recordsItem);
		mainMenu.childrens.push_back(soundSettingsItem);	
		mainMenu.childrens.push_back(exitGameItem);

		menu.Init(mainMenu);
	}

	void GameStateMainMenuData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				menu.GoBack();
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				menu.PressOnSelectedItem();
			}

			Orientation orientation = menu.GetCurrentContext().childrenOrientation;
			if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
			{
				menu.SwitchToPreviousMenuItem();
			}
			else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
			{
				menu.SwitchToNextMenuItem();
			}
		}
	}

	void GameStateMainMenuData::Update(float timeDelta)
	{
	

	}

	void GameStateMainMenuData::Draw(sf::RenderWindow& window)
	{
		//BackGround
		window.draw(sf::Sprite(backgroundMainMenuSprite));
		
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		sf::Text* hintText = &menu.GetCurrentContext().hintText;
		hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
		hintText->setPosition(viewSize.x / 2.f, 105.f);
		window.draw(*hintText);

		menu.Draw(window, { viewSize.x / 2.f, viewSize.y / 2.f + 90.f }, { 0.5f, 0.f });
	}

}
