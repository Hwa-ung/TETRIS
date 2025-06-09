#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    #include <conio.h>
    #include <Windows.h>

    int my_getch() {
        return _getch();
    }
    int my_kbhit() {
        return _kbhit();
    }
    void clear_screen() {
        system("cls");
    }
    void sleep_ms(int ms) {
        Sleep(ms);
    }
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <sys/select.h>
    #include <time.h>

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

#endif // _WIN32 (4)

void hide_cursor() {
    printf("\033[?25l");
    fflush(stdout);
}

void show_cursor() {
    printf("\033[?25h");
    fflush(stdout);
}
#endif // PLATFORM_H (2)