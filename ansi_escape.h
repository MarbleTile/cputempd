#pragma once

#define STR(x) #x
#define ESC "\x1b["

#define CURS_UP(n)          ESC STR(n) "A"
#define CURS_DOWN(n)        ESC STR(n) "B"
#define CURS_FORWARD(n)     ESC STR(n) "C"
#define CURS_BACK(n)        ESC STR(n) "D"
#define CURS_NEXT(n)        ESC STR(n) "E"
#define CURS_PREV(n)        ESC STR(n) "F"
#define CURS_HORIZ(n)       ESC STR(n) "G"
#define CURS_POS(n, m)      ESC STR(n) ";" STR(m) "H"
#define CURS_ERASE(n)       ESC STR(n) "J"
#define CURS_ERASE_LINE(n)  ESC STR(n) "K"
#define CURS_SCROLL_UP(n)   ESC STR(n) "S"
#define CURS_SCROLL_DOWN(n) ESC STR(n) "T"
#define CURS_SAVE           ESC "S"
#define CURS_LOAD           ESC "U"

