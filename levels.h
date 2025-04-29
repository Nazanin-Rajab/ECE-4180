#ifndef LEVELS_H
#define LEVELS_H

#include "hardware_interface.h"
#include "game_constants.h"

// Level 1 functions
void displayDecimalNumber(uint8_t decNum);
void displayBinaryInput(uint8_t value, int timeRemaining);
bool runLevel1();

// Level 2 functions
bool runLevel2();

// Level 3 functions
void spawnEnemy3();
bool checkCollision3();
void drawLevel3();
bool runLevel3();

#endif // LEVELS_H 