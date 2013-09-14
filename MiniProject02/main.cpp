#include "../Resources/adafruit/HT1632/HT1632.h"
#include <stdio.h>
#include <unistd.h>
#include "icons.c"

/* 
This is a basic demo program showing how to write to a HT1632
These can be used for up to 16x24 LED matrix grids, with internal memory
and using only 3 pins - data, write and select.
Multiple HT1632's can share data and write pins, but need unique CS pins.
*/

#define BANK 1
#define DATA 28 //1_28
#define WR 17 //1_17
#define CS 16 //1_16

void testMatrix2(HT1632LEDMatrix *matrix) {
  //Display "A+"
  matrix->setTextSize(2);
  matrix->setTextColor(1);
  matrix->write('A');
  matrix->write('+');
  matrix->writeScreen();
  usleep(2000000);

  // Blink!
  matrix->blink(true);
  usleep(2000000);
  matrix->blink(false);

  // Dim down and then up
  for (int8_t i = 15; i >= 0; i--) {
    matrix->setBrightness(i);
    usleep(100000);
  }
  for (int8_t i = 0; i < 16; i++) {
    matrix->setBrightness(i);
    usleep(100000);
  }

  // Blink again!
  matrix->blink(true);
  usleep(2000000);
  matrix->blink(false);
}

void testMatrix1(HT1632LEDMatrix *matrix) {
  //Display icon
  matrix->drawBitmap(0, 0, icon, matrix->width(), matrix->height(), 1);  
  matrix->writeScreen();
  usleep(2000000);

  // Blink!
  matrix->blink(true);
  usleep(2000000);
  matrix->blink(false);

  // Dim down and then up
  for (int8_t i = 15; i >= 0; i--) {
    matrix->setBrightness(i);
    usleep(100000);
  }
  for (int8_t i = 0; i < 16; i++) {
    matrix->setBrightness(i);
    usleep(100000);
  }

  // Blink again!
  matrix->blink(true);
  usleep(2000000);
  matrix->blink(false);
}

int main(void) {
  printf("Starting...\n");
  HT1632LEDMatrix matrix = HT1632LEDMatrix(BANK, DATA, WR, CS);
  matrix.begin(HT1632_COMMON_16NMOS);
  
  printf("Clear\n");
  matrix.clearScreen();
  
  printf("Test #1\n");
  testMatrix1(&matrix);

  printf("Clear\n");
  matrix.clearScreen();
  
  printf("Test2\n");
  testMatrix2(&matrix);
  
  printf("Done!\n");
  return 0;
}
