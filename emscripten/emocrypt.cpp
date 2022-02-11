#include <common/encrypt.h>
#include <common/symbols.h>
#include <emscripten/bind.h>
#include <random>

using namespace emscripten;

val ec_encrypt(const std::string& password, const std::string& plaintext)
{
    auto ciphertext = ec::encrypt(plaintext.data(),
                                  plaintext.size(),
                                  password);
    if(ciphertext.empty())
        return val::undefined();

    std::random_device rd;
    std::mt19937 rng(rd());
    ec::Symbols symbols = ec::load_symbols();
    std::string encoded_ciphertext = ec::encode(rng, symbols, ciphertext.data(), ciphertext.size(), 20);
    if(encoded_ciphertext.empty())
        return val::undefined();

    return val(encoded_ciphertext);
}

val ec_decrypt(const std::string& password, const std::string& encoded_ciphertext)
{
    ec::Symbols symbols = ec::load_symbols();
    auto ciphertext = ec::decode(symbols, encoded_ciphertext);
    if(ciphertext.empty())
        return val::undefined();
    
    auto plaintext = ec::decrypt(ciphertext.data(), ciphertext.size(), password);
    if(plaintext.empty())
        return val::undefined();

    return val(plaintext);
}


EMSCRIPTEN_BINDINGS(emocrypt) {
    function("ec_encrypt", &ec_encrypt);
    function("ec_decrypt", &ec_decrypt);
}


