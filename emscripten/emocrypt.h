#pragma once

#include <string>
#include <emscripten/val.h>

std::string ec_encrypt(const std::string& password, const std::string& plaintext);
emscripten::val ec_decrypt(const std::string& password, const std::string& encoded_ciphertext);

