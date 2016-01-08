#include "../HT1632c.h"
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

//#define BANK 1
//#define DATA 28 // P9_12
//#define WR 17 // P9_23
//#define CS 16 // P9_15
//#define CLK 19 // P9_16

#define BANK 2
#define DATA 5 // P9_17
#define WR 2 // P9_22
#define CS 3 // P9_21
#define CLK 4 // P9_18

void testMatrix1(HT1632c *matrix) {
  matrix->ht1632_clear();
  for (char i=0; i<32; i++) {
    for (char j=0; j<16; j++) {
    matrix->ht1632_plot(i,j,1);
    usleep(100000);
    }
  }
  matrix->ht1632_clear();
}

void testMatrix2(HT1632c *matrix) {
  matrix->ht1632_clear();
  matrix->ht1632_putchar(0,0,'a',2);
  usleep(100);
}

void testMatrix3(HT1632c *matrix) {
  matrix->ht1632_clear();
  matrix->scrolltextxcolor(0, "Test message",1, 12);
  matrix->text("@bbbl", 0, 8, 1);
}

void clock(HT1632c *matrix) {
  char buffer[80];
  time_t rawtime;
  struct tm *timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"%H:%M",timeinfo);
  matrix->text(buffer, 1, 0, 1);
}

int main(int argc, char *argv[]) {
  printf("Starting...\n");
  HT1632c matrix = HT1632c(BANK, DATA, WR, CS, CLK);
  matrix.setup();
  if (argc != 2) {
    matrix.set_brightness(0);
  }
  else {
    matrix.set_brightness(atoi(argv[1]));
  }

  printf("Test #1\n");
  testMatrix1(&matrix);

  printf("Test #2\n");
  testMatrix2(&matrix);

  printf("Test #3\n");
  testMatrix3(&matrix);

  printf("Test #4\n");
  clock(&matrix);

  printf("Done!\n");
  return 0;
}
