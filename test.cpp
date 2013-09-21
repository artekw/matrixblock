#include "HT1632c.h"
#include <stdio.h>
#include <unistd.h>

#define BANK 1
#define DATA 28 // P9_12
#define WR 17 // P9_23
#define CS 16 // P9_15
#define CLK 19 // P9_16

void testMatrix(HT1632c *matrix) {
  matrix->ht1632_clear();
  for (uint8_t i=0; i<32; i++) {
    for (uint8_t j=0; j<16; j++) {
    matrix->ht1632_plot(i,j,1);
    usleep(100000);
    }
  }
  matrix->ht1632_clear();
}

void testMatrix2(HT1632c *matrix) {
  matrix->ht1632_clear();
  for (uint8_t i=0; i<1000; i++) {
    matrix->ht1632_plot(1,1,1);
    usleep(100);
    matrix->ht1632_plot(1,1,2);
    usleep(100);
  }
}

void testMatrix3(HT1632c *matrix) {
  matrix->ht1632_clear();
  matrix->ht1632_putchar(0,0,'a',2);
  usleep(100);
}

void testMatrix4(HT1632c *matrix) {
  matrix->ht1632_clear();
  matrix->scrolltextxcolor(0, "Arduino to minikont",2, 8);
  matrix->text("@bbbl", 0, 8, 1);
}

int main(void) {
  printf("Starting...\n");
  HT1632c matrix = HT1632c(BANK, DATA, WR, CS, CLK);
  matrix.setup();
  matrix.set_brightness(10);


  printf("Test #1\n");
  testMatrix4(&matrix);

  printf("Done!\n");
  return 0;
}
