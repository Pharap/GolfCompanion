#pragma once

#include <Arduboy2.h>

#define HALF_CHARACTER 2
#define GRID_VERT_SPACING 11
#define GRID_HORZ_SPACING 16
#define GRID_HOLE_1_X 40
#define GRID_TOTAL_X 110
#define GRID_PAR_Y 11
#define GRID_CELL_SPACING 4
#define KEY_REPEAT_DELAY 12


// ----------------------------------------------------------------------------
//  A better absolute as it uses less memory than the standard one .. 
//
template<typename T> T absT(const T & v) {
  
  return (v < 0) ? -v : v;

}


enum class MenuSelection : uint8_t {

  NewGame,
  ResetScores,
  CreditScreen,

};

constexpr const MenuSelection MenuSelectionFirst = MenuSelection::NewGame;
constexpr const MenuSelection MenuSelectionLast = MenuSelection::CreditScreen;

MenuSelection previousMenuSelection(MenuSelection selection)
{
	if(selection == MenuSelectionFirst)
		return selection;
		
	const uint8_t value = static_cast<uint8_t>(selection);
	return static_cast<MenuSelection>(value - 1);
}

MenuSelection nextMenuSelection(MenuSelection selection)
{
	if(selection == MenuSelectionLast)
		return selection;
	
	const uint8_t value = static_cast<uint8_t>(selection);
	return static_cast<MenuSelection>(value + 1);
}

enum class GameState : uint8_t {

  VSBoot,
  SplashScreen,
  CreditScreen,
  NumberOfPlayers,
  PlayerNames_Init,
  PlayerNames,
  NumberOfHoles,
  InGame_Init,
  InGame,
  FinalScore,

};

struct Hole
{
	uint8_t par = 0;  
	uint8_t playerScore[4] ={};
	
	Hole(void) = default;
	
	Hole(uint8_t par)
		: par(par)
	{
	}

	uint8_t getPlayerScore(uint8_t index) const
	{
		return (index < 4) ? this->playerScore[index] : 0;
	}

	void setPlayerScore(uint8_t index, uint8_t value)
	{
		if(index < 4)
			this->playerScore[index] = value;
	}
};

enum class CursorMode : uint8_t {

  Navigation,
  Edit,

};

struct Cursor {
 
  uint8_t x = 0;
  uint8_t y = 0;
  CursorMode mode = CursorMode::Navigation;  

};

