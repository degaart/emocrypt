#include "term_echo.h"
#include <stdio.h>
#include <termios.h>

namespace ec {

    TermEcho::TermEcho()
    {
    }

    TermEcho::~TermEcho()
    {
        enable();
    }

    void TermEcho::enable()
    {
        termios ti;
        tcgetattr(fileno(stdin), &ti);
        ti.c_lflag |= ECHO;
        tcsetattr(fileno(stdin), TCSAFLUSH, &ti);
    }

    void TermEcho::disable()
    {
        termios ti;
        tcgetattr(fileno(stdin), &ti);
        ti.c_lflag &= ~ECHO;
        tcsetattr(fileno(stdin), TCSAFLUSH, &ti);
    }

    void TermEcho::restore()
    {
        enable();
    }

}
