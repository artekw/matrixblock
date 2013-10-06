#!/usr/bin/python2
# -*- coding: utf-8 -*-

import Adafruit_BBIO.GPIO as GPIO   # http://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/gpio
import BitArray2D                   # https://pypi.python.org/pypi/BitArray2D/2.1
from BitArray2D import godel

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
REEN = 1
RED = 2
ORANGE = 3

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

        self.shadowram = BitArray2D.BitArray2D( rows = 64, columns = 8 )

        GPIO.setup(self.data, GPIO.OUT)
        GPIO.setup(self.write, GPIO.OUT)
        GPIO.setup(self.cs, GPIO.OUT)
        GPIO.setup(self.clk, GPIO.OUT)

        #GPIO.cleanup()
        if debug:
            print ("GPIOs exported!")

        self.setup()


    def calcBit(self, y):
        return (8>>(y&3))


    def outputA_74164_pulse(self):
        GPIO.output(self.clk, GPIO.HIGH)
        GPIO.output(self.clk, GPIO.LOW)


    def outputA_74164(self, x):
        if x == 1:
            GPIO.output(self.cs, GPIO.HIGH)
        else:
            GPIO.output(self.cs, GPIO.LOW)


    def chipselect(self, s):
        if s < 0:
            self.outputA_74164(0)
            #pass
            for c in xrange(0, self.chip_max):
                self.outputA_74164_pulse()
        elif s == 0:
            self.outputA_74164(1)
            #pass
            for c in xrange(0, self.chip_max):
                self.outputA_74164_pulse()
        else:
            self.outputA_74164(0)
            #pass
            self.outputA_74164_pulse()
            #pass
            self.outputA_74164(1)
            #pass
            for c in xrange(1, s):
                self.outputA_74164_pulse()


    def writebits(self, bits, firstbit):
        while firstbit:
            GPIO.output(self.write, GPIO.LOW)
            if bits & firstbit:
                GPIO.output(self.data, GPIO.HIGH)
            else:
                GPIO.output(self.data, GPIO.LOW)
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

        for i in xrange(1, self.chip_max):
            self.sendcmd(i, HT1632_CMD_SYSDIS)
            self.sendcmd(i, HT1632_CMD_SYSON)
            self.sendcmd(i, HT1632_CMD_LEDON)
            #self.sendcmd(i, HT1632_CMD_BLOFF)
            self.sendcmd(i, HT1632_CMD_MSTMD)
            self.sendcmd(i, HT1632_CMD_RCCLK)
            self.sendcmd(i, HT1632_CMD_COMS00)
            self.sendcmd(i, HT1632_CMD_PWM)

            for j in xrange(0, 96):
                self.senddata(i, j, 0)


    def xyToIndex(self, x, y):
        nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16
        if y > 7:
            nChip = nChip + 2
        else:
            nChip = nChip

        x = x % 16
        y = y % 8
        addr = (x<<1) + (y>>2)

        return addr;


    def get_pixel(self, x, y):
        nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16
        if y > 7:
            nChip = nChip + 2
        else:
            nChip = nChip

        addr = self.xyToIndex(x,y)
        bitval = self.calcBit(y)

        if (self.shadowram[godel(addr, nChip-1)] & bitval) & (self.shadowram[godel(addr+32, nChip-1)] & bitval):
            return 3
        elif (self. shadowram[godel(addr, nChip-1)] & bitval):
            return 1
        elif (self.shadowram[godel(addr+32, nChip-1)] & bitval):
            return 2
        else:
            return 0;


    def plot(self, x, y, color):
        print("plot")
        if (x<0 or x>self.x_max or y<0 or y>self.y_max):
            return;

        #if (color != 0 & color != 1 & color != 2 & color != 3):
            #return;

        nChip = 1 + 4 * (x/32) + (x-32 * (x/32) )/16
        if y > 7:
            nChip = nChip + 2
        else:
            nChip = nChip

        addr = self.xyToIndex(x,y)
        bitval = self.calcBit(y)

        if color == 0:
            print("black")
            if (self.get_pixel(x,y) != 0):

                self.shadowram[[addr], [nChip-1]] &= ~bitval
                self.senddata(nChip, addr, self.shadowram[[addr], [nChip-1]])
                addr = addr + 32;
                self.shadowram[addr, nChip-1] &= ~bitval
                self.senddata(nChip, addr, ht1632_shadowram[[addr], [nChip-1]])
        elif color == 1:
            print("green")
            if (self.get_pixel(x,y) != 1):
                print bitval

                self.shadowram[addr, nChip-1] = self.shadowram[godel(addr, nChip-1)] & bitval
                self.senddata(nChip, addr, self.shadowram[godel(addr, nChip-1)])
                addr = addr + 32
                self.shadowram[addr, nChip-1] = self.shadowram[godel(addr, nChip-1)] | ~bitval
                self.senddata(nChip, addr, self.shadowram[godel(addr, nChip-1)])