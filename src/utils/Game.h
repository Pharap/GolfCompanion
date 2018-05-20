#pragma once

#include <Arduboy2.h>
#include "Utils.h"

#define EEPROM_START                  EEPROM_STORAGE_SPACE_START + 100
#define EEPROM_START_C1               EEPROM_START
#define EEPROM_START_C2               EEPROM_START + 1 
#define EEPROM_GAME_STATE             EEPROM_START_C2 + 1
#define EEPROM_NO_OF_PLAYERS          EEPROM_GAME_STATE + 1
#define EEPROM_PLAYER_BEING_EDITED    EEPROM_NO_OF_PLAYERS + 1
#define EEPROM_NO_OF_HOLES            EEPROM_PLAYER_BEING_EDITED + 1
#define EEPROM_CURRENT_HOLE           EEPROM_NO_OF_HOLES + 1
#define EEPROM_CURSOR_X               EEPROM_CURRENT_HOLE + 1
#define EEPROM_CURSOR_Y               EEPROM_CURSOR_X + 1
#define EEPROM_CURSOR_MODE            EEPROM_CURSOR_Y + 1
#define EEPROM_HOLES                  EEPROM_CURSOR_MODE + 1
#define EEPROM_TOTAL                  EEPROM_HOLES + 90
#define EEPROM_NAMES                  EEPROM_TOTAL + 5
    
class Game
{
public:
	constexpr const static uint8_t letter1 = 'G';
	constexpr const static uint8_t letter2 = 'C';
	constexpr const static uint8_t nameLength = NAME_LENGTH;
	constexpr const static uint8_t nameSize = nameLength + 1;

public:
	uint8_t numberOfPlayers = 1;
	uint8_t playerBeingEdited = 1; 

	uint8_t numberOfHoles = 18;
	uint8_t currentHoleNumber = 3;

	Cursor cursor;

	Hole holes[18];
	Hole total;

	char names[4][nameSize];

private:

	GameState _state = GameState::VSBoot;
	uint8_t order[4] = { 1, 2, 3, 4 };

public: 

	Game(void)
	{
		for (uint8_t i = 0; i < 4; ++i)
			for (uint8_t j = 0; j < nameLength; ++j)
				names[i][j] = ' ';
	}



	//--------------------------------------------------------------------------------------------------------------------------
	// Methods ..

public: 

	GameState getState() const
	{
		return _state;
	}

	void setState(GameState val)
	{
		_state = val;
		saveEEPROM();
	}

	/* ----------------------------------------------------------------------------
	*   Is the EEPROM initialised?
	*
	*   Looks for the characters 'G' and 'C' in the first two bytes of the EEPROM
	*   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
	*   it resets the settings ..
	*/
	void initEEPROM(bool forceClear)
	{
		uint8_t c1 = EEPROM.read(EEPROM_START_C1);
		uint8_t c2 = EEPROM.read(EEPROM_START_C2);

		if (forceClear || c1 != letter1 || c2 != letter2)
		{
			EEPROM.update(EEPROM_START_C1, letter1);
			EEPROM.update(EEPROM_START_C2, letter2);
			saveEEPROM();
		}
	}


	void saveEEPROM()
	{
		EEPROM.update(EEPROM_GAME_STATE, static_cast<uint8_t>(_state));
		EEPROM.update(EEPROM_NO_OF_PLAYERS, numberOfPlayers);
		EEPROM.update(EEPROM_PLAYER_BEING_EDITED, playerBeingEdited);
		EEPROM.update(EEPROM_NO_OF_HOLES, numberOfHoles);
		EEPROM.update(EEPROM_CURRENT_HOLE, currentHoleNumber);
		EEPROM.update(EEPROM_CURSOR_X, cursor.x);
		EEPROM.update(EEPROM_CURSOR_Y, cursor.y);
		EEPROM.update(EEPROM_CURSOR_MODE, static_cast<uint8_t>(cursor.mode));

		for (uint8_t i = 0; i < 18; ++i)
		{
			const uint16_t address = EEPROM_HOLES + (i * sizeof(Hole));
			if(i < numberOfHoles)
			{
				EEPROM.put(address, holes[i]);
			}
			else
			{
				const Hole hole = Hole();
				EEPROM.put(address, hole);
			}
		}

		EEPROM.put(EEPROM_TOTAL, total);

		uint8_t address = EEPROM_NAMES;

		for (uint8_t i = 0; i < 4; ++i)
			for (uint8_t j = 0; j < nameSize; ++j)
			{
				EEPROM.update(address, names[i][j]);
				++address;
			}
	}

	void loadEEPROM()
	{
		_state = static_cast<GameState>(EEPROM.read(EEPROM_GAME_STATE));
		numberOfPlayers = EEPROM.read(EEPROM_NO_OF_PLAYERS);
		playerBeingEdited = EEPROM.read(EEPROM_PLAYER_BEING_EDITED);
		numberOfHoles = EEPROM.read(EEPROM_NO_OF_HOLES);
		currentHoleNumber = EEPROM.read(EEPROM_CURRENT_HOLE);
		cursor.x = EEPROM.read(EEPROM_CURSOR_X);
		cursor.y = EEPROM.read(EEPROM_CURSOR_Y);
		cursor.mode = static_cast<CursorMode>(EEPROM.read(EEPROM_CURSOR_MODE));

		for (uint8_t i = 0; i < numberOfHoles; ++i)
		{
			if(i < numberOfHoles)
			{
				EEPROM.get(EEPROM_HOLES + (i * sizeof(Hole)), holes[i]);
			}
			else
			{
				holes[i] = Hole();
			}
		}

		EEPROM.get(EEPROM_TOTAL, total);

		uint8_t address = EEPROM_NAMES;

		for (uint8_t i = 0; i < 4; ++i)
			for (uint8_t j = 0; j < nameSize; ++j)
			{
				names[i][j] = EEPROM.read(address);
				++address;
			}
	}

	void clear(bool reset)
	{
		currentHoleNumber = 3;
		cursor.x = 0;
		cursor.y = 0;
		cursor.mode = CursorMode::Navigation;

		if (reset)
		{
			numberOfHoles = 18;

			for (uint8_t i = 0; i < numberOfHoles; ++i)
				holes[i] = Hole(4);

			numberOfPlayers = 1;
			playerBeingEdited = 1; 

			for (uint8_t i = 0; i < 4; ++i)
				for (uint8_t j = 0; j < nameLength; ++j)
					names[i][j] = ' ';

			total = Hole(72);
		}
		else
		{
			for (uint8_t i = 0; i < 18; ++i)
			{
				if(i < numberOfHoles)
					holes[i] = Hole(holes[i].par);
				else
					holes[i] = Hole();
			}

			uint8_t totalPar = 0;
			
			for (uint8_t i = 0; i < numberOfHoles; ++i)
				totalPar += holes[i].par;

			total = Hole(totalPar);
		}
	}

	uint8_t getOrder(uint8_t position) const
	{
		return order[position];
	}
	
	uint8_t getScore(uint8_t index) const
	{
		return total.getPlayerScore(index);
	}

	char * getName(uint8_t index)
	{
		return (index < 4) ? (&names[4][0]) : nullptr;
	}

	void determineWinners()
	{
		for (uint8_t i = 0 ; i < 4; ++i)
			order[i] = i;

		uint8_t swap;
		uint8_t n = numberOfPlayers;

		for (uint8_t i = 0 ; i < ( n - 1 ); ++i)
		{
			for (uint8_t j = 0 ; j < (n - i - 1); ++j)
			{
				if (getScore(order[j]) > getScore(order[j + 1]))
				{
					swap       = order[j];
					order[j]   = order[j + 1];
					order[j + 1] = swap;
				}
			}      
		}
	}
    
};
