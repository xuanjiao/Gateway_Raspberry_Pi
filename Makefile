# Use g++ compiler
CC = g++

HEADER = ble/ble_process.h  ble/gatt_client.h

# We change .c to exec file by remove '.c'
SOURCE = $(wildcard *.cpp)
TARGETS = $(patsubst %.cpp,%,$(SOURCE))

# print warnings
CFLAGS = -Wall -Ible

# Place object file in build directory 
ODIR = build

# Link with thread lib and gattlib
USER_LIBS = -lgattlib -lpthread


all: $(TARGETS)

# %:%.cpp (e.g. app1: app1.c)
# @(left side of:) <(first right side of :)
$(TARGETS):%:%.cpp $(HEADER)
	$(CC) $< $(CFLAGS) -o $@ $(USER_LIBS) 


clean:
	-rm -rf $(TARGETS)

