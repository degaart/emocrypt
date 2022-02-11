#include "term_echo.h"
#include <stdio.h>
#include <termios.h>

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
        termios ti;
        tcgetattr(_fd, &ti);
        ti.c_lflag |= ECHO;
        tcsetattr(_fd, TCSAFLUSH, &ti);
    }

    void TermEcho::disable()
    {
        termios ti;
        tcgetattr(_fd, &ti);
        ti.c_lflag &= ~ECHO;
        tcsetattr(_fd, TCSAFLUSH, &ti);
    }

    void TermEcho::restore()
    {
        enable();
    }

}
