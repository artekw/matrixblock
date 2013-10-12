CC=gcc
CCPP=g++

CPPFLAGS=-D_HW_PLATFORM_BEAGLEBONE #-DDEBUG=2
CXXFLAGS=-O0 -g -pedantic -Wall -std=gnu++11
CFLAGS=-pedantic -Wall

TARGETS=daemon daemon.opp BoneHeader/BoneHeader.opp MMAP/mmapgpio.opp HT1632c.opp

all: $(TARGETS)

clean:
	clear
	rm -f $(TARGETS)

daemon: BoneHeader/BoneHeader.opp daemon.opp  MMAP/mmapgpio.opp HT1632c.opp
	$(CCPP) $^ -o $@

daemon.opp: HT1632c.opp
HT1632.opp: BoneHeader/BoneHeader.o MMAP/mmapgpio.opp

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.opp: %.cpp
	$(CCPP) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
