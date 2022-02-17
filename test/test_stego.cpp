//////////////////////////////////////////////////////////////////////////////////////////
// test_stego.cpp                                                                       //
//                                                                                      //
// Tests that the user is a stegosaurus                                                 //
//////////////////////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <cmath>
#include <common/encrypt.h>
#include <common/fd.h>
#include <common/format.h>
#include <common/utf8.h>
#include <fcntl.h>
#include <fstream>
#include <random>
#include <regex>
#include <vector>

namespace ec {

}

int main()
{
    auto symbols = ec::load_symbols();

    std::string payload = 
R"(
🐲💣💩🍗😤📲🐽👹💠🔚🔝🐯🌷😾📩💎📀👍👈🆓
🌴🍗🎇🎃😊💟📃💯🐁🐘💅🛃🎰🌟🐠🎺🍞🍫🐻🎳
😼💰🔚💮🏠🎵🎷🎷💣😌💀🍹🚟🌷👨🕙📕🍑👋🔥
🎫😅🎄🐳👒🆕🌰💹🐸🕝🕘🕗🐫🏯🕘🍎📝🐹👢🎭
🛃🌲😣😢🌀🍺🔊
)";

    ec::FD fd("channel.txt", O_RDONLY);
    std::string channel = fd.read_all();

    std::string carrier = ec::conceal(payload, channel, symbols);
    ec::println(carrier);
    return 0;
}
