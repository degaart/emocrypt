#include <cerrno>
#include <common/encrypt.h>
#include <common/format.h>
#include <common/options.h>
#include <common/term_echo.h>
#include <fstream>
#include <termios.h>
#include <unistd.h>

namespace {

    bool decrypt(const std::optional<std::string>& infile, const std::optional<std::string>& outfile)
    {
        ec::fprint(std::cerr, "Password: ");
        std::cout.flush();

        ec::TermEcho term_echo;
        term_echo.disable();

        std::string password;
        if(!std::getline(std::cin, password)) {
            ec::fprintln(std::cerr, "\nNo password supplied");
            return false;
        }
        term_echo.enable();
        ec::fprint(std::cerr, "\n");

        std::ifstream fis;
        std::istream* is = &std::cin;
        if(infile) {
            fis.open(*infile, std::ios::in|std::ios::binary);
            if(!fis) {
                ec::fprintln(std::cerr, "open(\"", *infile, "\"): ", strerror(errno));
                return false;
            }
            fis.exceptions(std::ios::badbit);
            is = &fis;
        }

        ec::Symbols symbols = ec::load_symbols();
        std::string encoded_ciphertext((std::istreambuf_iterator<char>(*is)),
                                        std::istreambuf_iterator<char>());
        ec::byte_string ciphertext = ec::decode(symbols, encoded_ciphertext);
        ec::byte_string plaintext = ec::decryt(ciphertext.data(), ciphertext.size(), password);

        std::ofstream fos;
        std::ostream* os = &std::cout;
        if(outfile) {
            fos.open(*outfile, std::ios::out|std::ios::binary|std::ios::trunc);
            if(!fos) {
                ec::fprintln(std::cerr, "open(\"", *outfile, "\"): ", strerror(errno));
                return false;
            }
            fos.exceptions(std::ios::badbit);
            os = &fos;
        }
        os->write(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
        return true;
    }

    bool encrypt(const std::optional<std::string>& infile, const std::optional<std::string>& outfile, int line_length)
    {
        ec::fprint(std::cerr, "Password: ");
        std::cout.flush();

        ec::TermEcho term_echo;
        term_echo.disable();

        std::string password;
        if(!std::getline(std::cin, password)) {
            ec::fprintln(std::cerr, "\nNo password supplied");
            return false;
        }

        ec::fprint(std::cerr, "\nConfirm: ");
        std::cerr.flush();

        std::string confirmation;
        std::getline(std::cin, confirmation);

        if(confirmation != password) {
            ec::fprintln(std::cerr, "\nPassword mismatch");
            return false;
        }
        ec::fprintln(std::cerr);
        term_echo.enable();

        std::ifstream fis;
        std::istream* is = &std::cin;
        if(infile) {
            fis.open(*infile, std::ios::binary|std::ios::in);
            if(!fis) {
                ec::fprintln(std::cerr, "open(\"", *infile, "\"): ", strerror(errno));
                return false;
            }
            fis.exceptions(std::ios::badbit);
            is = &fis;
        }

        std::string plaintext((std::istreambuf_iterator<char>(*is)),
                              std::istreambuf_iterator<char>());
        if(is->bad()) {
            ec::fprintln(std::cerr, "read(): ", strerror(errno));
            return false;
        }

        ec::byte_string ciphertext = ec::encrypt(plaintext.data(), plaintext.size(), password);

        std::random_device rd;
        std::mt19937 rng(rd());
        ec::Symbols symbols = ec::load_symbols();
        std::string encoded_ciphertext = ec::encode(rng, symbols, ciphertext.data(), ciphertext.size(), line_length);
        
        std::ofstream fos;
        std::ostream* os = &std::cout;
        if(outfile) {
            fos.open(*outfile, std::ios::binary|std::ios::out|std::ios::trunc);
            if(!fos) {
                ec::fprintln(std::cerr, "open(\"", *outfile, "\"): ", strerror(errno));
                return false;
            }
            fos.exceptions(std::ios::badbit);
            os = &fos;
        }
        os->write(encoded_ciphertext.data(), encoded_ciphertext.size());
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
    opt.add("line-length", ec::ArgType::Required, 'l');
    opt.add("help", ec::ArgType::None, 'h');
    opt.parse(argc, argv);

    std::optional<std::string> infile;
    if(opt.isPresent("infile"))
        infile = opt.arg("infile");
    
    std::optional<std::string> outfile;
    if(opt.isPresent("outfile"))
        outfile = opt.arg("outfile");

    int line_length = 80;
    if(opt.isPresent("line-length"))
        line_length = std::stoi(*opt.arg("line-length"));

    if(opt.isPresent("help")) {
        ec::fprintln(std::cerr, "Usage: ", argv[0], " <options>");
        ec::fprintln(std::cerr, "Options:");
        ec::fprintln(std::cerr, "    --infile, -i       Input file (default: stdin)");
        ec::fprintln(std::cerr, "    --outfile, -o      Output file (default: stdout)");
        ec::fprintln(std::cerr, "    --decrypt, -d      Decrypt");
        ec::fprintln(std::cerr, "    --encrypt, -e      Encrypt (default)");
        ec::fprintln(std::cerr, "    --line-length, -l  Line length (default: 80)");
        ec::fprintln(std::cerr, "    --help, -h         Show help");
        return 0;
    } else if(opt.isPresent("decrypt")) {
        return !decrypt(infile, outfile);
    } else {
        return !encrypt(infile, outfile, line_length);
    }
    return 0;
}
