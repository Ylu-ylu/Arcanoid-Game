#include "GameState.h"
#include "GameStateData.h"
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStateGameWin.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"
#include <assert.h>


namespace ArkanoidGame
{
	GameState::GameState(GameStateType type, bool isExclusivelyVisible)
		: type(type)
		, isExclusivelyVisible(isExclusivelyVisible)
	{
		switch (type)
		{
		case GameStateType::MainMenu:
		{
			data = new GameStateMainMenuData();
			((GameStateMainMenuData*)data)->Init();			
			break;
		}
		case GameStateType::Playing:
		{
			data = new GameStatePlayingData();
			((GameStatePlayingData*)data)->Init();
			break;
		}
		case GameStateType::GameOver:
		{
			data = new GameStateGameOverData();
			((GameStateGameOverData*)data)->Init();
			break;
		}
		case GameStateType::GameWin:
		{
			data = new GameStateGameWinData();
			((GameStateGameWinData*)data)->Init();
			break;
		}
		case GameStateType::ExitDialog:
		{
			data = new GameStatePauseMenuData();
			((GameStatePauseMenuData*)data)->Init();
			break;
		}
		case GameStateType::Records:
		{
			data = new GameStateRecordsData();
			((GameStateRecordsData*)data)->Init();
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}		
	}

	GameState::~GameState()
	{
		if (data) 
		{
			switch (type)
			{
				case GameStateType::MainMenu:
				{
					delete (GameStateMainMenuData*)data;
					break;
				}
				case GameStateType::Playing:
				{
					delete (GameStatePlayingData*)data;
					break;
				}
				case GameStateType::GameOver:
				{
					delete (GameStateGameOverData*)data;
					break;
				}
				case GameStateType::GameWin:
				{
					delete (GameStateGameWinData*)data;
					break;
				}
				case GameStateType::ExitDialog:
				{
					delete (GameStatePauseMenuData*)data;
					break;
				}
				case GameStateType::Records:
				{
					delete (GameStateRecordsData*)data;
					break;
				}
			default:
				break;
			}		
			data = nullptr;
		}
	}

	void GameState::Update(float timeDelta)
	{
		static_cast<GameStateData*>(data)->Update(timeDelta);
	}

	void GameState::Draw(sf::RenderWindow& window)
	{
		static_cast<GameStateData*>(data)->Draw(window);
	}

	void GameState::HandleWindowEvent(sf::Event& event)
	{
		static_cast<GameStateData*>(data)->HandleWindowEvent(event);
	}
	void* GameState::CopyDate(const GameState& state) const
	{
		void* data = nullptr;
		switch (state.type)
		{
		case GameStateType::MainMenu:
		{
			data = new GameStateMainMenuData();
			*((GameStateMainMenuData*)data)= *((GameStateMainMenuData*)state.data);
			break;
		}
		case GameStateType::Playing:
		{
			data = new GameStatePlayingData();
			*((GameStatePlayingData*)data) = *((GameStatePlayingData*)state.data);
			break;
		}
		case GameStateType::GameOver:
		{
			data = new GameStateGameOverData();
			*((GameStateGameOverData*)data)= *((GameStateGameOverData*)state.data);
			break;
		}
		case GameStateType::GameWin:
		{
			data = new GameStateGameWinData();
			*((GameStateGameWinData*)data)= *((GameStateGameWinData*)state.data);
			break;
		}
		case GameStateType::ExitDialog:
		{
			data = new GameStatePauseMenuData();
			*((GameStatePauseMenuData*)data)= *((GameStatePauseMenuData*)state.data);
			break;
		}
		case GameStateType::Records:
		{
			data = new GameStateRecordsData();
			*((GameStateRecordsData*)data)= *((GameStateRecordsData*)state.data);
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
		return data;
	}

	
}