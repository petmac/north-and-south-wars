#pragma once

using s8 = char;
using s16 = short;
using s32 = long;

static_assert(sizeof(s8) == 1);
static_assert(sizeof(s16) == 2);
static_assert(sizeof(s32) == 4);

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned long;

static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
