#pragma once
#include <SFML/Graphics.hpp>
#include <memory>      // Include for std::shared_ptr
#include <utility>     // Include for std::move
#include "GameStateData.h"

namespace ArkanoidGame
{
	enum class GameStateType
	{
		None = 0,
		MainMenu,
		Playing,
		GameOver,
		GameWin,
		ExitDialog,
		Records,
	};
	class GameState
	{
	public:
		GameState() = default;
		GameState(GameStateType type, bool isExclusivelyVisible);
		GameState(const GameState& state) = delete;
		// Mark the move constructor as noexcept and remove internal include
		GameState(GameState&& state) noexcept { operator=(std::move(state)); }
		~GameState();

		GameState& operator= (const GameState& state) = delete;
		
		GameState& operator= (GameState&& state) noexcept 
		{
			type = state.type;			
			data = std::move(state.data);  // Use move instead of copy
			isExclusivelyVisible = state.isExclusivelyVisible;
			state.data=nullptr;
			return *this;
		}
		GameStateType GetType() const { return type; }
		bool IsExclusivelyVisible() const { return isExclusivelyVisible; }

		template<class T>
		T* GetData() const 
		{
			return static_cast<T*>(data.get());
		}

		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);
		void HandleWindowEvent(sf::Event& event);

	private:
		GameStateType type = GameStateType::None;
		std::shared_ptr<GameStateData> data = nullptr;
		bool isExclusivelyVisible = false;
	};
}