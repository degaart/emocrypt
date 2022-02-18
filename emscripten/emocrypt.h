#pragma once

#include <string>
#include <emscripten/val.h>

std::string ec_encrypt(const std::string& password, const std::string& plaintext);
std::string ec_decrypt(const std::string& password, const std::string& encoded_ciphertext);
std::string ec_conceal(const std::string& password, const std::string& plaintext, const std::string& channel);
