#pragma once

#define TERMINAL_RESET "\x1b[0m"

#include <iostream>

/*
    ANSI Escape Sequences - @fnky
    https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

    Color Name 	Foreground Color Code 	Background Color Code
        Black 	        30 	                    40
        Red 	        31 	                    41
        Green 	        32 	                    42
        Yellow 	        33 	                    43
        Blue 	        34 	                    44
        Magenta         35 	                    45
        Cyan 	        36 	                    46
        White 	        37 	                    47
        Default         39 	                    49

    # Set style to bold, red foreground.
    \x1b[1;31mHello
    # Set style to dimmed white foreground with red background.
    \x1b[2;37;41mWorld

*/

inline void printActionPrefix(){ // White
    std::cout << "\x1b[47;30m" << "[Action]" << TERMINAL_RESET << " ";
}

inline void printCanvasPrefix(){ // Yellow
    std::cout << "\x1b[43;30m" << "[Canvas]" << TERMINAL_RESET << " ";
}

inline void printUpdatePrefix(){ // Black
    std::cout << "\x1b[40;37m" << "(update)" << TERMINAL_RESET << " ";
}