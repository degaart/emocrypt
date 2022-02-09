#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace ec {
    enum ArgType {
        None,
        Required,
        Optional,
    };

    struct Option {
        ArgType type;
        char shortopt;
        int value;
        bool present;
        std::optional<std::string> arg;
    };

    class Options {
    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    public:
        Options();
        ~Options();
        void add(std::string name, ArgType type, char shortopt = '\0');
        void parse(int argc, char** argv);
        bool isPresent(const std::string& name) const;
        std::optional<std::string> arg(const std::string& name) const;
        const std::vector<std::string>& positionals() const;
    };
}
