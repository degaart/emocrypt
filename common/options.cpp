#include "format.h"
#include "options.h"
#include <algorithm>
#include <getopt.h>
#include <map>
#include <sstream>
#include <vector>

using std::optional;
using std::nullopt;

namespace ec {

    class Options::Impl {
        private:
            std::map<std::string,Option> _options;
            int _nextValue;
            std::vector<std::string> _positionals;

            optional<Option> get(const std::string& name) const;
        public:
            Impl();
            void add(std::string name, ArgType type, char shortopt = '\0');
            void parse(int argc, char** argv);
            bool isPresent(const std::string& name) const;
            optional<std::string> arg(const std::string& name) const;
            const std::vector<std::string>& positionals() const;
    };

    Options::Impl::Impl()
        : _nextValue(256)
    {
    }
    
    void Options::Impl::add(std::string name, ArgType type, char shortopt)
    {
        Option opt;
        opt.type = type;
        opt.shortopt = shortopt;
        opt.value = shortopt ? shortopt : _nextValue++;
        opt.present = false;
        _options.emplace(std::move(name), opt);
    }

    void Options::Impl::parse(int argc, char** argv)
    {
        std::unique_ptr<option[]> options(new option[_options.size() + 1]);
        options[_options.size()] = {};

        std::stringstream opts;
        size_t i = 0;
        for(const auto& opt : _options) {
            options[i].name = opt.first.c_str();
            switch(opt.second.type) {
                case None:
                    options[i].has_arg = no_argument;
                    if(opt.second.shortopt)
                        opts << opt.second.shortopt;
                    break;
                case Required:
                    options[i].has_arg = required_argument;
                    if(opt.second.shortopt)
                        opts << opt.second.shortopt << ":";
                    break;
                case Optional:
                    options[i].has_arg = optional_argument;
                        opts << opt.second.shortopt << ":";
                    break;
            }
            options[i].flag = nullptr;
            options[i].val = opt.second.value;
            i++;
        }

        int ch;
        while((ch = getopt_long(argc, argv, opts.str().c_str(), options.get(), nullptr)) != -1) {
            auto it = std::find_if(_options.begin(), _options.end(), [ch](const std::map<std::string,Option>::value_type& opt) {
                return opt.second.value == ch;
            });
            
            if(it == _options.end())
                throw std::runtime_error(format("Unexpected error parsing args (getopt_long returned ", ch, ")"));
            
            it->second.present = true;
            if(optarg)
                it->second.arg = std::string(optarg);
        }

        argc -= optind;
        argv += optind;

        for(int i = 0; i < argc; i++) {
            _positionals.push_back(argv[i]);
        }
    }

    optional<Option> Options::Impl::get(const std::string& name) const
    {
        auto it = _options.find(name);
        if(it == _options.end())
            return nullopt;
        return it->second;
    }

    bool Options::Impl::isPresent(const std::string& name) const
    {
        auto opt = get(name);
        if(!opt)
            return false;
        return opt->present;
    }

    optional<std::string> Options::Impl::arg(const std::string& name) const
    {
        auto opt = get(name);
        if(!opt)
            return nullopt;
        return opt->arg;
    }

    const std::vector<std::string>& Options::Impl::positionals() const
    {
        return _positionals;
    }

    Options::Options()
        : _impl(new Options::Impl())
    {
    }

    Options::~Options()
    {
    }

    void Options::add(std::string name, ArgType type, char shortopt)
    {
        _impl->add(std::move(name), type, shortopt);
    }

    void Options::parse(int argc, char** argv)
    {
        _impl->parse(argc, argv);
    }

    bool Options::isPresent(const std::string& name) const
    {
        return _impl->isPresent(name);
    }

    optional<std::string> Options::arg(const std::string& name) const
    {
        return _impl->arg(name);
    }

    const std::vector<std::string>& Options::positionals() const
    {
        return _impl->positionals();
    }


}
