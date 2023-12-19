#ifndef SPEDENSPELIT_H
#define SPEDENSPELIT_H
#include <arduino.h>


//initializeTimer() subroutine initializes Arduino Timer1 module to give interrupts at rate 1Hz
void initializeTimer(void);

//initializeGame() subroutine is used to initialize all variables needed to store randomnumbers and player buttons push data. This function is called from startTheGame() function.
void initializeGame(void);

/* checkGame() subroutine is used to check the status of the Game after each player button press.

If the lates player button press is wrong, the game stops and if the lates press was right, game display is incremented.

Parameters byte lastButtonPress of the player 0 or 1 or 2 or 3 
*/
void checkGame(byte);

//startTheGame() subroutine calls InitializeGame() fucntion and enables Timer1 interrupts to start the Game.
void startTheGame(void);

void endGame(void);

#endif

