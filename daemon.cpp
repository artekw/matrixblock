#include "HT1632c.h"
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

#define BANK 1
#define DATA 28 // P9_12
#define WR 17 // P9_23
#define CS 16 // P9_15
#define CLK 19 // P9_16

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

  clock(&matrix);

  printf("Done!\n");
  return 0;
}
