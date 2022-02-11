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

    std::random_device rd;
    std::mt19937 rng(rd());
    ec::Symbols symbols = ec::load_symbols();
    std::string encoded_ciphertext = ec::encode(rng, symbols, ciphertext.data(), ciphertext.size(), 20);

    return encoded_ciphertext;
}

val ec_decrypt(const std::string& password, const std::string& encoded_ciphertext)
{
    ec::Symbols symbols = ec::load_symbols();
    auto ciphertext = ec::decode(symbols, encoded_ciphertext);
    auto plaintext = ec::decrypt(ciphertext.data(), ciphertext.size(), password);
    return val(plaintext);
}


EMSCRIPTEN_BINDINGS(emocrypt) {
    function("ec_encrypt", &ec_encrypt);
    function("ec_decrypt", &ec_decrypt);
}


