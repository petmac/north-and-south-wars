#include "copper.h"

#include "custom.h"

#include "gcc8_c_support.h" // offsetof

// put copperlist into chip mem so we can use it without copying
const UWORD copper2[] __attribute__((section(".MEMF_CHIP"))) = {
    offsetof(Custom, color[0]),
    0x0000,
    0x4101,
    0xff00,
    offsetof(Custom, color[0]),
    0x0111, // line 0x41
    0x4201,
    0xff00,
    offsetof(Custom, color[0]),
    0x0222, // line 0x42
    0x4301,
    0xff00,
    offsetof(Custom, color[0]),
    0x0333, // line 0x43
    0x4401,
    0xff00,
    offsetof(Custom, color[0]),
    0x0444, // line 0x44
    0x4501,
    0xff00,
    offsetof(Custom, color[0]),
    0x0555, // line 0x45
    0x4601,
    0xff00,
    offsetof(Custom, color[0]),
    0x0666, // line 0x46
    0x4701,
    0xff00,
    offsetof(Custom, color[0]),
    0x0777, // line 0x47
    0x4801,
    0xff00,
    offsetof(Custom, color[0]),
    0x0888, // line 0x48
    0x4901,
    0xff00,
    offsetof(Custom, color[0]),
    0x0999, // line 0x49
    0x4a01,
    0xff00,
    offsetof(Custom, color[0]),
    0x0aaa, // line 0x4a
    0x4b01,
    0xff00,
    offsetof(Custom, color[0]),
    0x0bbb, // line 0x4b
    0x4c01,
    0xff00,
    offsetof(Custom, color[0]),
    0x0ccc, // line 0x4c
    0x4d01,
    0xff00,
    offsetof(Custom, color[0]),
    0x0ddd, // line 0x4d
    0x4e01,
    0xff00,
    offsetof(Custom, color[0]),
    0x0eee, // line 0x4e
    0x4f01,
    0xff00,
    offsetof(Custom, color[0]),
    0x0fff, // line 0x4e
    0xffff,
    0xfffe // end copper list
};
