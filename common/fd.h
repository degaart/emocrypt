#pragma once

#include <string>

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
    };

}
