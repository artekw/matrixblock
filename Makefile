CC=gcc
CCPP=g++

CPPFLAGS=-D_HW_PLATFORM_BEAGLEBONE #-DDEBUG=2
CXXFLAGS=-O0 -g -pedantic -Wall -std=gnu++11
CFLAGS=-pedantic -Wall

TARGETS=daemon \
daemon.opp \
MMAP/mmapgpio.opp \
HT1632c.opp

all: $(TARGETS)

clean:
	clear
	rm -f $(TARGETS)

daemon: daemon.opp MMAP/mmapgpio.opp HT1632c.opp
	$(CCPP) $^ -o $@

daemon.opp: HT1632c.opp
HT1632.opp: MMAP/mmapgpio.opp

%.o: %.c
	$(CC) -c $< -o $@

%.opp: %.cpp
	$(CCPP) -c $< -o $@
