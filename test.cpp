#include "HT1632c.h"
#include <stdio.h>
#include <unistd.h>

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
#define CLK 19 // 1_18

void testMatrix(HT1632c *matrix) {
  matrix->ht1632_clear();
  for (uint8_t i=0; i<32; i++) {
    for (uint8_t j=0; j<16; j++) {
    matrix->ht1632_plot(i,j,1);
    usleep(50000);
    }
  }
  //matrix->ht1632_clear();
  //matrix->ht1632_plot(0,1,3);
  //matrix->ht1632_clear();
  //sleep(1);
}

int main(void) {
  printf("Starting...\n");
  HT1632c matrix = HT1632c(BANK, DATA, WR, CS, CLK);
  matrix.setup();
  matrix.set_brightness(5);


  printf("Test #1\n");
  testMatrix(&matrix);

  printf("Done!\n");
  return 0;
}
