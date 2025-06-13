#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)

    #include <windows.h>
    #include <stdio.h>
    #include <conio.h>

    static inline void init_console(void) {
        SetConsoleOutputCP(CP_UTF8);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD outMode; GetConsoleMode(hOut, &outMode);
        SetConsoleMode(hOut, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    int my_getch() {
        return _getch();
    }

    int my_kbhit() {
        return _kbhit();
    }

    void clear_screen() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(h, &csbi);
        DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
        COORD home = {0,0};
        DWORD written;
        FillConsoleOutputCharacter(h, ' ', cells, home, &written);
        FillConsoleOutputAttribute(h, csbi.wAttributes, cells, home, &written);
        SetConsoleCursorPosition(h, home);
    }

    void sleep_ms(int ms) {
        Sleep(ms);
    }

    #define BOARD_TOP_BOTTOM "--"
    #define BOARD_TOPLEFT "+"
    #define BOARD_TOPRIGHT "+"
    #define BOARD_WALL "|"
    #define BOARD_BOTTOMLEFT "+"
    #define BOARD_BOTTOMRIGHT "+"
	#define FALLING_BLOCK "[]"
	#define FIXED_BLOCK "[]"

    static double get_time_sec(void) {
        static LARGE_INTEGER freq = {0};
        if (!freq.QuadPart) QueryPerformanceFrequency(&freq);
      LARGE_INTEGER now;
      QueryPerformanceCounter(&now);
      return (double)now.QuadPart / freq.QuadPart;
    }

#else
    // POSIX (Linux, macOS, Git Bash, MSYS2)
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/select.h>
    #include <stdio.h>
    #include <time.h>

    static inline void init_console(void) {}

    int my_getch() {
    struct termios oldt, newt;
    int oldf;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 즉시 입력 + 입력 안 보이게
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf & ~O_NONBLOCK);  // 차단 모드 설정

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);  // 원래 상태 복구

    return ch;
}

    int my_kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    int hit = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    if (ch != EOF) {
        ungetc(ch, stdin);  // 입력 버퍼에 다시 넣기
        hit = 1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return hit;
}


    void clear_screen() {
        printf("\x1b[H\x1b[2J");
    }

    void sleep_ms(int ms) {
        usleep(ms * 1000);
    }

    #define BOARD_TOP_BOTTOM "🟩"
    #define BOARD_TOPLEFT "🟩"
    #define BOARD_TOPRIGHT "🟩"
    #define BOARD_WALL "🟩"
    #define BOARD_BOTTOMLEFT "🟩"
    #define BOARD_BOTTOMRIGHT "🟩"
	#define FALLING_BLOCK "🔳"
	#define FIXED_BLOCK "🟥"

    static double get_time_sec(void) {
      struct timespec t;
      clock_gettime(CLOCK_MONOTONIC, &t);
      return t.tv_sec + t.tv_nsec * 1e-9;
    }
#endif

void hide_cursor() {
    printf("\033[?25l");
    fflush(stdout);
}

void show_cursor() {
    printf("\033[?25h");
    fflush(stdout);
}
#define BLANK "  "

#endif // PLATFORM_H


