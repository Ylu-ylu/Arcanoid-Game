#include "GameStateRecords.h"
#include "Application.h"
#include "Text.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>
#include <sstream>

namespace ArkanoidGame
{
	void GameStateRecordsData::Init()
	{
		assert(font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		titleText.setString("RECORDS");
		titleText.setFont(font);
		titleText.setFillColor(sf::Color::Red);
		titleText.setCharacterSize(48);

		SetTableRecords();

		hintText.setString("Press ESC to return back to main menu");
		hintText.setFont(font);
		hintText.setFillColor(sf::Color::White);
		hintText.setCharacterSize(24);
	}

	void GameStateRecordsData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PopState();
			}
		}
	}

	void GameStateRecordsData::Update(float timeDelta)
	{

	}

	void GameStateRecordsData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 0.f }));
		titleText.setPosition(viewSize.x / 2.f, 50.f);
		window.draw(titleText);

		// We need to create new vector here as DrawItemsList needs vector of pointers
		std::vector<sf::Text*> textsList;
		textsList.reserve(tableTexts.size());
		for (auto& text : tableTexts)
		{
			textsList.push_back(&text);
		}

		sf::Vector2f tablePosition = { titleText.getGlobalBounds().left, viewSize.y / 2.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { -0.3f, 0.f });

		hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 1.f }));
		hintText.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(hintText);
	}

	void GameStateRecordsData::SetTableRecords()
	{
		tableTexts.reserve(MAX_RECORDS_TABLE_SIZE);

		const Game& game = Application::Instance().GetGame();
		int playerScore = Application::Instance().GetGame().GetFinalScore();

		std::multimap<int, std::string> sortedRecordsTable;

		// Get the player's score	
		std::string playerName = PLAYER_NAME;

		for (const auto& item : game.GetRecordsTable())
		{
			sortedRecordsTable.insert(std::make_pair(item.second, item.first));
		}

		// Insert the player's score into the sorted map
		sortedRecordsTable.insert(std::make_pair(playerScore, playerName));

		auto it = sortedRecordsTable.rbegin();
		for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it) // Note, we can do several actions in for action block
		{
			tableTexts.emplace_back(); // Create text in place
			sf::Text& text = tableTexts.back();

			// We can use streams for writing into string and reading from it
			std::stringstream sstream;
			sstream << i + 1 << ". " << it->second << ": " << it->first;
			text.setString(sstream.str());
			text.setFont(font);
			text.setFillColor(it->second == playerName ? sf::Color::Green : sf::Color::White);
			text.setCharacterSize(24);

		}

		bool isPlayerInTable = false;

		if (!isPlayerInTable)
		{
			tableTexts.emplace_back();
			sf::Text& text = tableTexts.back();
			std::stringstream sstream;
			int playerScore = Application::Instance().GetGame().GetFinalScore();
			sstream << MAX_RECORDS_TABLE_SIZE << ". " << PLAYER_NAME << ": " << playerScore;
			text.setString(sstream.str());
			text.setFillColor(sf::Color::Green);
		}
	}
}
