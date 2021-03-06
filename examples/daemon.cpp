#include "BoneHeader/BoneHeader.h"
#include "HT1632c.h"
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
#define BANK 1
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
  char buffer[80];
  time_t rawtime;
  struct tm *timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"%H:%M",timeinfo);
  matrix->cls();
  matrix->text(buffer, 1, 0, 2);
}

void adc() {

}

int main(int argc, char *argv[]) {
  /*
  args:
    -p 8080 : port number
    -b 0-15 : brightness level | default 0
    <json string> : message to display on matrix
  */

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
