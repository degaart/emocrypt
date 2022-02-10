/*
    Generates symbol.h from unicode emoji data file
    Emoji data file can be downloaded from: https://www.unicode.org/Public/UCD/latest/ucd/emoji/emoji-data.txt
    TODO: Use https://unicode.org/Public/emoji/14.0/emoji-sequences.txt and https://unicode.org/Public/emoji/14.0/emoji-zwj-sequences.txt instead
*/
#include <common/format.h>
#include <fstream>
#include <locale>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>

using std::unique_ptr;
using std::make_unique;

unique_ptr<std::string> to_utf8(char32_t codepoint)
{
    std::locale loc("");
    const std::codecvt<char32_t, char, std::mbstate_t> &cvt =
                   std::use_facet<std::codecvt<char32_t, char, std::mbstate_t>>(loc);

    std::mbstate_t state{{0}};

    const char32_t * last_in;
    char *last_out;
    char buf[4];
    std::codecvt_base::result res = cvt.out(state, &codepoint, 1+&codepoint, last_in,
                                            buf, buf+4, last_out);
    if(res != std::codecvt_base::result::ok)
        return nullptr;
    auto sz = last_out - buf;
    return make_unique<std::string>(buf, sz);
}

int main(int argc, char** argv)
{
    if(argc < 2)
        throw std::runtime_error("Required argument missing: input file");

    std::ifstream is(argv[1], std::ios::in|std::ios::binary);
    if(!is)
        throw std::system_error(errno, std::system_category());
    is.exceptions(std::ios::badbit);

    std::regex rex(R"(([0-9A-Fa-f]+)(?:\.\.([0-9A-Fa-f]+))?\s+;\s+Emoji\s+.*)");
    std::string line;
    std::vector<std::tuple<std::string,uint32_t>> result;
    while(std::getline(is, line)) {
        std::smatch mat;
        if(std::regex_match(line, mat, rex)) {
            uint64_t start_val = std::stoll(mat[1], nullptr, 16);
            uint64_t end_val = mat[2].length() ? std::stoll(mat[2], nullptr, 16) : start_val;

            for(uint64_t cp = start_val; cp < end_val; cp++) {
                std::string utf8_str = *to_utf8(cp);
                result.push_back(std::make_tuple<std::string,uint32_t>(std::move(utf8_str), cp));
            }
        }
    }

    ec::println("namespace ec {");
    ec::println("    const unsigned char symbols[][4] = {");
    for(const auto& emoji : result) {
        ec::print("        {");
        for(int i = 0; i < 4; i++) {
            unsigned value = 0;
            if(i < std::get<0>(emoji).size())
                value = std::get<0>(emoji)[i] & 0xFF;
            ec::print(std::hex, "0x", value, ", ");
        }
        ec::println("}, // ", std::get<1>(emoji), " ", std::get<0>(emoji));
    }
    ec::println("    };");
    ec::println("}");
    return 0;
}

