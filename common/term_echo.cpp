#include "term_echo.h"

#ifdef WIN32
#   include <Windows.h>
#else
#   include <termios.h>
#endif

namespace ec {

    TermEcho::TermEcho(int fd)
        : _fd(fd)
    {
    }

    TermEcho::~TermEcho()
    {
        enable();
    }

    void TermEcho::enable()
    {
#ifdef WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        mode |= ENABLE_ECHO_INPUT;
        SetConsoleMode(hStdin, mode );
#else
        termios ti;
        tcgetattr(_fd, &ti);
        ti.c_lflag |= ECHO;
        tcsetattr(_fd, TCSAFLUSH, &ti);
#endif
    }

    void TermEcho::disable()
    {
#ifdef WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        mode &= ~ENABLE_ECHO_INPUT;
        SetConsoleMode(hStdin, mode );
#else
        termios ti;
        tcgetattr(_fd, &ti);
        ti.c_lflag &= ~ECHO;
        tcsetattr(_fd, TCSAFLUSH, &ti);
#endif
    }

    void TermEcho::restore()
    {
        enable();
    }

}
