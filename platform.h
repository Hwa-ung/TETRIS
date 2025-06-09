#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    #include <conio.h>
    #include <Windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <sys/select.h>
#endif // _WIN32 (4)

// getch
int my_getch() {
    #ifdef _WIN32
        return _getch();
    #else
        struct termios oldt, newt;
        int ch;

        // 현재 터미널 속성 가져오기
        if (tcgetattr(STDIN_FILENO, &oldt) == -1) return -1;

        // 새 설정 복사
        newt = oldt;

        // 캐노니컬 모드/에코 끄고, 최소 입력/시간 설정
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN] = 1;
        newt.c_cc[VTIME] = 0;

        // 새 설정 적용
        if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) return -1;

        // 문자 하나 읽기 (엔터 없이 즉시 입력)
        if (read(STDIN_FILENO, &ch, 1) < 0) ch = -1;

        // 원래 설정 복원
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
    #endif
}


// kbhit
int my_kbhit() {
    #ifdef _WIN32
        return _kbhit();
    #else
        struct timeval tv = {0L, 0L};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(1, &fds, NULL, NULL, &tv);
    #endif // _WIN32 (40)
}

// clear screen
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else 
        printf("\033[2J");
        printf("\033[H");
    #endif // _WIN32 (53)
}

// sleep (millisecond)
void sleep_ms(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif // _WIN32 (63)
}
void hide_cursor() {
    printf("\033[?25l");
}

void show_cursor() {
    printf("\033[?25h");
}
#endif // PLATFORM_H (2)