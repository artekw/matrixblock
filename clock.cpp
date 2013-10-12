#include "HT1632c.h"
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <string.h>
/*

#define DATA 28 // P9_12
#define WR 17 // P9_23
#define CS 16 // P9_15
#define CLK 19 // P9_16
*/
#define BANK 2
#define DATA 5 // P9_12
#define WR 2 // P9_23
#define CS 3 // P9_15
#define CLK 4 // P9_16

void clock(HT1632c *matrix) {
  char buffer[6];
  char minutes[3];

  time_t rawtime;
  struct tm *timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (minutes, 3, "%M", timeinfo); // FIXME!
  strftime (buffer,6,"%H:%M",timeinfo);

  if(strcmp( minutes, "00" )) {
    matrix->cls();
    matrix->text(buffer, 1, 0, 1);
  }
  else {
    matrix->cls();
    matrix->text(buffer, 1, 0, 2);
    matrix->scrolltextxcolor(8, buffer, 3, 9);
  }
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
