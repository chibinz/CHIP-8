SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -O3
LDFLAGS = -lopengl32 -lglfw3dll

main.exe: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	del /q /s $(OBJ)
