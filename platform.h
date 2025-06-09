#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)

    #include <windows.h>
    #include <stdio.h>

    int my_getch() {
        DWORD mode, count;
        INPUT_RECORD rec;
        HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(h, &mode);
        SetConsoleMode(h, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
        FlushConsoleInputBuffer(h);
        do {
            ReadConsoleInput(h, &rec, 1, &count);
        } while (rec.EventType != KEY_EVENT || !rec.Event.KeyEvent.bKeyDown);
        SetConsoleMode(h, mode);
        return rec.Event.KeyEvent.uChar.AsciiChar;
    }

    int my_kbhit() {
        DWORD count;
        GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &count);
        return count > 0;
    }

    void clear_screen() {
        system("cls");
    }

    void sleep_ms(int ms) {
        Sleep(ms);
    }

#else
    // POSIX (Linux, macOS, Git Bash, MSYS2)
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/select.h>
    #include <stdio.h>

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
        printf("\033[2J\033[H");
        fflush(stdout);
    }

    void sleep_ms(int ms) {
        usleep(ms * 1000);
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

#endif // PLATFORM_H
