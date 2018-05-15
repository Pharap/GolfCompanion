#pragma once

#include <Arduboy2.h>

//Starting location in EEPROM for Players Names
#define EEPROM_PLAYER_NAMES 150    

// Name 1 ->   EPROM Loc 150 - 153
// 0 at 154
// Name 2 ->   EPROM Loc 155 - 158
// 0 at 159
// Name 3 ->   EPROM Loc 160 - 163
// 0 at 164
// Name 4 ->   EPROM Loc 165 - 168
// 0 at 169


#define GRID_VERT_SPACING 11
#define GRID_HORZ_SPACING 16
#define GRID_HOLE_1_X 40
#define GRID_TOTAL_X 100
#define GRID_PAR_Y 11
#define GRID_CELL_SPACING 4


enum class GameState : uint8_t {

  VSBoot,
  SplashScreen,
  NumberOfPlayers,
  PlayerNames_Init,
  PlayerNames,
  NumberOfHoles,
  InGame_Init,
  InGame,
  FinalScore

};

struct Hole {

  uint8_t par = 4;  
  uint8_t player1Score;
  uint8_t player2Score;
  uint8_t player3Score;
  uint8_t player4Score;

  uint8_t getPlayerScore(uint8_t index) {

    switch (index) {

      case 1: return player1Score;
      case 2: return player2Score;
      case 3: return player3Score;
      case 4: return player4Score;
      default: return 0;
    }

  }

  void setPlayerScore(uint8_t index, uint8_t val) {

    switch (index) {

      case 1: player1Score = val; break;
      case 2: player2Score = val; break;
      case 3: player3Score = val; break;
      case 4: player4Score = val; break;

    }

  }

};

struct Cursor {
 
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t mode = 0;  

};