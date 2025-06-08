# 대상 실행 파일 이름
TARGET = tetris

# 소스 파일
SRCS = TETRIS_ver1.c

# 헤더 포함 경로 (필요시 추가 가능)
INCLUDES =

# 컴파일러 및 플래그
CC = gcc
CFLAGS = -Wall -g

# 링크 플래그 (없으면 비워둠)
LDFLAGS =

# 기본 빌드 규칙
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

# 실행
run: $(TARGET)
	./$(TARGET)

# 클린업
clean:
	rm -f $(TARGET)
