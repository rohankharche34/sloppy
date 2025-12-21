CC      := gcc
TARGET  := main.exe
SRC     := main.c

CFLAGS  := -Wall -Wextra $(shell pkg-config --cflags sdl2)
LDFLAGS := $(shell pkg-config --libs sdl2) 

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
