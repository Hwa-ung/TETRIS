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
        int ch;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

    int my_kbhit() {
        struct timeval tv = {0L, 0L};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
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
