#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Name.h"
#include "src/utils/Utils.h"
#include "src/images/Images.h"
#include "src/fonts/Font4x6.h"
#include "src/utils/Game.h"
 
Arduboy2Ext arduboy;
Font4x6 font4x6 = Font4x6();

Sprites sprite;
Name name;


//globals
uint8_t fadeWidth;
uint8_t frameCount = 0; 
uint16_t backdropx = 0;
uint16_t backdropy = 0;
MenuSelection menuSelection = MenuSelection::NewGame;
Game game;


// ----------------------------------------------------------------------------
//  Setup
//
void setup() {

  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.initRandomSeed();
  arduboy.setFrameRate(60);

  name.clear();
  game.initEEPROM(false);
  game.loadEEPROM();

}


// ----------------------------------------------------------------------------
//  Space pad a number of x digits .. 
//
void printPaddedNumber(uint8_t val, uint8_t digits)
{
  if (digits >= 4 && val < 1000)
	font4x6.print(' ');
	
  if (digits >= 3 && val < 100)
	font4x6.print(' ');
	
  if (digits >= 2 && val < 10)
	font4x6.print(' ');
	
  font4x6.print(val);
}


// ----------------------------------------------------------------------------
//  Flash the cursor? 
//
bool flashCursor() {
  return (arduboy.getFrameCount(64) < 32);
}


// ----------------------------------------------------------------------------
//  Reset the fade effect .. 
//
void resetFade() {
  fadeWidth = 0;
}

// ----------------------------------------------------------------------------
//  Reset the fade in effect .. 
//
void resetFadeIn() {
  fadeWidth = WIDTH;
}


// ----------------------------------------------------------------------------
//  Fade In function .. 
//
bool fadeIn() {

  for (uint8_t i = 0; i < (HEIGHT / 2); ++i)
  {
    arduboy.drawFastHLine(0, (i * 2), fadeWidth, BLACK);
    arduboy.drawFastHLine((WIDTH - fadeWidth), (i * 2) + 1, fadeWidth, BLACK);
  }

  // If fade isn't complete, decrease the counter
  if (fadeWidth > 0) 
  {
    fadeWidth = fadeWidth - 4;
    return false;
  }
  else 
  {
    return true;
  }

}


// ----------------------------------------------------------------------------
//  Fade Out function .. 
//
bool fadeOut() 
{

  for (uint8_t i = 0; i < (HEIGHT / 2); ++i) 
  {
    arduboy.drawFastHLine(0, (i * 2), fadeWidth, BLACK);
    arduboy.drawFastHLine((WIDTH - fadeWidth), (i * 2) + 1, fadeWidth, BLACK);
  }

  // If fade isn't complete, increase the counter
  if(fadeWidth < WIDTH) 
  {
    fadeWidth += 2;
    return false;
  }
  else 
  {
    return true;
  }
  
}

void drawBottomGrass()
{
  sprite.drawExternalMask(0, 46, grasstee, grassteemask, 0, 0);
}

void drawPopWindow()
{
  arduboy.fillRect(0, 24, 128, 18, BLACK);
  arduboy.drawLine(0, 25, 128, 25, WHITE);
  arduboy.drawLine(0, 40, 128, 40, WHITE);
}

void drawHeader()
{
  sprite.drawExternalMask(15, 2, GolfCompanionHeader, GolfCompanionHeadermask, 0,0);
}

void numberPlayers()
{

  scrollingBackground();
  drawBottomGrass();
  drawHeader();
  drawPopWindow();
  font4x6.setCursor(6, 29);
  font4x6.print(F("How many players? "));
  font4x6.println(game.numberOfPlayers);

  if (flashCursor()) { arduboy.drawLine(96, 37, 99, 37); }

  if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.isFrameCount(KEY_REPEAT_DELAY, frameCount)) {

    if (arduboy.pressed(UP_BUTTON) && game.numberOfPlayers < 4)         
    { 
      ++game.numberOfPlayers;
      frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
    }
    else if (arduboy.pressed(DOWN_BUTTON)&& game.numberOfPlayers > 1)   
    { 
      --game.numberOfPlayers; 
      frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
    }

  }
  
  if (arduboy.justPressed(A_BUTTON))                                 
  { 
    game.setState(GameState::PlayerNames_Init); 
  }
  else if (arduboy.justPressed(B_BUTTON))                                 
  { 
    game.setState(GameState::SplashScreen);
  }

}

void playerNames() 
{
	scrollingBackground();
	drawBottomGrass();
	drawHeader();
	drawPopWindow();

	font4x6.setCursor(6, 29);
	font4x6.print(F("Player "));
	font4x6.print(game.playerBeingEdited);
	font4x6.print(F(" name? "));

	for (uint8_t x = 0; x < NAME_LENGTH; ++x) 
	{
		font4x6.setCursor(82 + (x * NAME_CHARACTER_SPACING), 29);

		if (flashCursor() && name.getCharIndex() == x)
		{
			arduboy.drawLine(82 + (x * NAME_CHARACTER_SPACING), 37, 82 + (x * NAME_CHARACTER_SPACING) + NAME_UNDERLINE_WIDTH, 37, WHITE);
		}

		font4x6.print(name.getChar(x));
	}


	// Handle buttons ..

	uint8_t charIndex = name.getCharIndex();

	if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.isFrameCount(KEY_REPEAT_DELAY, frameCount))
	{
		if (arduboy.pressed(UP_BUTTON))
		{ 
				name.incChar(charIndex); 
				frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
		}
		if (arduboy.pressed(DOWN_BUTTON))
		{ 
			name.decChar(charIndex); 
			frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
		}		
	}
	
	if (arduboy.justPressed(LEFT_BUTTON))		 
	{ 
		name.decCharIndex(); 
	} 
	else if (arduboy.justPressed(RIGHT_BUTTON))		
	{ 
		name.incCharIndex(); 
	} 
	else if (arduboy.justPressed(B_BUTTON))
	{
		if (game.playerBeingEdited > 1)
		{
			game.setState(GameState::PlayerNames_Init);
			--game.playerBeingEdited;
		}
		else
		{
			game.setState(GameState::NumberOfPlayers);
		}
	}

	if (arduboy.justPressed(A_BUTTON))				
	{		
		// name.save(EEPROM_PLAYER_NAMES + ((game.playerBeingEdited - 1) * (NAME_LENGTH + 1)));
		switch (game.playerBeingEdited)
		{
			case 1:
				strcpy(game.name1 , name.getString());
				break;

			case 2:
				strcpy(game.name2 , name.getString());
				break;

			case 3:
				strcpy(game.name3 , name.getString());
				break;

			case 4:
				strcpy(game.name4 , name.getString());
				break;
		}

		if (game.playerBeingEdited < game.numberOfPlayers) 
		{
			// Enter the next player's name ..
			game.setState(GameState::PlayerNames_Init);
			++game.playerBeingEdited;		
		}
		else 
		{
			game.setState(GameState::NumberOfHoles);
		}		
	}
}

void numberHoles()
{
	scrollingBackground();
	drawBottomGrass();
	drawHeader();
	drawPopWindow();
	font4x6.setCursor(6, 29);
	font4x6.print(F("How many Holes? "));
	font4x6.println(game.numberOfHoles);

	if (flashCursor()) { arduboy.drawLine(86, 37, (game.numberOfHoles < 10 ? 89 : 94), 37); }

	if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.isFrameCount(KEY_REPEAT_DELAY, frameCount))
	{
		if (arduboy.pressed(UP_BUTTON) && game.numberOfHoles < 18)				 
		{ 
			clearScoreDetails(game.numberOfHoles - 1, 4);
			++game.numberOfHoles; 
			frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
		}
		else if (arduboy.pressed(DOWN_BUTTON) && game.numberOfHoles > 3)	 
		{ 
			--game.numberOfHoles; 
			clearScoreDetails(game.numberOfHoles - 1, 0);
			if (game.numberOfHoles < game.currentHoleNumber) game.currentHoleNumber--;
			frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
		}
	}
	
	if (arduboy.justPressed(A_BUTTON))
	{ 
		game.setState(GameState::InGame_Init); 
	}
	else if (arduboy.justPressed(B_BUTTON))
	{ 
		game.setState(GameState::PlayerNames_Init); 
	}
}

void clearScoreDetails(uint8_t holeNumber, uint8_t par)
{
	game.holes[holeNumber] = Hole(par);  
	game.total = Hole();

	for (uint8_t i = 0; i < game.numberOfHoles; ++i)
	{
		if(i < game.numberOfHoles)
		{
			game.total.par += game.holes[i].par;
			for (uint8_t j = 0; j < game.numberOfHoles; ++j)
				game.total.playerScore[j] += game.holes[i].playerScore[j];
		}
		else
		{	
			game.holes[i] = Hole();
		}
	}
}


void scrollingBackground()
{
  arduboy.drawBitmap(backdropx, backdropy, GolfBackground, 128, 64, WHITE);
  arduboy.drawBitmap(backdropx + 128, backdropy, GolfBackground, 128, 64, WHITE);

  if(arduboy.everyXFrames(5))
  { // when running at 60fps
    --backdropx;
    if (backdropx < -128)
    {
      backdropx = 0;
    }
  }
}

void loop()
{

  if (!arduboy.nextFrame()) return;

  arduboy.clear();
  arduboy.pollButtons();
  arduboy.clear();

  switch (game.getState())
  {
    case GameState::VSBoot:
      vsBoot();
      break;

    case GameState::SplashScreen:
      splashScreen();
      break;

    case GameState::CreditScreen:
      creditScreen();
      break;

    case GameState::NumberOfPlayers:
      numberPlayers();
      break;

    case GameState::PlayerNames_Init:    
      game.setState(GameState::PlayerNames);

      switch (game.playerBeingEdited)
      {
        case 1:
          name.setChars(game.name1);
          break;

        case 2:
          name.setChars(game.name2);
          break;

        case 3:
          name.setChars(game.name3);
          break;

        case 4:
          name.setChars(game.name4);
          break;
      }

    case GameState::PlayerNames:
      playerNames();
      break;

    case GameState::NumberOfHoles:
      numberHoles();
      break;

    case GameState::InGame_Init:
      game.setState(GameState::InGame);

    case GameState::InGame:
      inGame();
      break;

    case GameState::FinalScore:
      finalScoreDisplay();
      break;
      
    } 
  
  arduboy.display();
}

void vsBoot()
{
	// Vsoft logo display
	arduboy.drawBitmap(0, 0, bootlogo, 128, 64, WHITE);

	if (fadeOut())
	{
		resetFade();
		resetFadeIn();
		game.setState(GameState::SplashScreen);
	}
}

void splashScreen()
{
  scrollingBackground();
  drawBottomGrass();
  sprite.drawExternalMask(0, 0, signTitle, signTitleMask, 0,0);
  fadeIn();

  switch (menuSelection) {

    case MenuSelection::NewGame:
      Sprites::drawErase(90, 21, cursor_mask, 0);
      Sprites::drawSelfMasked(90, 21, cursor, 0);
      break;

    case MenuSelection::ResetScores:
      Sprites::drawErase(90, 29, cursor_mask, 0);
      Sprites::drawSelfMasked(90, 29, cursor, 0);
      break;

    case MenuSelection::CreditScreen:
      Sprites::drawErase(90, 37, cursor_mask, 0);
      Sprites::drawSelfMasked(90, 37, cursor, 0);
      break;

  }

  if (arduboy.justPressed(UP_BUTTON))
    menuSelection = previousMenuSelection(menuSelection);
    
  if (arduboy.justPressed(DOWN_BUTTON))
    menuSelection = nextMenuSelection(menuSelection);

  // If 'A' button is pressed move to gameplay
  if (arduboy.justPressed(A_BUTTON)) {
    
    switch (menuSelection) {

      case MenuSelection::NewGame:
        game.clear(true);
        game.setState(GameState::NumberOfPlayers); 
        resetFadeIn();
        break;

      case MenuSelection::ResetScores:
        game.clear(false);
        game.setState(GameState::InGame_Init); 
        resetFadeIn();
        break;

      case MenuSelection::CreditScreen:
        game.clear(false);
        game.setState(GameState::CreditScreen); 
        resetFadeIn();
        break;

    } 
  }
}


void inGame()
{

  Sprites::drawOverwrite(0, 0, scorecard, 0);
  
	/*for(uint8_t i = 0; i < 4; ++i)
	{
		if (game.numberOfPlayers > i)
		{
			
			font4x6.setCursor(1, GRID_PAR_Y + ((i + 1) * GRID_VERT_SPACING));
			font4x6.print(game.name[i]);
		}
	}*/

  font4x6.setCursor(1, GRID_PAR_Y + (1 * GRID_VERT_SPACING));
  font4x6.print(game.name1);

  if (game.numberOfPlayers >= 2) 
  {
    font4x6.setCursor(1, GRID_PAR_Y + (2 * GRID_VERT_SPACING));
    font4x6.print(game.name2);
  }
  if (game.numberOfPlayers >= 3) 
  {
    font4x6.setCursor(1, GRID_PAR_Y + (3 * GRID_VERT_SPACING));
    font4x6.print(game.name3);
  }
  if (game.numberOfPlayers >= 4) 
  {
    font4x6.setCursor(1, GRID_PAR_Y + (4 * GRID_VERT_SPACING));
    font4x6.print(game.name4);
  }
  
  //arduboy.drawHorizontalDottedLine(0, WIDTH, GRID_PAR_Y - 2, 2);
  
	for(uint8_t i = 0; i < 3; ++i)
	{
		//const uint8_t gridSpacingVertical = (i * GRID_VERT_SPACING);
		const uint8_t gridSpacingHorizontal = (i * GRID_HORZ_SPACING);
		
		{
			const uint8_t k = 2 - i;
			
			const uint8_t holeIndex = game.currentHoleNumber - k;
			
			const uint8_t x = GRID_HOLE_1_X + gridSpacingHorizontal + GRID_CELL_SPACING + (holeIndex < 10 ? HALF_CHARACTER : 0);
			const uint8_t y = 0;

			font4x6.setCursor(x, y);
			font4x6.print(holeIndex);
		}
		
		{
			const uint8_t k = 3 - i;
			const uint8_t holeIndex = game.currentHoleNumber - k;
			const uint8_t par = game.holes[holeIndex].par;
			
			const uint8_t x = GRID_HOLE_1_X + gridSpacingHorizontal + GRID_CELL_SPACING + (par < 10 ? HALF_CHARACTER : 0);
			const uint8_t y = GRID_PAR_Y;

			font4x6.setTextColor(BLACK);
			font4x6.setCursor(x, y);
			font4x6.print(par);
			font4x6.setTextColor(WHITE);
		}
	}

	for(uint8_t i = 0; i < 4; ++i)
	{
		if (game.numberOfPlayers > i)
		{
			const uint8_t j = i + 1;
			const uint8_t gridSpacingVertical = (j * GRID_VERT_SPACING);
			const uint8_t gridSpacingHorizontal = (j * GRID_HORZ_SPACING);
		  
			font4x6.setCursor(GRID_TOTAL_X + GRID_CELL_SPACING, GRID_PAR_Y + gridSpacingVertical);
			font4x6.print(game.total.playerScore[i]);

			for(uint8_t j = 0; j < 3; ++j)
			{
				const uint8_t k = 3 - j;
				const uint8_t holeIndex = game.currentHoleNumber - k;
				const uint8_t x = GRID_HOLE_1_X + gridSpacingHorizontal + GRID_CELL_SPACING + (game.holes[holeIndex].playerScore[i] < 10 ? HALF_CHARACTER : 0);
				const uint8_t y = GRID_PAR_Y + gridSpacingVertical;

				font4x6.setCursor(x, y);
				font4x6.print(game.holes[holeIndex].playerScore[i]);  
			}
		}
	}

  switch (game.cursor.mode)
  {

    case CursorMode::Navigation: // Move cursor around ..      
      arduboy.drawRect(GRID_HOLE_1_X + (game.cursor.x * GRID_HORZ_SPACING) + 1, GRID_PAR_Y + (1 * GRID_VERT_SPACING) + ((game.cursor.y - 1) * GRID_VERT_SPACING) - 1, (GRID_HORZ_SPACING - 1), (GRID_VERT_SPACING - 1), (game.cursor.y == 0 ? BLACK : WHITE));

      if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.isFrameCount(KEY_REPEAT_DELAY, frameCount))
	  {
        if (arduboy.pressed(UP_BUTTON) && game.cursor.y > 0)
		{
          game.cursor.y--;
          game.saveEEPROM();
          frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
        }

        if (arduboy.pressed(DOWN_BUTTON) && game.cursor.y < game.numberOfPlayers)
		{
          game.cursor.y++;
          game.saveEEPROM();
          frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
        }

        if (arduboy.pressed(LEFT_BUTTON))
		{
          if (game.cursor.x > 0) {
            game.cursor.x--;
            game.saveEEPROM();
            frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
          }
          else
          {
            if (game.currentHoleNumber > 3)
			{
              game.currentHoleNumber--;
              game.saveEEPROM();
              frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
            }
            else
            {
              game.setState(GameState::NumberOfHoles);
              frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
            }
          }
        }

        if (arduboy.pressed(RIGHT_BUTTON))
        {
          if (game.cursor.x < 2)
		  {
            game.cursor.x++;
            game.saveEEPROM();
            frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
          }
          else
          {
            if (game.currentHoleNumber < game.numberOfHoles)
            {
              game.currentHoleNumber++;
              game.saveEEPROM();
              frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
            }
            else
            {
              game.setState(GameState::FinalScore);
              frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
            }
          }
        }
      }

      if (arduboy.justPressed(A_BUTTON))
	  {
        game.cursor.mode = CursorMode::Edit;
        game.saveEEPROM();
      }

      if (arduboy.justPressed(B_BUTTON))
	  {
        game.setState(GameState::NumberOfHoles);
      }

      break;

    case CursorMode::Edit: // Change score ..
      if (flashCursor())
	  {
        arduboy.drawLine(GRID_HOLE_1_X + (game.cursor.x * GRID_HORZ_SPACING) + 2, GRID_PAR_Y + (1 * GRID_VERT_SPACING) + (game.cursor.y * GRID_VERT_SPACING) - 3, GRID_HOLE_1_X + ((game.cursor.x + 1) * GRID_HORZ_SPACING) - 2, GRID_PAR_Y + (1 * GRID_VERT_SPACING) + (game.cursor.y * GRID_VERT_SPACING) - 3, (game.cursor.y == 0 ? BLACK : WHITE));
      }

      // Hole Par ..
     
      if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.isFrameCount(KEY_REPEAT_DELAY, frameCount))
	  {
        if (arduboy.pressed(UP_BUTTON) && game.cursor.y == 0)
        {
          uint8_t par = game.holes[game.currentHoleNumber - (3 - game.cursor.x)].par;
          if (par <= 9) {
            ++game.holes[game.currentHoleNumber - (3 - game.cursor.x)].par;
            ++game.total.par;
            game.saveEEPROM();
          }
          frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
        }

        if (arduboy.pressed(DOWN_BUTTON) && game.cursor.y == 0)
        {
          uint8_t par = game.holes[game.currentHoleNumber - (3 - game.cursor.x)].par;
          if (par > 1) {
            --game.holes[game.currentHoleNumber - (3 - game.cursor.x)].par;
            --game.total.par;
            game.saveEEPROM();
          }
          frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
        }

        // Player scores ..

        if (arduboy.pressed(UP_BUTTON) && game.cursor.y > 0)
        {
          uint8_t playerScore = game.holes[game.currentHoleNumber - (3 - game.cursor.x)].getPlayerScore(game.cursor.y - 1);
          uint8_t totalScore = game.total.getPlayerScore(game.cursor.y - 1);

          if (playerScore <= 9) {
            game.holes[game.currentHoleNumber - (3 - game.cursor.x)].setPlayerScore(game.cursor.y - 1, playerScore + 1);
            game.total.setPlayerScore(game.cursor.y - 1, totalScore + 1);
            game.saveEEPROM();
          }
          frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
        }

        if (arduboy.pressed(DOWN_BUTTON) && game.cursor.y > 0)
        {
          uint8_t playerScore = game.holes[game.currentHoleNumber - (3 - game.cursor.x)].getPlayerScore(game.cursor.y - 1);
          uint8_t totalScore = game.total.getPlayerScore(game.cursor.y - 1);

          if (playerScore > 0)
          {            
            game.holes[game.currentHoleNumber - (3 - game.cursor.x)].setPlayerScore(game.cursor.y - 1, playerScore - 1);
            game.total.setPlayerScore(game.cursor.y - 1, totalScore - 1);
            game.saveEEPROM();
          }
          frameCount = arduboy.getFrameCount(KEY_REPEAT_DELAY);
        }
      }
      
      if (arduboy.justPressed(A_BUTTON))
	  {
        game.cursor.mode = CursorMode::Navigation;
        game.saveEEPROM();
      }
      break;
  }
}

void creditScreen()
{
	scrollingBackground();
	sprite.drawExternalMask(0, 0, Credits, CreditsMask, 0,0);
	if (arduboy.justPressed(A_BUTTON))
		game.setState(GameState::SplashScreen);
}

void finalScoreDisplay()
{
	game.determineWinners();
	scrollingBackground();
	sprite.drawExternalMask(0, 0, finalScore, finalScoreMask, 0,0);

	font4x6.setTextColor(BLACK);
	font4x6.setCursor(2,0);
	font4x6.print(F("Par~for~this~course~is "));
	font4x6.print(game.total.par);
	font4x6.setTextColor(WHITE);
	//font4x6.setCursor(18,7);
	//font4x6.print("\nPlayer	 Score	Par");

	for(uint8_t i = 0; i < 4; ++i)
	{	
		if (game.numberOfPlayers > i)
		{		
			const uint8_t order = game.getOrder(i);
			const uint8_t score = game.getScore(order);
			const int8_t par = static_cast<int8_t>(score) - static_cast<int8_t>(game.total.par);
			const uint8_t y = 26 + (9 * i);
			
			font4x6.setCursor(18, y);
			font4x6.print(game.getName(order));
			font4x6.setCursor(66, y);
			printPaddedNumber(score, 3);
			font4x6.setCursor(97, y);
			font4x6.print(par >= 0 ? ' ' : '-');
			printPaddedNumber(absT(par), 2);
		}
	}

	if (arduboy.justPressed(B_BUTTON))
	game.setState(GameState::InGame);
	
	if (arduboy.justPressed(A_BUTTON))
	game.setState(GameState::SplashScreen);
}

