CC = gcc
CFLAGS = -Wall -Wextra -O2
DEBUGFLAGS = -g -O0
TARGET = build/main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

debug: CFLAGS = -Wall -Wextra $(DEBUGFLAGS)
debug: clean $(TARGET)

clean:
	rm -f $(TARGET)
