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
The program ignores non-emoji input, so it is possible to conceal the encrypted
message inside another text (steganography):

```
I'd just like 🐃to interject for🔒 a moment. 🏨What you're refering🈵 to as
🍑Linux, is in🔙 fact, GNU/Linux, or🐱 as I've 🏩recently taken to📚 calling
it, 🔪GNU plus Linux.🍒 Linux is 🍟not an operating🛃 system unto itself,😁
but rather 👊another free component🐁 of a 👅fully functioning GNU😡 system
made 📏useful by the🍔 GNU corelibs, shell💆 utilities and 🌸vital system
components🔡 comprising a 😌full OS as🕐 defined by 🔎POSIX.

Many computer 🕐users run a🈸 modified version 💖of the GNU💋 system every
💾day, without realizing😸 it. Through 💡a peculiar turn 🍃of events, the🆑
version of ⛄GNU which is✋ widely used 🚛today is often💖 called Linux, 📇
and many of 🎰its users are🎯 not aware 🕠that it is📈 basically the 📗GNU
system, developed📦 by the GNU🕔 Project.

There 🙍really is a🎰 Linux, and 😹these people are🐧 using it, 🏁but it is
💜 just a part📠 of the 🎐system they use.📬 Linux is 🐶the kernel: the🔍
program in 💏the system that🍜 allocates the machine's🌀 resources to 💘the
other programs💂 that you 📳run. The kernel🔰 is an 💩essential part of 🔡an
operating system,🍕 but useless 👴by itself; it👎 can only 🕐function in
the🕜 context of 🐧a complete operating 🚫system. Linux is🐥 normally used
💟in combination with🚟 the GNU 🗽operating system: the🎠 whole system 🕟is
basically GNU 🌺with Linux added,🌟 or GNU/Linux. 🙅All the so-called📛
Linux distributions 📱are really distributions🔆 of GNU/Linux!🔱
```



## Online version

https://degaart.github.io/emocrypt/

## Usage - command line client

### Encrypt a file

```
emocrypt -i <input-file> -o <output-file>
```

### Encrypt then conceal a file

```
emocrypt -i <input-file> -o <output-file> -c <file-to-use-when-concealing-encrypted-text>
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

