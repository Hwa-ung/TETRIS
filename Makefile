# 파일 이름 설정
TARGET = tetris
SRC = TETRIS_ver1.c
OBJ = $(SRC:.c=.o)

# 플랫폼 판별
ifeq ($(OS),Windows_NT)
	CC := $(shell which gcc)
	RM = del /Q
	EXEC = $(TARGET).exe
else
	# macOS / Linux
	UNAME_S := $(shell uname -s)
	CC = gcc
	RM = rm -f
	EXEC = $(TARGET)
endif

CFLAGS = -Wall -g

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)
	
clean:
	$(RM) $(EXEC)
