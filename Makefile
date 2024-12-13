CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -pthread

SRC = main.c vote.c data.c
OBJ = $(SRC:.c=.o)
TARGET = voting_machine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
