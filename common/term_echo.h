#pragma once

namespace ec {
    
    class TermEcho {
    private:
        int _fd;
    public:
        explicit TermEcho(int fd);
        ~TermEcho();
        void enable();
        void disable();
        void restore();
    };
    
}
