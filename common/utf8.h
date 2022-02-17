#pragma once

#include <cassert>
#include <iterator>
#include <string>

namespace ec {
    
    int utf8_seq_length(unsigned char ch);

    class utf8_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = char;
        using pointer = const value_type*;
        using reference = const value_type&;

    private:
        pointer _ptr;
    public:
        utf8_iterator(pointer ptr) : _ptr(ptr) {}
        std::string operator*() const { return std::string(_ptr, utf8_seq_length(*_ptr)); }
        pointer operator->() const { return _ptr; }
        utf8_iterator& operator++() { _ptr += utf8_seq_length(*_ptr); return *this; }

        utf8_iterator operator++(int) { utf8_iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator==(const utf8_iterator& a, const utf8_iterator& b) { return a._ptr == b._ptr; }
        friend bool operator!=(const utf8_iterator& a, const utf8_iterator& b) { return a._ptr != b._ptr; }
    };

}

