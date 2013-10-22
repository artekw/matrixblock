#!/usr/bin/python2
# -*- coding: utf-8 -*-

import Adafruit_BBIO.GPIO as GPIO   # http://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/gpio
import Adafruit_BBIO.ADC as ADC
import numpy as np
from time import sleep

""" Constans """
HT1632_ID_CMD = 4   # ID = 100 - Commands
HT1632_ID_RD = 6   # ID = 110 - Read RAM
HT1632_ID_WR = 5   # ID = 101 - Write RAM
HT1632_ID_BITS = (1<<2)   # IDs are 3 bits

HT1632_CMD_SYSDIS = 0x00  # CMD= 0000-0000-x Turn off oscil
HT1632_CMD_SYSON = 0x01  # CMD= 0000-0001-x Enable system oscil
HT1632_CMD_LEDOFF = 0x02  # CMD= 0000-0010-x LED duty cycle gen off
HT1632_CMD_LEDON = 0x03  # CMD= 0000-0011-x LEDs ON
HT1632_CMD_BLOFF = 0x08  # CMD= 0000-1000-x Blink ON
HT1632_CMD_BLON = 0x09  # CMD= 0000-1001-x Blink Off
HT1632_CMD_SLVMD = 0x10  # CMD= 0001-00xx-x Slave Mode
HT1632_CMD_MSTMD = 0x14  # CMD= 0001-01xx-x Master Mode
HT1632_CMD_RCCLK = 0x18  # CMD= 0001-10xx-x Use on-chip clock
HT1632_CMD_EXTCLK = 0x1C  # CMD= 0001-11xx-x Use external clock
HT1632_CMD_COMS00 = 0x20  # CMD= 0010-ABxx-x commons options
HT1632_CMD_COMS01 = 0x24  # CMD= 0010-ABxx-x commons options
HT1632_CMD_COMS10 = 0x28  # CMD= 0010-ABxx-x commons options
HT1632_CMD_COMS11 = 0x2C  # CMD= 0010-ABxx-x commons options
HT1632_CMD_PWM = 0xA0  # CMD= 101x-PPPP-x PWM duty cycle
HT1632_CMD_BITS = (1<<7)

BLACK = 0
GREEN = 1
RED = 2
ORANGE = 3

FONT = np.array([
  [0x00, 0x00, 0x00, 0x00, 0x00, 0], #   space
  [0x00, 0x00, 0x7d, 0x00, 0x00, 0], # ! Exclaimation Point
  [0x00, 0x70, 0x00, 0x70, 0x00, 0], # " Double Quotes
  [0x14, 0x7f, 0x14, 0x7f, 0x14, 0], # # Pound
  [0x12, 0x2a, 0x7f, 0x2a, 0x24, 0], # $ Dollar Sign
  [0x62, 0x64, 0x08, 0x13, 0x23, 0], # % Percent
  [0x36, 0x49, 0x55, 0x22, 0x05, 0], # & Ampersand
  [0x00, 0x50, 0x60, 0x00, 0x00, 0], # ' Right Single Quote
  [0x00, 0x1c, 0x22, 0x41, 0x00, 0], # ( Left Parenthesis
  [0x00, 0x41, 0x22, 0x1c, 0x00, 0], # ) Right Parenthesis
  [0x08, 0x2a, 0x1c, 0x2a, 0x08, 0], # * Asterisk
  [0x08, 0x08, 0x3e, 0x08, 0x08, 0], # + Plus
  [0x00, 0x05, 0x06, 0x00, 0x00, 0], # , Comma
  [0x08, 0x08, 0x08, 0x08, 0x08, 0], # - Minus
  [0x00, 0x03, 0x03, 0x00, 0x00, 0], # . Period
  [0x02, 0x04, 0x08, 0x10, 0x20, 0], #  Forward Slash

  [0x3e, 0x45, 0x49, 0x51, 0x3e, 0], # 0
  [0x00, 0x21, 0x7f, 0x01, 0x00, 0], # 1
  [0x21, 0x43, 0x45, 0x49, 0x31, 0], # 2
  [0x42, 0x41, 0x51, 0x69, 0x46, 0], # 3
  [0x0c, 0x14, 0x24, 0x7f, 0x04, 0], # 4
  [0x72, 0x51, 0x51, 0x51, 0x4e, 0], # 5
  [0x1e, 0x29, 0x49, 0x49, 0x06, 0], # 6
  [0x40, 0x47, 0x48, 0x50, 0x60, 0], # 7
  [0x36, 0x49, 0x49, 0x49, 0x36, 0], # 8
  [0x30, 0x49, 0x49, 0x4a, 0x3c, 0], # 9

  [0x00, 0x36, 0x36, 0x00, 0x00, 0], # : Colon
  [0x00, 0x35, 0x36, 0x00, 0x00, 0], # ; SemiColon
  [0x00, 0x08, 0x14, 0x22, 0x41, 0], # < Less Than
  [0x14, 0x14, 0x14, 0x14, 0x14, 0], # = Equal
  [0x41, 0x22, 0x14, 0x08, 0x00, 0], # > Greater Than
  [0x20, 0x40, 0x45, 0x48, 0x30, 0], # ? Question Mark
  [0x26, 0x49, 0x4f, 0x41, 0x3e, 0], # @ At

  [0x3f, 0x48, 0x48, 0x48, 0x3f, 0], # A
  [0x7f, 0x49, 0x49, 0x49, 0x36, 0], # B
  [0x3e, 0x41, 0x41, 0x41, 0x22, 0], # C
  [0x7f, 0x41, 0x41, 0x22, 0x1c, 0], # D
  [0x7f, 0x49, 0x49, 0x49, 0x41, 0], # E
  [0x7f, 0x48, 0x48, 0x48, 0x40, 0], # F
  [0x3e, 0x41, 0x41, 0x49, 0x2e, 0], # G
  [0x7f, 0x08, 0x08, 0x08, 0x7f, 0], # H
  [0x00, 0x41, 0x7f, 0x41, 0x00, 0], # I
  [0x02, 0x01, 0x41, 0x7e, 0x40, 0], # J
  [0x7f, 0x08, 0x14, 0x22, 0x41, 0], # K
  [0x7f, 0x01, 0x01, 0x01, 0x01, 0], # L
  [0x7f, 0x20, 0x10, 0x20, 0x7f, 0], # M
  [0x7f, 0x10, 0x08, 0x04, 0x7f, 0], # N
  [0x3e, 0x41, 0x41, 0x41, 0x3e, 0], # O
  [0x7f, 0x48, 0x48, 0x48, 0x30, 0], # P
  [0x3e, 0x41, 0x45, 0x42, 0x3d, 0], # Q
  [0x7f, 0x48, 0x4c, 0x4a, 0x31, 0], # R
  [0x31, 0x49, 0x49, 0x49, 0x46, 0], # S
  [0x40, 0x40, 0x7f, 0x40, 0x40, 0], # T
  [0x7e, 0x01, 0x01, 0x01, 0x7e, 0], # U
  [0x7c, 0x02, 0x01, 0x02, 0x7c, 0], # V
  [0x7f, 0x02, 0x04, 0x02, 0x7f, 0], # W
  [0x63, 0x14, 0x08, 0x14, 0x63, 0], # X
  [0x60, 0x10, 0x0f, 0x10, 0x60, 0], # Y
  [0x43, 0x45, 0x49, 0x51, 0x61, 0], # Z

  [0x00, 0x00, 0x7f, 0x41, 0x41, 0], # [ Left Square Bracket
  [0x20, 0x10, 0x08, 0x04, 0x02, 0], # \ Back Slash
  [0x41, 0x41, 0x7f, 0x00, 0x00, 0], # ] Right Square Bracket
  [0x10, 0x20, 0x40, 0x20, 0x10, 0], # ^ Power
  [0x01, 0x01, 0x01, 0x01, 0x01, 0], # _ Underscore
  [0x00, 0x40, 0x20, 0x10, 0x00, 0], # ` Accent

  [0x02, 0x15, 0x15, 0x15, 0x0f, 0], # a
  [0x7f, 0x05, 0x09, 0x09, 0x06, 0], # b
  [0x0e, 0x11, 0x11, 0x11, 0x02, 0], # c
  [0x06, 0x09, 0x09, 0x05, 0x7f, 0], # d
  [0x0e, 0x15, 0x15, 0x15, 0x0c, 0], # e
  [0x08, 0x3f, 0x48, 0x40, 0x20, 0], # f
  [0x08, 0x14, 0x15, 0x15, 0x1e, 0], # g
  [0x7f, 0x04, 0x08, 0x08, 0x07, 0], # h
  [0x00, 0x01, 0x2f, 0x01, 0x00, 0], # i
  [0x02, 0x01, 0x11, 0x5e, 0x00, 0], # j
  [0x7f, 0x04, 0x0a, 0x11, 0x00, 0], # k
  [0x00, 0x41, 0x7f, 0x01, 0x00, 0], # l
  [0x1f, 0x10, 0x0c, 0x10, 0x0f, 0], # m
  [0x1f, 0x08, 0x10, 0x10, 0x0f, 0], # n
  [0x0e, 0x11, 0x11, 0x11, 0x0e, 0], # o
  [0x1f, 0x14, 0x14, 0x14, 0x08, 0], # p
  [0x08, 0x14, 0x14, 0x0c, 0x1f, 0], # q
  [0x1f, 0x08, 0x10, 0x10, 0x08, 0], # r
  [0x09, 0x15, 0x15, 0x15, 0x02, 0], # s
  [0x10, 0x7e, 0x11, 0x01, 0x02, 0], # t
  [0x1e, 0x01, 0x01, 0x02, 0x1f, 0], # u
  [0x1c, 0x02, 0x01, 0x02, 0x1c, 0], # v
  [0x1e, 0x01, 0x02, 0x01, 0x1e, 0], # w
  [0x11, 0x0a, 0x04, 0x0a, 0x11, 0], # x
  [0x18, 0x05, 0x05, 0x05, 0x1e, 0], # y
  [0x11, 0x13, 0x15, 0x19, 0x11, 0], # z

  [0x00, 0x08, 0x36, 0x41, 0x00, 0], # [ Left Braces
  [0x00, 0x00, 0x7f, 0x00, 0x00, 0], # | Pipe
  [0x00, 0x41, 0x36, 0x08, 0x00, 0]])

class HT1632c(object):
    def __init__(self, data, write, cs, clk, a=1, debug=False):

        self.data = data
        self.write = write
        self.cs = cs
        self.clk = clk
        self.numdisp = a
        self.chip_max = 4 * self.numdisp

        self.y_max = 16
        self.x_max = 32*self.numdisp

        self.shadowram = np.zeros((64, 8), dtype=int)

        GPIO.setup(self.data, GPIO.OUT)
        GPIO.setup(self.write, GPIO.OUT)
        GPIO.setup(self.cs, GPIO.OUT)
        GPIO.setup(self.clk, GPIO.OUT)

        ADC.setup()

        self.debug = debug

        #GPIO.cleanup()
        if self.debug:
            print ("GPIOs exported!")

        self.setup()


    def calcBit(self, y):
        return (8>>(y&3))


    def outputA_74164_pulse(self):
        GPIO.output(self.clk, GPIO.HIGH)
        GPIO.output(self.clk, GPIO.LOW)


    def outputA_74164(self, x):
        GPIO.output(self.cs, GPIO.HIGH) if x==1 else GPIO.output(self.cs, GPIO.LOW)

    def chipselect(self, s):
        if s < 0:
            self.outputA_74164(0)
            for c in xrange(0, self.chip_max): self.outputA_74164_pulse()

        elif s == 0:
            self.outputA_74164(1)
            for c in xrange(0, self.chip_max): self.outputA_74164_pulse()
        else:
            self.outputA_74164(0)
            self.outputA_74164_pulse()
            self.outputA_74164(1)
            for c in xrange(1, s): self.outputA_74164_pulse()


    def writebits(self, bits, firstbit):
        while firstbit:
            GPIO.output(self.write, GPIO.LOW)
            GPIO.output(self.data, GPIO.HIGH) if (bits & firstbit) else GPIO.output(self.data, GPIO.LOW)
            GPIO.output(self.write, GPIO.HIGH)
            firstbit >>= 1


    def sendcmd(self, chipno, command):
        self.chipselect(chipno)
        self.writebits(HT1632_ID_CMD, 1<<2)
        self.writebits(command, 1<<7)
        self.writebits(0, 1)
        self.chipselect(0)


    def senddata(self, chipno, address, data):
        self.chipselect(chipno)
        self.writebits(HT1632_ID_WR, 1<<2)
        self.writebits(address, 1<<6)
        self.writebits(data, 1<<3)
        self.chipselect(0)


    def setup(self):
        GPIO.output(self.cs, GPIO.HIGH)

        for i in xrange(1, self.chip_max + 1):
            self.sendcmd(i, HT1632_CMD_SYSDIS)
            self.sendcmd(i, HT1632_CMD_SYSON)
            self.sendcmd(i, HT1632_CMD_LEDON)
            #self.sendcmd(i, HT1632_CMD_BLOFF)
            self.sendcmd(i, HT1632_CMD_MSTMD)
            self.sendcmd(i, HT1632_CMD_RCCLK)
            self.sendcmd(i, HT1632_CMD_COMS00)
            self.sendcmd(i, HT1632_CMD_PWM)

            for j in xrange(0, 96): self.senddata(i, j, 0)


    def set_brightness(self, level):
      if level > 15:
        level = 15
      # automatic regulation of brightness
      if level == -1:
        num = []
        for i in xrange(0,5):
          value = ADC.read("AIN0")
          num.append(round(value * 16, 0))
          if self.debug:
            print num
        for i in xrange(0, 5):
          self.sendcmd(i, HT1632_CMD_PWM | int(reduce(lambda x, y: x + y, num) / len(num)))
      # manual
      else:
        for i in xrange(0, 5):
          self.sendcmd(i, HT1632_CMD_PWM | level)


    def xyToIndex(self, x, y):
        x = x % 16
        y = y % 8
        addr = (x<<1) + (y>>2)
        return addr


    def get_pixel(self, x, y):
        nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16 + (2 if y>7 else 0)

        addr = self.xyToIndex(x,y)
        bitval = self.calcBit(y)

        if (self.shadowram[addr, nChip-1] & bitval) and (self.shadowram[addr+32, nChip-1] & bitval):
            return ORANGE
        elif (self.shadowram[addr, nChip-1] & bitval):
            return GREEN
        elif (self.shadowram[addr+32, nChip-1] & bitval):
            return RED
        else:
            return BLACK


    def plot(self, x, y, color):
        if (x<0 or x>self.x_max or y<0 or y>self.y_max):
            return

        if (color != BLACK and color != GREEN and color != RED and color != ORANGE):
            return

        nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16 + (2 if y>7 else 0)

        addr = self.xyToIndex(x,y)
        bitval = self.calcBit(y)

        if self.debug:
            print ("x, y = %s, %s" % (x, y))
            print ("addr = %s" % (addr))
            print ("bitval = %s" % (bitval))

        if color == BLACK:
            if (self.get_pixel(x,y) != BLACK):
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] & ~bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])
                addr = addr + 32;
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] & ~bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])
        elif color == GREEN:
            if (self.get_pixel(x,y) != GREEN):
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] | bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])
                addr = addr + 32
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] & ~bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])
        elif color == RED:
            if (self.get_pixel(x,y) != RED):
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] & ~bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])
                addr = addr + 32
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] | bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])
        elif color == ORANGE:
            if (self.get_pixel(x,y) != ORANGE):
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] | bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])
                addr = addr + 32
                self.shadowram[addr, nChip-1] = self.shadowram[addr, nChip-1] | bitval
                self.senddata(nChip, addr, self.shadowram[addr, nChip-1])


    def clearWithBlack(self):
        for i in xrange(0, 64):
            for j in xrange(0, 16):
                self.plot(i,j, BLACK)


    def clear(self):
        for i in xrange(1, self.chip_max + 1):
            self.chipselect(-1)
            self.writebits(HT1632_ID_WR, 1<<2)
            self.writebits(0, 1<<6)
            for k in xrange(1, 96/2): self.writebits(0, 1<<7)
            self.chipselect(0)
            for j in xrange(0, 64): self.shadowram[j, i] = 0


    def putChar(self, x, y, c, color):
        c = ord(c) - 32 # offset

        for col in xrange(0, 6):
            dots = FONT[c, col]
            for row in xrange(0, 8):
                if (dots & (64>>row)):
                    self.plot(x+col, y+row, color)
                else:
                    self.plot(x+col, y+row, 0)


    def text(self, msg, x, y, color):
        msgl = len(msg)
        for l in xrange(0, msgl):
            self.putChar(x+6*l, y, msg[l], color)

    def scrolltext(self, y, msg, color, delay):
        msgl = len(msg)
        xa = 0
        while xa < 1:
            xpos = self.x_max
            while xpos > (-1 * (6 * msgl)):
                for l in xrange(0, msgl):
                    self.putChar(xpos + (6 * l), y, msg[l], color)
                sleep(0.001 * delay)
                xpos -= 1
            xa = 1



class LEDS(object):
    def __init__(self, red="P9_8", green="P9_7", blue="P9_9"):
        self.red = red
        self.green = green
        self.blue = blue

        GPIO.setup(self.red, GPIO.OUT)
        GPIO.setup(self.green, GPIO.OUT)
        GPIO.setup(self.blue, GPIO.OUT)

    def _on(self, led):
        GPIO.output(led, GPIO.LOW)

    def _off(self,led):
      GPIO.output(led, GPIO.HIGH)