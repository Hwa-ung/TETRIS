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
        struct temios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW< &oldt);
        return ch;
    #endif // _WIN32 (17)
}

// kbhit
int my_kbhit() {
    #ifdef _WIN32
        return _kbhit();
    #else
        struct timeval tv = {0L, 0L};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        return select(1, &fds, NULL, NULL, &tv);
    #endif // _WIN32 (34)
}

// clear screen
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif // _WIN32 (47)
}

// sleep (millisecond)
void sleep_ms(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif // _WIN32 (56)
}

#endif // PLATFORM_H (2)