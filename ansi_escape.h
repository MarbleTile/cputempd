#pragma once

#define STR(x) #x
#define ESC "\x1b["

#define ERASE_TO_END    0
#define ERASE_TO_BEGIN  1
#define ERASE_ALL       2

#define CURS_UP             ESC "%d" "A"
#define CURS_DOWN           ESC "%d" "B"
#define CURS_FORWARD        ESC "%d" "C"
#define CURS_BACK           ESC "%d" "D"
#define CURS_NEXT           ESC "%d" "E"
#define CURS_PREV           ESC "%d" "F"
#define CURS_HORIZ          ESC "%d" "G"
#define CURS_POS            ESC "%d" ";" "%d" "H"
#define CURS_ERASE          ESC "%d" "J"
#define CURS_ERASE_LINE     ESC "%d" "K"
#define CURS_SCROLL_UP      ESC "%d" "S"
#define CURS_SCROLL_DOWN    ESC "%d" "T"
#define CURS_SAVE           ESC "s"
#define CURS_LOAD           ESC "u"
#define CURS_REPORT         ESC "6n"
#define CURS_REPORT_SCAN    ESC "%d;%dr"

#define CURS_RESET          ESC "0m"
#define CURS_BOLD           ESC "1m"
#define CURS_ITALIC         ESC "3m"
#define CURS_UNDERLINE      ESC "4m"
#define CURS_BLACK          ESC "30m"
#define CURS_RED            ESC "31m"
#define CURS_GREEN          ESC "32m"
#define CURS_YELLOW         ESC "33m"
#define CURS_BLUE           ESC "34m"
#define CURS_MAGENTA        ESC "35m"
#define CURS_CYAN           ESC "36m"
#define CURS_WHITE          ESC "37m"
#define CURS_BKG_BLACK      ESC "40m"
#define CURS_BKG_RED        ESC "41m"
#define CURS_BKG_GREEN      ESC "42m"
#define CURS_BKG_YELLOW     ESC "43m"
#define CURS_BKG_BLUE       ESC "44m"
#define CURS_BKG_MAGENTA    ESC "45m"
#define CURS_BKG_CYAN       ESC "46m"
#define CURS_BKG_WHITE      ESC "47m"

