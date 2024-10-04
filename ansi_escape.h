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

#define CURS_RESET          ESC "0M"
#define CURS_BOLD           ESC "1M"
#define CURS_ITALIC         ESC "3M"
#define CURS_UNDERLINE      ESC "4M"
#define CURS_BLACK          ESC "30M"
#define CURS_RED            ESC "31M"
#define CURS_GREEN          ESC "32M"
#define CURS_YELLOW         ESC "33M"
#define CURS_BLUE           ESC "34M"
#define CURS_MAGENTA        ESC "35M"
#define CURS_CYAN           ESC "36M"
#define CURS_WHITE          ESC "37M"
#define CURS_BKG_BLACK      ESC "40M"
#define CURS_BKG_RED        ESC "41M"
#define CURS_BKG_GREEN      ESC "42M"
#define CURS_BKG_YELLOW     ESC "43M"
#define CURS_BKG_BLUE       ESC "44M"
#define CURS_BKG_MAGENTA    ESC "45M"
#define CURS_BKG_CYAN       ESC "46M"
#define CURS_BKG_WHITE      ESC "47M"

