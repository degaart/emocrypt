#pragma once

namespace ec {
    
    class TermEcho {
    public:
        TermEcho();
        ~TermEcho();
        void enable();
        void disable();
        void restore();
    };
    
}
