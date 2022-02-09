#pragma once

#include <string>
#include <map>
#include <vector>
#include <random>

namespace ec {

    using byte_string = std::basic_string<unsigned char>;

    struct Symbols {
        std::map<int,std::vector<std::string>> enc;
        std::map<std::string,int> dec;
    };

    Symbols load_symbols();

    std::string encode(std::mt19937& rng, const Symbols& symbols, const void* buffer, size_t size, int line_length = 0);
    byte_string decode(const Symbols& symbols, const std::string& s);
    byte_string encrypt(const void* buffer, size_t length, const std::string& password);
    byte_string decryt(const void* buffer, size_t length, const std::string& password);

}

