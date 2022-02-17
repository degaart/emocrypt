#pragma once

#include <iterator>
#include <string>
#include <algorithm>

namespace ec {

    class FD {
    private:
        int _fd;
    public:
        FD(const std::string& path, int mode);
        FD(const FD&) = delete;
        FD(FD&& other);
        ~FD();
        FD& operator=(const FD&) = delete;
        FD& operator=(FD&& other);
        size_t read(void*, size_t);
        void write(const void*, size_t);
        void close();
        int fd();
        
        template<typename T = std::string>
        T read_all() {
            T result;
            char buffer[512];
            while(true) {
                auto r = read(buffer, sizeof(buffer));
                if(r == 0)
                    break;
                std::copy_n(buffer, r, std::back_inserter(result));
            }
            return result;
        }
    };

}
