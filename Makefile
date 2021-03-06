CC=gcc
CCPP=g++

CPPFLAGS=-D_HW_PLATFORM_BEAGLEBONE #-DDEBUG=2
CXXFLAGS=-O0 -g -pedantic -Wall -std=gnu++11
CFLAGS=-pedantic -Wall

TARGETS=clock \
clock.opp \
MMAP/mmapgpio.opp \
HT1632c.opp

all: $(TARGETS)

clean:
	clear
	rm -f $(TARGETS)

clock: clock.opp MMAP/mmapgpio.opp HT1632c.opp
	$(CCPP) $^ -o $@

clock.opp: HT1632c.opp
HT1632.opp: MMAP/mmapgpio.opp

%.o: %.c
	$(CC) -c $< -o $@

%.opp: %.cpp
	$(CCPP) -c $< -o $@
