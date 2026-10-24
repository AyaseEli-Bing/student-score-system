CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
TARGET  = student_score_system
SRC     = student_score_system.c

.PHONY: all build run clean

all: build

build: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)
