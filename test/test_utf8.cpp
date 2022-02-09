#include <common/format.h>
#include <common/utf8.h>
#include <cassert>

using namespace ec;

void test_utf8_seq_length()
{
    ec::println("*** Testing utf8_seq_length()");

    assert(utf8_seq_length(0x24) == 1);
    assert(utf8_seq_length(0xC2) == 2);
    assert(utf8_seq_length(0xE0) == 3);
    assert(utf8_seq_length(0xE2) == 3);
    assert(utf8_seq_length(0xED) == 3);
    assert(utf8_seq_length(0xF0) == 4);
    assert(utf8_seq_length(0xF0) == 4);
    assert(utf8_seq_length(0xA0) == -1);
}

int main()
{
    test_utf8_seq_length();
}
