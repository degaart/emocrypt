#include "encrypt.h"
#include "symbols.h"
#include <cassert>
#include <sodium.h>

namespace ec {

    static const auto KD_OPSLIMIT = 4;
    static const auto KD_MEMLIMIT = 1U * 1024U * 1024U; /* sorry but the default is too high for emscripten */
    static const auto KD_ALG = crypto_pwhash_argon2id_ALG_ARGON2ID13;

    Symbols load_symbols()
    {
        Symbols result;
        for(int i = 0; i < sizeof(ec::symbols) / sizeof(ec::symbols[0]); i++) {
            std::string s;
            for(int j = 0; j < 4; j++) {
                if(ec::symbols[i][j])
                    s.append(1, static_cast<char>(ec::symbols[i][j]));
            }

            result.enc[i % 256].push_back(s);
            result.dec[s] = i % 256;
        }
        return result;
    }

    std::string encode(std::mt19937& rng, const Symbols& symbols, const void* buffer, size_t size, int line_length)
    {
        std::uniform_int_distribution<int> dist1(0, 1);
        std::uniform_int_distribution<int> dist2(0, 2);

        int cur_line_length = 0;
        std::string result;
        for(const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buffer); size; ptr++, size--) {
            int value = *ptr;

            // we have two possible symbols per value
            // let's just choose the symbol to use randomly
            auto symbol_choices = symbols.enc.at(value);
            if(symbol_choices.size() == 2)
                result.append(symbol_choices.at(dist1(rng)));
            else
                result.append(symbol_choices.at(dist2(rng)));

            cur_line_length++;
            if(cur_line_length == line_length) {
                cur_line_length = 0;
                result.append("\n");
            }
        }
        return result;
    }

    byte_string decode(const Symbols& symbols, const std::string& s)
    {
        byte_string result;
        auto seq_length = [](int ch) {
            if((ch & 0xF0) == 0xF0)
                return 4;
            else if((ch & 0xE0) == 0xE0)
                return 3;
            else if((ch & 0xC0) == 0xC0)
                return 2;
            else if((ch & 0x80) == 0x80)
                return -1;
            else
                return 1;
        };

        for(auto it = s.begin(); it < s.end();) {
            int ch = static_cast<unsigned char>(*it);
            assert(ch >= 0);
            auto length = seq_length(ch);
            if(length > 0) {
                std::string sequence(it, it + length);
                auto dec_it = symbols.dec.find(sequence);
                if(dec_it != symbols.dec.end())
                    result.push_back(dec_it->second);

                it += length;
            } else {
                ++it;
            }
        }
        return result;
    }

    byte_string encrypt(const void* buffer, size_t length, const std::string& password)
    {
        size_t result_size = 
            crypto_pwhash_SALTBYTES +
            crypto_secretbox_NONCEBYTES +
            length +
            crypto_secretbox_MACBYTES;
        byte_string result(result_size, '\0');
        unsigned char* salt = &result[0];
        unsigned char* nonce = &result[0] + crypto_pwhash_SALTBYTES;
        unsigned char* ciphertext = &result[0] + crypto_pwhash_SALTBYTES + crypto_secretbox_NONCEBYTES;
        
        /* salt */
        randombytes_buf(salt, crypto_pwhash_SALTBYTES);
        
        /* key derivation */
        byte_string key(crypto_secretbox_KEYBYTES, '\0');
        int ret = crypto_pwhash(&key[0], key.size(),
                                password.data(), password.size(),
                                salt,
                                KD_OPSLIMIT,
                                KD_MEMLIMIT,
                                KD_ALG);
        if(ret)
            throw std::runtime_error("crypto_pwhash() failed");

        /* nonce */
        randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);
        ret = crypto_secretbox_easy(ciphertext,
                                    reinterpret_cast<const unsigned char*>(buffer), length,
                                    nonce,
                                    key.data());
        if(ret)
            throw std::runtime_error("crypto_secretbox_easy() failed");
        
        return result;
    }

    byte_string decrypt(const void* buffer, size_t length, const std::string& password)
    {
        byte_string result(length - crypto_pwhash_SALTBYTES - crypto_secretbox_NONCEBYTES - crypto_secretbox_MACBYTES, '\0');
        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buffer);
        const unsigned char* salt = ptr;
        const unsigned char* nonce = ptr + crypto_pwhash_SALTBYTES;
        const unsigned char* ciphertext = ptr + crypto_pwhash_SALTBYTES + crypto_secretbox_NONCEBYTES;
        
        /* key derivation */
        byte_string key(crypto_secretbox_KEYBYTES, '\0');
        int ret = crypto_pwhash(&key[0], key.size(),
                                password.data(), password.size(),
                                salt,
                                KD_OPSLIMIT,
                                KD_MEMLIMIT,
                                KD_ALG);
        if(ret)
            throw std::runtime_error("crypto_pwhash() failed");
        
        /* decrypt */
        ret = crypto_secretbox_open_easy(&result[0],
                                        ciphertext,
                                        result.size() + crypto_secretbox_MACBYTES,
                                        nonce,
                                        key.data());
        if(ret)
            throw std::runtime_error("crypto_secretbox_open_easy() failed");
        return result;
    }

}



