#include <algorithm>
#include <cassert>
#include <common/encrypt.h>
#include <common/format.h>
#include <locale>
#include <map>
#include <random>
#include <vector>

using namespace ec;

static void test_encrypt()
{
    ec::println("*** Testing encrypt() and decrypt()");
    std::string password("hunter2");
    std::string plaintext("All your base are belong to us");
    ec::println("Plaintext size: ", plaintext.size());

    std::random_device rd;
    std::mt19937 rng(rd());
    Symbols symbols = load_symbols();

    auto ciphertext = encrypt(plaintext.c_str(), plaintext.size(), password);
    auto encoded = encode(rng, symbols, ciphertext.data(), ciphertext.size());
    ec::println("Encoded: ", encoded);
    auto percent = (encoded.size() * 100) / plaintext.size();
    ec::println("Encoded size: ", encoded.size(), " (", percent, "% increase)");

    auto decoded = decode(symbols, encoded);
    assert(decoded == ciphertext);

    auto decrypted = decrypt(decoded.data(), decoded.size(), password);
    ec::println("Decrypted: ", decrypted.c_str());
    ec::println("Decrypted size: ", decrypted.size());
}

static void test_decode()
{
    ec::println("*** Testing encode() and decode()");

    static const size_t data_length = 275000;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    Symbols symbols = load_symbols();

    std::string plain(data_length, '\0');
    for(size_t i = 0; i < data_length; i++) {
        plain[i] = dist(rng);
    }

    auto encoded = encode(rng, symbols, plain.data(), plain.size(), 80);
    auto decoded = decode(symbols, encoded);
    assert(decoded.size() == plain.size());
    assert(!memcmp(decoded.data(), plain.data(), plain.size()));
}

static void test_encrypt_large()
{
    ec::println("*** Testing encrypt() with large data");

    static const size_t data_length = 275000;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    std::string plaintext(data_length, '\0');
    for(size_t i = 0; i < data_length; i++) {
        plaintext[i] = dist(rng);
    }

    std::string password = "hunter2";
    auto ciphertext = ec::encrypt(plaintext.data(), plaintext.size(), password);
    ec::println("ciphertext.size(): ", ciphertext.size());

    auto decrypted = ec::decrypt(ciphertext.data(), ciphertext.size(), password);
    ec::println("decypted.size(): ", decrypted.size());

    assert(decrypted.size() == plaintext.size());
    assert(!memcmp(decrypted.data(), plaintext.data(), plaintext.size()));
}

int main()
{
    test_decode();
    test_encrypt();
    test_encrypt_large();
    return 0;
}

