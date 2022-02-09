#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace ec {

    template<typename... Args>
    inline void fprint(std::ostream& os, Args&&... args)
    {
        (os << ... << std::forward<Args>(args));
    }

    template<typename... Args>
    inline void fprintln(std::ostream& os, Args&&... args)
    {
        (os << ... << std::forward<Args>(args)) << "\n";
    }

    template<typename... Args>
    inline void print(Args&&... args)
    {
        fprint(std::cout, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void println(Args&&... args)
    {
        fprint(std::cout, std::forward<Args>(args)..., "\n");
    }

    template<typename... Args>
    inline std::string format(Args&&... args)
    {
        std::stringstream ss;
        fprint(ss, std::forward<Args>(args)...);
        return ss.str();
    }

}


