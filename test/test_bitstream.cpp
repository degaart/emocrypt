#include <climits>
#include <common/format.h>
#include <cstdint>
#include <cassert>
#include <sstream>

struct BitStream {
    const unsigned char* _buffer;
    size_t _len;
    int _offset;

    BitStream(const void* buffer, size_t len)
        : _buffer(reinterpret_cast<const unsigned char*>(buffer)),
          _len(len),
          _offset(0)
    {
    }

    /* EOF: return -1 */
    int read()
    {
        if(_len == 0)
            return -1;

        int result = static_cast<bool>(*_buffer & (1 << _offset));
        _offset++;
        if(_offset >= CHAR_BIT) {
            _buffer++;
            _len--;
            _offset = 0;
        }
        return result;
    }

    int read(int count)
    {
        if(_len == 0)
            return -1;

        int result = 0;
        int shift = 0;
        while(count) {
            int ret = read();
            if(ret == -1)
                break;
            else if(ret == 1)
                result |= 1 << shift;
            shift++;
        }
        return result;
    }

};

int main()
{
    uint32_t buffer = 0xC3F0966A;           /* 1100 0011 1111 0000 1001 0110 0110 1010 */
    BitStream bs(&buffer, sizeof(buffer));

    int index = 0;
    std::stringstream ss;
    while(true) {
        int ret = bs.read();
        if(ret == -1)
            break;
        ss << ret;
        
        index++;
        if(index % 4 == 0)
            ss << " ";
    }
    ec::println(ss.str());
    return 0;
}

