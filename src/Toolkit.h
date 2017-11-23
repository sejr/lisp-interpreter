#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

class Toolkit {
public:
    enum LogType {
        INFO,
        DEBUG,
        ERROR,
        WARNING,
        SUCCESS
    };

    static void console(Toolkit::LogType type, std::string message, bool showHeader);
    static void info(std::string message);
    static void warning(std::string message);
    static void error(std::string message);
    static void errorDetail(std::string message);
    static void success(std::string message);
    static void debug(std::string message);
};


void Toolkit::console(LogType type, string message, bool showHeader) {
    switch(type) {
        case INFO:
            showHeader 
                ? cout << "[\033[1;36mINFO   \033[0m] "
                : cout << "[\033[0;36m . . . \033[0m ";
                break;
        case DEBUG:
            showHeader 
                ? cout << "[\033[1;37mDEBUG  \033[0m] "
                : cout << "[\033[0;37m . . . \033[0m] ";
                break;
        case ERROR:
            showHeader 
                ? cout << "[\033[1;31mERROR  \033[0m] "
                : cout << "[\033[0;31m . . . \033[0m] ";
                break;
        case WARNING:
            showHeader 
                ? cout << "[\033[1;33mWARNING\033[0m] "
                : cout << "[\033[0;33m . . . \033[0m] ";
                break;
        case SUCCESS:
            showHeader 
                ? cout << "[\033[1;32mSUCCESS\033[0m] "
                : cout << "[\033[0;32m . . . \033[0m] ";
                break;
        default:
            showHeader 
                ? cout << "[\033[1;37mTOOLKIT\033[0m] "
                : cout << "[\033[0;37m . . . \033[0m] ";
                break;
    }

    std::cout << /* "\033[0;37m" + */  message + "\033[0m\n";
}


void Toolkit::info(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[0;36m" + message + "\033[0m\n";
}

void Toolkit::warning(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[0;33m" + message + "\033[0m\n";
}

void Toolkit::error(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[1;37;41m" + message + "\033[0m\n";
}

void Toolkit::errorDetail(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[0;31m" + message + "\033[0m\n";
}

void Toolkit::success(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[1;37;42m" + message + "\033[0m\n";
}

void Toolkit::debug (std::string message) {
    std::cout << "\033[0;37m[ . . . ]\033[0m ";
    std::cout << message;
}

#endif
