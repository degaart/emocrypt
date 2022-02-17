#include <cassert>
#include <cerrno>
#include <common/encrypt.h>
#include <common/fd.h>
#include <common/format.h>
#include <common/options.h>
#include <common/term_echo.h>
#include <common/version.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string.h>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#else
#   include <unistd.h>
#endif

using std::unique_ptr;
using std::make_unique;

namespace {

    static constexpr auto FLAG_APPEND_NEWLINE = 0x1;           // Append newline to outpur

    std::string get_password(const std::string& prompt)
    {
#ifdef DEBUG_PASSWORD
        return "hunter2";
#elif defined(WIN32)
        ec::fprint(std::cerr, prompt);

        std::string password(254, '\0');
        HANDLE handle = CreateFileA(
            "CONIN$",
            GENERIC_READ | GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            0,
            0
        );

        DWORD orig = 0;
        GetConsoleMode(handle, &orig);

        DWORD mode = orig;
        mode |= ENABLE_PROCESSED_INPUT;
        mode &= ~ENABLE_ECHO_INPUT;
        SetConsoleMode(handle, mode);

        DWORD readBytes;
        ReadConsole(
            handle,
            &password[0], password.size(),
            &readBytes,
            nullptr);
        WriteConsole(handle, "\n", 1, 0, 0);
        SetConsoleMode(handle, orig);
        CloseHandle(handle);

        if(readBytes >= 2)
            password.resize(readBytes - 2);
        else
            password.resize(0);

        assert(password.find('\n') == std::string::npos);
        assert(password.find('\r') == std::string::npos);
        ec::fprint(std::cerr, "\n");
        return password;
#else
        ec::fprint(std::cerr, prompt);
        std::cout.flush();

        std::string password;
        char ch;
        ec::FD tty("/dev/tty", O_RDONLY);

        ec::TermEcho term_echo(tty.fd());
        term_echo.disable();

        while(true) {
            auto ret = tty.read(&ch, sizeof(ch));
            if(!ret || ch == '\n')
                break;
            
            password.append(1, ch);
        }

        ec::fprint(std::cerr, "\n");
        assert(password.find('\n') == std::string::npos);
        
        return password;
#endif
    }

    bool decrypt(const std::string& infile, const std::string& outfile, unsigned flags)
    {
        std::string password = get_password("Password: ");
        if(password.empty()) {
            ec::fprintln(std::cerr, "Password is required");
            return false;
        }

        std::ifstream fis;
        std::istream* is = &std::cin;
        if(infile.size()) {
            fis.open(infile, std::ios::in|std::ios::binary);
            if(!fis) {
                ec::fprintln(std::cerr, "open(\"", infile, "\"): ", strerror(errno));
                return false;
            }
            fis.exceptions(std::ios::badbit);
            is = &fis;
        }

        ec::Symbols symbols = ec::load_symbols();
        std::string encoded_ciphertext((std::istreambuf_iterator<char>(*is)),
                                        std::istreambuf_iterator<char>());
        ec::byte_string ciphertext = ec::decode(symbols, encoded_ciphertext);
        if(ciphertext.empty()) {
            ec::fprintln(std::cerr, "Invalid ciphertext data");
            return false;
        }

        ec::byte_string plaintext = ec::decrypt(ciphertext.data(), ciphertext.size(), password);
        if(plaintext.empty()) {
            ec::fprintln(std::cerr, "Decryption failed");
            return false;
        }

        std::ofstream fos;
        std::ostream* os = &std::cout;
        if(outfile.size()) {
            fos.open(outfile, std::ios::out|std::ios::binary|std::ios::trunc);
            if(!fos) {
                ec::fprintln(std::cerr, "open(\"", outfile, "\"): ", strerror(errno));
                return false;
            }
            fos.exceptions(std::ios::badbit);
            os = &fos;
        }

        if(flags & FLAG_APPEND_NEWLINE) {
#ifdef WIN32
            plaintext.append("\r\n");
#else
            plaintext.append(reinterpret_cast<const unsigned char*>("\n"));
#endif
        }
        os->write(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
            
        return true;
    }

    bool encrypt(const std::string& infile, const std::string& outfile, const std::string& channel, int line_length, unsigned flags)
    {
        std::string password = get_password("Password: ");
        if(password.empty()) {
            ec::fprintln(std::cerr, "Password is required");
            return false;
        }

        std::string confirmation = get_password("Confirmation: ");
        if(confirmation != password) {
            ec::fprintln(std::cerr, "Password and confirmation do not match");
            return false;
        }

        std::ifstream fis;
        std::istream* is = &std::cin;
        if(infile.size()) {
            fis.open(infile, std::ios::binary|std::ios::in);
            if(!fis) {
                ec::fprintln(std::cerr, "open(\"", infile, "\"): ", strerror(errno));
                return false;
            }
            fis.exceptions(std::ios::badbit);
            is = &fis;
        }

        std::string channel_text;
        if(!channel.empty()) {
            ec::FD fd(channel, O_RDONLY);
            channel_text = fd.read_all();
        }

        std::string plaintext((std::istreambuf_iterator<char>(*is)),
                              std::istreambuf_iterator<char>());
        if(is->bad()) {
            ec::fprintln(std::cerr, "read(): ", strerror(errno));
            return false;
        }

        ec::byte_string ciphertext = ec::encrypt(plaintext.data(), plaintext.size(), password);
        if(ciphertext.empty()) {
            ec::fprintln(std::cerr, "Encryption failed");
            return false;
        }

        std::random_device rd;
        std::mt19937 rng(rd());
        ec::Symbols symbols = ec::load_symbols();
        std::string output = ec::encode(rng, symbols, ciphertext.data(), ciphertext.size(), line_length);
        
        std::ofstream fos;
        std::ostream* os = &std::cout;
        if(outfile.size()) {
            fos.open(outfile, std::ios::binary|std::ios::out|std::ios::trunc);
            if(!fos) {
                ec::fprintln(std::cerr, "open(\"", outfile, "\"): ", strerror(errno));
                return false;
            }
            fos.exceptions(std::ios::badbit);
            os = &fos;
        }

        if(!channel.empty()) {
            output = ec::conceal(output, channel_text, symbols);
            if(output.empty()) {
                ec::fprintln(std::cerr, "Failed to conceal message");
                return false;
            }
        }

        if(flags & FLAG_APPEND_NEWLINE) {
#ifdef WIN32
            output.append("\r\n");
#else
            output.append("\n");
#endif
        }

        os->write(output.data(), output.size());
        return true;
    }
}

int main(int argc, char** argv)
{
    ec::Options opt;
    opt.add("infile", ec::ArgType::Required, 'i');
    opt.add("outfile", ec::ArgType::Required, 'o');
    opt.add("decrypt", ec::ArgType::None, 'd');
    opt.add("encrypt", ec::ArgType::None, 'e');
    opt.add("conceal", ec::ArgType::Required, 'c');
    opt.add("line-length", ec::ArgType::Required, 'l');
    opt.add("newline", ec::ArgType::None, 'n');
    opt.add("version", ec::ArgType::None, 'v');
    opt.add("help", ec::ArgType::None, 'h');
    opt.parse(argc, argv);

    std::string infile;
    if(opt.isPresent("infile"))
        infile = opt.arg("infile");
    
    std::string outfile;
    if(opt.isPresent("outfile"))
        outfile = opt.arg("outfile");

    std::string channel;
    if(opt.isPresent("conceal"))
        channel = opt.arg("conceal");

    int line_length = 20;
    if(opt.isPresent("line-length"))
        line_length = std::stoi(opt.arg("line-length"));

    unsigned flags = 0;
    if(opt.isPresent("newline"))
        flags |= FLAG_APPEND_NEWLINE;

    if(opt.isPresent("help")) {
        ec::fprintln(std::cerr, "Usage: ", argv[0], " <options>");
        ec::fprintln(std::cerr, "Options:");
        ec::fprintln(std::cerr, "  --infile,-i        Input file (default: stdin)");
        ec::fprintln(std::cerr, "  --outfile,-o       Output file (default: stdout)");
        ec::fprintln(std::cerr, "  --decrypt,-d       Decrypt");
        ec::fprintln(std::cerr, "  --encrypt,-e       Encrypt (default)");
        ec::fprintln(std::cerr, "  --conceal,-c       Hide ciphertext by using this file as a channel");
        ec::fprintln(std::cerr, "  --line-length,-l   Line length (default: 20)");
        ec::fprintln(std::cerr, "  --newline,-n       Append a newline to output");
        ec::fprintln(std::cerr, "  --version,-v       Show program version");
        ec::fprintln(std::cerr, "  --help,-h          Show help");
        return 0;
    } else if(opt.isPresent("version")) {
        ec::println(argv[0], " ", ec::VERSION_MAJOR, ".", ec::VERSION_MINOR);
    } else if(opt.isPresent("decrypt")) {
        return !decrypt(infile, outfile, flags);
    } else {
        return !encrypt(infile, outfile, channel, line_length, flags);
    }
    return 0;
}
