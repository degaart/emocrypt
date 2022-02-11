#include "encrypt.h"
#include "format.h"
#include "symbols.h"
#include <cassert>
#include <sodium.h>

namespace ec {

    static const auto KD_VERSION = 1U;
    static const auto KD_OPSLIMIT = 4U;
    static const auto KD_MEMLIMIT = 20U; /* 1MB (lowered to help with the webasm version) */
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
        size_t nonce_size = std::max(crypto_pwhash_SALTBYTES, crypto_secretbox_NONCEBYTES);
        size_t result_size = nonce_size + 4 + length + crypto_secretbox_MACBYTES;
        byte_string result(result_size, '\0');
        unsigned char* nonce = &result[0];
        unsigned char* version = nonce + nonce_size;
        unsigned char* opslimit = version + sizeof(unsigned char);
        unsigned char* memlimit = opslimit + sizeof(unsigned char);
        unsigned char* algo = memlimit + sizeof(unsigned char);
        unsigned char* ciphertext = algo + sizeof(unsigned char);
        
        /* salt */
        randombytes_buf(nonce, nonce_size);

        /* Params */
        assert(KD_MEMLIMIT > 1 && KD_MEMLIMIT < 64);
        *version = KD_VERSION ^ nonce[0];
        *opslimit = KD_OPSLIMIT ^ nonce[1];
        *memlimit = KD_MEMLIMIT ^ nonce[2];
        *algo  = KD_ALG ^ nonce[3];
        
        /* key derivation */
        byte_string key(crypto_secretbox_KEYBYTES, '\0');
        int ret = crypto_pwhash(&key[0], key.size(),
                                password.data(), password.size(),
                                nonce,
                                KD_OPSLIMIT,
                                (1 << KD_MEMLIMIT),
                                KD_ALG);
        if(ret)
            return byte_string();

        /* nonce */
        ret = crypto_secretbox_easy(ciphertext,
                                    reinterpret_cast<const unsigned char*>(buffer), length,
                                    nonce,
                                    key.data());
        if(ret)
            return byte_string();
        return result;
    }

    byte_string decrypt(const void* buffer, size_t length, const std::string& password)
    {
        size_t nonce_size = std::max(crypto_pwhash_SALTBYTES, crypto_secretbox_NONCEBYTES);
        assert(length >= nonce_size + 4 + crypto_secretbox_MACBYTES);
        
        const unsigned char* nonce = reinterpret_cast<const unsigned char*>(buffer);
        const unsigned char* version = nonce + nonce_size;
        const unsigned char* opslimit = version + sizeof(unsigned char);
        const unsigned char* memlimit = opslimit + sizeof(unsigned char);
        const unsigned char* algo = memlimit + sizeof(unsigned char);
        const unsigned char* ciphertext = algo + sizeof(unsigned char);

        /* Check version */
        int v_version = *version ^ nonce[0];
        int v_opslimit = *opslimit ^ nonce[1];
        int v_memlimit = *memlimit ^ nonce[2];
        int v_algo = *algo ^ nonce[3];
        if(v_version != KD_VERSION)
            return byte_string();
        else if(v_opslimit == 0 || v_opslimit > 256)
            return byte_string();
        else if(v_memlimit < 1 || v_memlimit > 64)
           return byte_string();
    
        /* key derivation */
        byte_string key(crypto_secretbox_KEYBYTES, '\0');
        int ret = crypto_pwhash(&key[0], key.size(),
                                password.data(), password.size(),
                                nonce,
                                v_opslimit,
                                1 << v_memlimit,
                                v_algo);
        if(ret)
            return byte_string();
        
        /* decrypt */
        size_t result_size = length - nonce_size - 4 - crypto_secretbox_MACBYTES;
        byte_string result(result_size, '\0');
        ret = crypto_secretbox_open_easy(&result[0],
                                        ciphertext,
                                        result.size() + crypto_secretbox_MACBYTES,
                                        nonce,
                                        key.data());
        if(ret)
            return byte_string();
        return result;
    }

}



