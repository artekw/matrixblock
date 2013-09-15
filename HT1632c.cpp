#include "HT1632c.h"
//#include "fonts.c"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "BoneHeader/BoneHeader.h"

// our own copy of the "video" memory; 64 bytes for each of the 4 screen quarters;
// each 64-element array maps 2 planes:
// indexes from 0 to 31 are allocated for green plane;
// indexes from 32 to 63 are allocated for red plane;
// when a bit is 1 in both planes, it is displayed as orange (green + red);
unsigned char ht1632_shadowram[64][8] = {0};

// constructor:
// assign pins, one single screen
HT1632c::HT1632c(uint8_t bank, uint8_t da, uint8_t wr, uint8_t cs, uint8_t cl)
{
      ht1632_data = da;  // Data pin (pin 7 of display connector)
      ht1632_wrclk = wr; // Write clock pin (pin 5 of display connector)
      ht1632_cs = cs;    // Chip Select (pin 1 of display connnector)
      ht1632_clk = cl;   // clock pin (pin 2 of display connector)
      Number_of_Displays = 1;
      CHIP_MAX =  4*Number_of_Displays; // Four HT1632Cs on one board
      X_MAX = 32*Number_of_Displays;
      Y_MAX = 16;

      export_gpio(bank * 32 + ht1632_data);
      set_gpio_direction(bank * 32 + ht1632_data, "out");
      export_gpio(bank * 32 + ht1632_wrclk);
      set_gpio_direction(bank * 32 + ht1632_wrclk, "out");
      export_gpio(bank * 32 + ht1632_cs);
      set_gpio_direction(bank * 32 + ht1632_cs, "out");
      export_gpio(bank * 32 + ht1632_clk);
      set_gpio_direction(bank * 32 + ht1632_clk, "out");

      gpiobank = new GPIO_MMAP(bank);

      if (ht1632_cs > 0) {
        gpiobank->write(ht1632_cs, 1);
      }
      if (ht1632_wrclk > 0) {
        gpiobank->write(ht1632_wrclk, 1);
      }
      if (ht1632_data > 0) {
        gpiobank->write(ht1632_data, 1);
      }
      if (ht1632_clk > 0) {
        gpiobank->write(ht1632_clk, 1);
      }

      ts.tv_sec = 0;
      ts.tv_nsec = DELAY;
}

// constructor:
// assign pins, multiple single screen
HT1632c::HT1632c(uint8_t bank, uint8_t a, uint8_t da, uint8_t wr, uint8_t cs, uint8_t cl)
{
      ht1632_data = da;  // Data pin (pin 7 of display connector)
      ht1632_wrclk = wr; // Write clock pin (pin 5 of display connector)
      ht1632_cs = cs;    // Chip Select (pin 1 of display connnector)
      ht1632_clk = cl;   // clock pin (pin 2 of display connector)
      Number_of_Displays = a;
      CHIP_MAX =  4*Number_of_Displays; // Four HT1632Cs on one board
      X_MAX = 32*Number_of_Displays;
      Y_MAX = 16;

      export_gpio(bank * 32 + ht1632_data);
      set_gpio_direction(bank * 32 + ht1632_data, "out");
      export_gpio(bank * 32 + ht1632_wrclk);
      set_gpio_direction(bank * 32 + ht1632_wrclk, "out");
      export_gpio(bank * 32 + ht1632_cs);
      set_gpio_direction(bank * 32 + ht1632_cs, "out");
      export_gpio(bank * 32 + ht1632_clk);
      set_gpio_direction(bank * 32 + ht1632_clk, "out");

      gpiobank = new GPIO_MMAP(bank);

      if (ht1632_cs > 0) {
        gpiobank->write(ht1632_cs, 1);
      }
      if (ht1632_wrclk > 0) {
        gpiobank->write(ht1632_wrclk, 1);
      }
      if (ht1632_data > 0) {
        gpiobank->write(ht1632_data, 1);
      }
      if (ht1632_clk > 0) {
        gpiobank->write(ht1632_clk, 1);
      }

      ts.tv_sec = 0;
      ts.tv_nsec = DELAY;
}

//**************************************************************************************************
//Function Name: OutputCLK_Pulse
//Function Feature: enable CLK_74164 pin to output a clock pulse
//Input Argument: void
//Output Argument: void
//**************************************************************************************************
void HT1632c::OutputCLK_Pulse(void) //Output a clock pulse
{
  gpiobank->write(ht1632_clk, 1);
  gpiobank->write(ht1632_clk, 0);
}


//**************************************************************************************************
//Function Name: OutputA_74164
//Function Feature: enable pin A of 74164 to output 0 or 1
//Input Argument: x: if x=1, 74164 outputs 1. If x?1, 74164 outputs 0.
//Output Argument: void
//**************************************************************************************************
void HT1632c::OutputA_74164(uint8_t x) //Input a digital level to 74164
{
  gpiobank->write(ht1632_cs, (x==1 ? 1 : 0));
}


//**************************************************************************************************
//Function Name: ChipSelect
//Function Feature: enable HT1632C
//Input Argument: select: HT1632C to be selected
// If select=0, select none.
// If s<0, select all.
//Output Argument: void
//**************************************************************************************************
void HT1632c::ChipSelect(int select)
{
  unsigned char tmp = 0;
  if(select<0) //Enable all HT1632Cs
  {
    OutputA_74164(0);
    CLK_DELAY;
    for(tmp=0; tmp<CHIP_MAX; tmp++)
    {
      OutputCLK_Pulse();
    }
  }
  else if(select==0) //Disable all HT1632Cs
  {
    OutputA_74164(1);
    CLK_DELAY;
    for(tmp=0; tmp<CHIP_MAX; tmp++)
    {
      OutputCLK_Pulse();
    }
  }
  else
  {
    OutputA_74164(1);
    CLK_DELAY;
    for(tmp=0; tmp<CHIP_MAX; tmp++)
    {
      OutputCLK_Pulse();
    }
    OutputA_74164(0);
    CLK_DELAY;
    OutputCLK_Pulse();
    CLK_DELAY;
    OutputA_74164(1);
    CLK_DELAY;
    tmp = 1;
    for( ; tmp<select; tmp++)
    {
      OutputCLK_Pulse();
    }
  }
}

/*
 * ht1632_writebits
 * Write bits (up to 8) to h1632 on pins ht1632_data, ht1632_wrclk
 * Chip is assumed to already be chip-selected
 * Bits are shifted out from MSB to LSB, with the first bit sent
 * being (bits & firstbit), shifted till firsbit is zero.
 */
void HT1632c::ht1632_writebits (uint8_t bits, uint8_t firstbit)
{
  //printf(" ");
  while (firstbit) {
    //printf((bits&firstbit ? "1" : "0"));
    gpiobank->write(ht1632_wrclk, 0);
    if (bits & firstbit) {
      gpiobank->write(ht1632_data, 1);
    }
    else {
      gpiobank->write(ht1632_data, 0);
    }
    gpiobank->write(ht1632_wrclk, 1);
    firstbit >>= 1;
  }
}

/*
 * ht1632_sendcmd
 * Send a command to the ht1632 chip.
 */
void HT1632c::ht1632_sendcmd (uint8_t chipNo, uint8_t command)
{
  ChipSelect(chipNo);
  ht1632_writebits(HT1632_ID_CMD, 1<<2);  // send 3 bits of id: COMMMAND
  ht1632_writebits(command, 1<<7);  // send the actual command
  ht1632_writebits(0, 1);   /* one extra dont-care bit in commands. */
  ChipSelect(0);
}

/*
 * ht1632_senddata
 * send a nibble (4 bits) of data to a particular memory location of the
 * ht1632.  The command has 3 bit ID, 7 bits of address, and 4 bits of data.
 *    Select 1 0 1 A6 A5 A4 A3 A2 A1 A0 D0 D1 D2 D3 Free
 * Note that the address is sent MSB first, while the data is sent LSB first!
 * This means that somewhere a bit reversal will have to be done to get
 * zero-based addressing of words and dots within words.
 */
void HT1632c::ht1632_senddata (uint8_t chipNo, uint8_t address, uint8_t data)
{
  ChipSelect(chipNo);
  ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
  ht1632_writebits(address, 1<<6); // Send address
  ht1632_writebits(data, 1<<3); // send 4 bits of data
  ChipSelect(0);
}

void HT1632c::setup()
{
  ht1632_setup();
}

void HT1632c::ht1632_setup()
{
  gpiobank->write(ht1632_cs, 1);  /* unselect (active 0) */

  for (int j=1; j<=CHIP_MAX; j++)
  {
    ht1632_sendcmd(j, HT1632_CMD_SYSDIS);  // Disable system
    ht1632_sendcmd(j, HT1632_CMD_COMS00);
    //ht1632_sendcmd(j, HT1632_CMD_BLOFF);
    ht1632_sendcmd(j, HT1632_CMD_MSTMD);  /* Master Mode */
    ht1632_sendcmd(j, HT1632_CMD_RCCLK);        // HT1632C
    ht1632_sendcmd(j, HT1632_CMD_SYSON);  /* System on */
    ht1632_sendcmd(j, HT1632_CMD_LEDON);  /* LEDs on */
    ht1632_sendcmd(j, HT1632_CMD_PWM);

    for (uint8_t i=0; i<96; i++)
    {
      ht1632_senddata(j, i, 0);  // clear the display!
    }
  }
  //usleep(10000);
}

//**************************************************************************************************
//Function Name: xyToIndex
//Function Feature: get the value of x,y
//Input Argument: x: X coordinate
//                y: Y coordinate
//Output Argument: address of xy
//**************************************************************************************************
uint8_t HT1632c::xyToIndex(uint8_t x, uint8_t y)
{
  uint8_t nChip, addr;

//  DC: Fixed to work with multiple screens
    nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16 + (y>7?2:0);

//  if (x>=32) {
//    nChip = 3 + x/16 + (y>7?2:0);
//  } else {
//    nChip = 1 + x/16 + (y>7?2:0);
//  }

  x = x % 16;
  y = y % 8;
  addr = (x<<1) + (y>>2);

  return addr;
}

//**************************************************************************************************
//Function Name: get_pixel
//Function Feature: get the value of x,y
//Input Argument: x: X coordinate
//                y: Y coordinate
//Output Argument: color setted on x,y coordinates
//**************************************************************************************************
int HT1632c::get_pixel(uint8_t x, uint8_t y) {
  uint8_t addr, bitval, nChip;

//  DC: Fixed to work with multiple screens
    nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16 + (y>7?2:0);

//  if (x>=32) {
//    nChip = 3 + x/16 + (y>7?2:0);
//  } else {
//    nChip = 1 + x/16 + (y>7?2:0);
//  }

  addr = xyToIndex(x,y);
  bitval = calcBit(y);

  if ((ht1632_shadowram[addr][nChip-1] & bitval) && (ht1632_shadowram[addr+32][nChip-1] & bitval)) {
    return ORANGE;
  } else if (ht1632_shadowram[addr][nChip-1] & bitval) {
    return GREEN;
  } else if (ht1632_shadowram[addr+32][nChip-1] & bitval) {
    return RED;
  } else {
    return 0;
  }
}

void HT1632c::set_brightness (uint8_t pwm) {
  if (pwm > 15) {
    pwm = 15;
  }
  for (int i = 0; i < 5; i++) {
    ht1632_sendcmd(i, HT1632_CMD_PWM | pwm);
  }
}

/*
 * plot a point on the display, with the upper left hand corner
 * being (0,0), and the 0er right hand corner being (31, 15);
 * parameter "color" could have one of the 4 values:
 * black (off), red, green or yel0;
 */
//void ht1632_plot (uint8_t x, uint8_t y, uint8_t color)
void HT1632c::ht1632_plot (uint8_t x, uint8_t y, uint8_t color)
{
   uint8_t nChip, addr, bitval;

  if (x<0 || x>X_MAX || y<0 || y>Y_MAX)
    return;

  if (color != BLACK && color != GREEN && color != RED && color != ORANGE)
    return;

//  DC: Fixed to work with multiple screens
    nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16 + (y>7?2:0);

  addr = xyToIndex(x,y);
  bitval = calcBit(y);

  switch (color)
  {
    case BLACK:
      if (get_pixel(x,y) != BLACK) { // compare with memory to only set if pixel is other color
        // clear the bit in both planes;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
    case GREEN:
      if (get_pixel(x,y) != GREEN) { // compare with memory to only set if pixel is other color
        // set the bit in the green plane and clear the bit in the red plane;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
    case RED:
      if (get_pixel(x,y) != RED) { // compare with memory to only set if pixel is other color
        // clear the bit in green plane and set the bit in the red plane;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
    case ORANGE:
      if (get_pixel(x,y) != ORANGE) { // compare with memory to only set if pixel is other color
        // set the bit in both the green and red planes;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
  }
}


/*
 * ht1632_clear
 * clear the display, and the shadow memory, and the snapshot
 * memory.  This uses the "write multiple words" capability of
 * the chipset by writing all 96 words of memory without raising
 * the chipselect signal.
 */
void HT1632c::ht1632_clear()
{
  char i;
  for (int i=1; i<=CHIP_MAX; i++)
  {
    ChipSelect(-1);
    ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
    ht1632_writebits(0, 1<<6); // Send address
    for (i = 0; i < 96/2; i++) // Clear entire display
      ht1632_writebits(0, 1<<7); // send 8 bits of data
    ChipSelect(0);

    for (int j=0; j < 64; j++)
      ht1632_shadowram[j][i] = 0;
  }
}

void HT1632c::cls() {
  for(int i = 0; i < 64; i++)
    for(int j = 0; j < 16; j++)
      plot(i, j, BLACK);
}
/*
void HT1632c::putChar(char c, int x, int y, uint8_t color) {
  ht1632_putchar(x, y, c, color);
}
*/
/*
//void ht1632_putchar(uint8_t x, uint8_t y, char c, uint8_t color=GREEN)
void HT1632c::ht1632_putchar(int x, int y, char c, uint8_t color)
{
  uint8_t dots;
  //if (c >= 'A' && c <= 'Z' ||
  //  (c >= 'a' && c <= 'z') ) {
  //  c &= 0x1F;   // A-Z maps to 1-26
  //}
  //else if (c >= '0' && c <= '9') {
  //  c = (c - '0') + 27;
  //}
  //else if (c == ' ') {
  //  c = 0; // space
  //}

  if (c == ' ') {c = 0;}
  else if (c == '!') {c = 1;}
  else if (c == '"') {c = 2;}
  else if (c == '#') {c = 3;}
  else if (c == '$') {c = 4;}
  else if (c == '%') {c = 5;}
  else if (c == '&') {c = 6;}
  //else if (c == ''') {c = 7;}
  else if (c == '(') {c = 8;}
  else if (c == ')') {c = 9;}
  else if (c == '*') {c = 10;}
  else if (c == '+') {c = 11;}
  else if (c == ',') {c = 12;}
  else if (c == '-') {c = 13;}
  else if (c == '.') {c = 14;}
  else if (c == '/') {c = 15;}

  else if (c >= '0' && c <= '9') {
    c = (c - '0') + 16;
  }

  else if (c == ':') {c = 26;}
  else if (c == ';') {c = 27;}
  else if (c == '<') {c = 28;}
  else if (c == '=') {c = 29;}
  else if (c == '>') {c = 30;}
  else if (c == '?') {c = 31;}
  else if (c == '@') {c = 32;}

  else if (c >= 'A' && c <= 'Z') {
    c = (c - 'A') + 33;
  }

  else if (c == '[') {c = 59;}
  //else if (c == '\') {c = 60;}
  else if (c == ']') {c = 61;}
  else if (c == '^') {c = 62;}
  else if (c == '_') {c = 63;}
  else if (c == '`') {c = 64;}

  else if (c >= 'a' && c <= 'z') {
    c = (c - 'a') + 65;
  }

  else if (c == '{') {c = 91;}
  else if (c == '|') {c = 92;}
  else if (c == '}') {c = 93;}

  for (char col=0; col< 6; col++) {
    dots = &font[c][col];
    for (char row=0; row < 7; row++) {
      if (dots & (64>>row))          // only 7 rows.
        plot(x+col, y+row, color);
      else
        plot(x+col, y+row, 0);
    }
  }
}

*/
/*
 * Copy a character glyph from the myfont data structure to
 * display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses plot() to draw each dot.
 * Slightly adopted for using fonts with more rows then 8
 *  ht1632_putcharsizecolor(x location, y location , char ,
 *  size as integer, colorname (RANDOMCOLOR for random color),
 *  name of the font array,  umber of columns, number of rows, 'G' for Gimp or 'T' for The Dot Factory font arrays)
 */
/*
void HT1632c::ht1632_putcharsizecolor(int x, int y,unsigned char c,  char size, uint8_t color, uint8_t secondcolor, unsigned char fontname[][NCOLUMNS],  int columncountfont, char rowcountfont, char oddeven)
{

  uint8_t dots, dots2,dots3, cc,cc2, cc3, rr, g, t, t3, divisor;
  uint8_t maximumdrawfont, showcolor,showsecondcolor; //128 for the large fonts (=8x8 and 12x8), 64 for all smaller ones
  if  (rowcountfont<=7)
    maximumdrawfont=64;
  else
    maximumdrawfont=128;
  for (uint8_t col=0; col<columncountfont*size ; col++) {
    // Addressing the right 8 lines because 'The Dot Factory' starts from the bottom, all others from top
    if (rowcountfont <=8) {
      cc=col/size;
      dots = pgm_read_byte_near(&fontname[c][cc]);
      divisor=1;
    }
    else if (rowcountfont>8 && rowcountfont <=16){
      if (oddeven=='T'){
        g=0;
        t=1;
      }
      else {
        g=1;
        t=0;
      }

      cc=col/size*2+g;
      cc2=col/size*2+t;
      dots = pgm_read_byte_near(&fontname[c][cc]);
      dots2 = pgm_read_byte_near(&fontname[c][cc2]);
      divisor=2;
    }
    else if (rowcountfont>16 && rowcountfont <=24){
      if (oddeven=='T'){
        g=0;
        t=1;
        t3=2;
      }
      else {
        g=2;
        t=1;
        t3=0;
      }
      divisor=3;
      cc=col/size*divisor+g;
      cc2=col/size*divisor+t;
      cc3=col/size*divisor+t3;
      dots = pgm_read_byte_near(&fontname[c][cc]);
      dots2 = pgm_read_byte_near(&fontname[c][cc2]);
      dots3 = pgm_read_byte_near(&fontname[c][cc3]);

    }
    for (uint8_t row=0; row < rowcountfont/divisor*size; row++) {
      showcolor=color;
      showsecondcolor=secondcolor;

      rr=row/size;
      if (dots & (maximumdrawfont>>rr))
        plot(x+col, y+row, showcolor);
      else
        plot(x+col, y+row, showsecondcolor);
      if (divisor>=2){
        if (dots2 & (maximumdrawfont>>rr))
          plot(x+col, y+rowcountfont/divisor+row, showcolor);
        else
          plot(x+col, y+rowcountfont/divisor+row, showsecondcolor);
      }
      if (divisor>=3){
        if (dots3& (maximumdrawfont>>rr))
          plot(x+col, y+2*rowcountfont/divisor+row, showcolor);
        else
          plot(x+col, y+2*rowcountfont/divisor+row, showsecondcolor);
      }
    }
  }
}
*/


/***********************************************************************
 * Scrolling  functions
 * for scrolling text and bitmaps
 * Please take only fonts with fixed heigth and width, otherwise the
 * chars will be overlapping
 * Original functions by Bill Ho
 ***********************************************************************/

/*
 * scrolltextcolor()
 * Scrolls a text string from left to right
 * Simple function for the original ht1632_putchar method without MULTICOLOR and no background color
 * Original function by Bill Ho
 * scrolltextxcolor(y location, string ,  colorname (RANDOMCOLOR for random color), delaytime in milliseconds)
 */
 /*
void HT1632c::scrolltextxcolor(int y,char Str1[ ], uint8_t color, int delaytime){
  int messageLength = strlen(Str1)+ 1;
  uint8_t showcolor;
  int xa = 0;
  while (xa<1) {
    int xpos = X_MAX;
    while (xpos > (-1 * ( 6*messageLength))) {
      for (int i = 0; i < messageLength; i++) {
        showcolor=color;
        ht1632_putchar(xpos + (6* i),  y,Str1[i],showcolor);
      }
      sleep(delaytime);// reduce speed of scroll
      xpos--;
    }
    xa =1;
  }
}

// basic text function, no scroll
void HT1632c::text(char str[], int x, int y, int color) {
  int m = strlen(str)+ 1;
  for (int i=0; i < m; i++) {
    ht1632_putchar(x+6*i, y, str[i], color);
  }
}

// basic text function, no scroll
void HT1632c::text(int num, int x, int y, int color) {
  int m = numdigits(num);
  for (int i = 0; i < m; i++) {
    int digit = extractDigit(num, m-i-1) + 48;
    ht1632_putchar(x+6*i, y, digit, color);
  }
}

int HT1632c::extractDigit(int number, int place) {
  for(int i = 0; i < place; ++i)
    number /= 10;   //get rid of the preceding digits
  return number % 10;  //now ignore all of the fol0ing ones
}

int HT1632c::numdigits(int n)
{
int count = 0;
do
{
++count;
n /= 10;
}
while(n != 0);
return count;
}

// basic image function, assuming it fits on the screen

*/