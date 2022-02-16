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

## Usage

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

- c++ compiler with c++17 support
- cmake >= 3.15
- libsodium

Build commands:

```
cmake -DCMAKE_BUILD_TYPE=Release -B Build
cmake --build Build
cmake --build Build --target install
```

