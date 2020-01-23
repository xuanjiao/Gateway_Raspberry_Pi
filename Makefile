# Use g++ compiler
CC = g++

HEADER = ble/ble_process.h ble/gatt_client.h
SOURCES = main.cpp
EXECUTEBLE = simplescan

# print warnings
CFLAGS = -Wall -Ible

# Place object file in build directory 
ODIR = build

# Link with thread lib and gattlib
USER_LIBS = -lgattlib -lpthread

OBJECTS = simplescan.cpp.o

all: $(EXECUTEBLE)

# -c(generate .o file) -o(generate output file) @(left side of:) <(first right side of :)
$(OBJECTS): $(SOURCES) $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $(ODIR)/$@ $(USER_LIBS)

# ^(all right side of :)
$(EXECUTEBLE) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(ODIR)/$^ $(USER_LIBS)

clean:
	-rm -f $(OBJECTS)