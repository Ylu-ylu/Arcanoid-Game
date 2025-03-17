#pragma once
#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "GameSettings.h"
#include <unordered_map>
#include "GameState.h"

namespace ArkanoidGame
{
	class RecordsTable;
	
	enum class GameStateChangeType
	{
		None,
		Push,
		Pop,
		Switch
	};

	class Game
	{
	public:
		void StartGame();
		void PauseGame();		
		void LooseGame();		
		void ExitGame();
		void QuitGame();
		void ShowRecords();		
		void SetFinalScore(int finalScore) { m_finalScore = finalScore; }
		int GetFinalScore() const { return m_finalScore; };

	public:
		using RecordsTable = std::unordered_map<std::string, int>;

		Game();
		~Game();

		const RecordsTable& GetRecordsTable() const { return recordsTable; }
		// Remove current game state from the stack
		void PopState();

	public:
		void HandleWindowEvents(sf::RenderWindow& window);
		bool Update(float timeDelta); // Return false if game should be closed
		void Draw(sf::RenderWindow& window);		

	private:
		// Add new game state on top of the stack
		void PushState(GameStateType stateType, bool isExclusivelyVisible);
		// Remove all game states from the stack and add new one
		void SwitchStateTo(GameStateType newState);
		void Shutdown();		

	private:
		std::vector<GameState> stateStack;
		GameStateChangeType stateChangeType = GameStateChangeType::None;
		GameStateType pendingGameStateType = GameStateType::None;
		bool pendingGameStateIsExclusivelyVisible = false;

		int m_finalScore = 0;		
		RecordsTable recordsTable;
	};
		
}
