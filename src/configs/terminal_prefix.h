#pragma once

#if defined(__APPLE__)
    #define TERMINAL_PRINT_WITH_COLOR false
    #define TERMINAL_RESET ""
#else
    #define TERMINAL_PRINT_WITH_COLOR true
    #define TERMINAL_RESET "\x1b[0m"
#endif

#include <iostream>
#include <string>

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

inline void printInitMessage(const std::string &version = ""){
    if(!version.empty()){
        if(TERMINAL_PRINT_WITH_COLOR){
            std::cout << "\x1b[47;30m" << "GraphIt! v" << version << TERMINAL_RESET << std::endl;
        }else{
            std::cout << "GraphIt! v" << version << std::endl;
        }
    }
}

inline void printActionPrefix(){ // White
    if(TERMINAL_PRINT_WITH_COLOR){
        std::cout << "\x1b[47;30m" << "[Action]" << TERMINAL_RESET << " ";
    }else{
        std::cout << "[Action] ";
    }
}

inline void printCanvasPrefix(){ // Yellow
    if(TERMINAL_PRINT_WITH_COLOR){
        std::cout << "\x1b[43;30m" << "[Canvas]" << TERMINAL_RESET << " ";
    }else{
        std::cout << "[Canvas] ";
    }
}

inline void printUpdatePrefix(){ // Black
    if(TERMINAL_PRINT_WITH_COLOR){
        std::cout << "\x1b[40;37m" << "(update)" << TERMINAL_RESET << " ";
    }else{
        std::cout << "(update) ";
    }
}

inline void printErrorPrefix(){ // Red
    if(TERMINAL_PRINT_WITH_COLOR){
        std::cerr << "\x1b[41;37m" << "[ERROR]" << TERMINAL_RESET << " ";
    }else{
        std::cerr << "[ERROR] ";
    }
}

inline void printGuiPrefix(){ // Blue
    if(TERMINAL_PRINT_WITH_COLOR){
        std::cout << "\x1b[44;30m" << "[GUI]" << TERMINAL_RESET << " ";
    }else{
        std::cout << "[GUI] ";
    }
}