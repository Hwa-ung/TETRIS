# 파일 이름 설정
TARGET = TETRIS_ver1
SRC = TETRIS_ver1.c
OBJ = $(SRC:.c=.o)

# 플랫폼 판별
ifeq ($(OS),Windows_NT)
	ifeq ($(shell uname -o), Msys)
		EXEC = $(TARGET).exe
		RM = rm -f
		RUN = ./$(EXEC)
		CC = gcc
	else
		RUN = $(EXEC)
		RM = cmd /c "del /Q TETRIS_ver1.exe"
		EXEC = $(TARGET).exe
		CC = gcc
	endif
else
	# macOS / Linux
	UNAME_S := $(shell uname -s)
	EXEC = $(TARGET)
	RUN = ./$(EXEC)
	RM = rm -f
	CC = gcc
endif

CFLAGS = -Wall -g

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)
	
run: $(EXEC)
	$(RUN)

clean:
	$(RM) $(EXEC)

.PHONY: all run clean
