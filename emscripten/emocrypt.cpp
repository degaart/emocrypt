#include <common/encrypt.h>
#include <common/symbols.h>
#include <emscripten/bind.h>
#include <random>

using namespace emscripten;

std::string ec_encrypt(const std::string& password, const std::string& plaintext)
{
    auto ciphertext = ec::encrypt(plaintext.data(),
                                  plaintext.size(),
                                  password);
    if(ciphertext.empty())
        return "";

    std::random_device rd;
    std::mt19937 rng(rd());
    ec::Symbols symbols = ec::load_symbols();
    std::string encoded_ciphertext = ec::encode(rng, symbols, ciphertext.data(), ciphertext.size(), 20);
    if(encoded_ciphertext.empty())
        return "";

    return encoded_ciphertext;
}

std::string ec_decrypt(const std::string& password, const std::string& encoded_ciphertext)
{
    ec::Symbols symbols = ec::load_symbols();
    auto ciphertext = ec::decode(symbols, encoded_ciphertext);
    if(ciphertext.empty())
        return "";
    
    auto plaintext = ec::decrypt(ciphertext.data(), ciphertext.size(), password);
    if(plaintext.empty())
        return "";

    return std::string(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
}

std::string ec_conceal(const std::string& password, const std::string& plaintext, const std::string& channel)
{
    auto ciphertext = ec::encrypt(plaintext.data(),
                                  plaintext.size(),
                                  password);
    if(ciphertext.empty())
        return "";

    std::random_device rd;
    std::mt19937 rng(rd());
    ec::Symbols symbols = ec::load_symbols();
    std::string encoded_ciphertext = ec::encode(rng, symbols, ciphertext.data(), ciphertext.size(), 20);
    if(encoded_ciphertext.empty())
        return "";

    std::string covert_message = ec::conceal(encoded_ciphertext, channel, symbols);
    return covert_message;
}


EMSCRIPTEN_BINDINGS(emocrypt) {
    function("ec_encrypt", &ec_encrypt);
    function("ec_decrypt", &ec_decrypt);
    function("ec_conceal", &ec_conceal);
}


