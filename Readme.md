# emocrypt

## Encrypts data into emojis

This program encrypts arbitrary data into an emoji soup.
It uses XSalsa20 stream cipher with Poly1305 MAC encryption, and Argon2id key derivation.

## Example output

```
🐲💣💩🍗😤📲🐽👹💠🔚🔝🐯🌷😾📩💎📀👍👈🆓
🌴🍗🎇🎃😊💟📃💯🐁🐘💅🛃🎰🌟🐠🎺🍞🍫🐻🎳
😼💰🔚💮🏠🎵🎷🎷💣😌💀🍹🚟🌷👨🕙📕🍑👋🔥
🎫😅🎄🐳👒🆕🌰💹🐸🕝🕘🕗🐫🏯🕘🍎📝🐹👢🎭
🛃🌲😣😢🌀🍺🔊
```

The above ciphertext can be decrypted using the password 'hunter2'.
The program ignores non-emoji input, so it's possible to insert normal text between the emojis in an attempt to obfuscate the encryption.

## Online version

https://degaart.github.io/emocrypt/

## Usage - command line client

### Encrypt a file

```
emocrypt -i <input-file> -o <output-file>
```

### Decrypt file

```
emocrypt -d -i <input-file> -o <output-file>
```

## Building

Requirements:

- c++ compiler with c++14 support
- cmake >= 3.15
- libsodium


### Unix/macOS

Build commands:

```
cmake -DCMAKE_BUILD_TYPE=Release -B Build -DCMAKE_INSTALL_PREFIX=/opt/emocrypt
cmake --build Build
cmake --build Build --target install
```

### Windows (Visual Studio)

- Download and extract libsodium from https://download.libsodium.org/libsodium/releases/LATEST.tar.gz
- Open libsodium.sln and build the Debug (or the Release) configuration
- Open emocrypt's source code folder in Visual Studio
- Open menu: Project > CMake Settings for emocrypt
- Add the path to libsodium in "CMake command arguments": ```-Dlibsodium_INCLUDE_DIR=D:\some-path\libsodium\include -Dlibsodium_LIBRARY=C:\some-path\libsodium-stable\Build\Debug\x64\libsodium.lib```
- Build the solution

