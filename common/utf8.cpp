#include "utf8.h"

namespace ec {

    int utf8_seq_length(unsigned char ch)
    {
        if((ch & 0xF0) == 0xF0)
            return 4;
        else if((ch & 0xE0) == 0xE0)
            return 3;
        else if((ch & 0xC0) == 0xC0)
            return 2;
        else if((ch & 0x80) == 0x80)
            return -1;
        else
            return 1;
    }

}
