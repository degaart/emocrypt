#include "fd.h"
#include <cerrno>
#include <system_error>
#include <fcntl.h>
#include <unistd.h>

namespace ec {

    FD::FD(const std::string& path, int mode)
    {
        _fd = ::open(path.c_str(), mode);
        if(_fd == -1)
            throw std::system_error(errno, std::system_category());
    }

    FD::FD(FD&& other)
        : _fd(-1)
    {
        std::swap(_fd, other._fd);
    }

    FD::~FD()
    {
        close();
    }

    FD& FD::operator=(FD&& other)
    {
        if(this != &other) {
            close();
            std::swap(_fd, other._fd);
        }
        return *this;
    }

    size_t FD::read(void* buffer, size_t len)
    {
        ssize_t ret;
        do {
            ret = ::read(_fd, buffer, len);
        } while(ret == -1 && errno == EINTR);
        if(ret == -1)
            throw std::system_error(errno, std::system_category());
        return ret;
    }

    void FD::write(const void* buffer, size_t len)
    {
        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buffer);
        while(len) {
            ssize_t ret;
            do {
                ret = ::write(_fd, ptr, len);
            } while(ret == -1 && errno == EINTR);
            if(ret == -1)
                throw std::system_error(errno, std::system_category());
            
            len -= ret;
            ptr += ret;
        }
    }

    void FD::close()
    {
        if(_fd != -1)
            ::close(_fd);
        _fd = -1;
    }

    int FD::fd()
    {
        return _fd;
    }

}

