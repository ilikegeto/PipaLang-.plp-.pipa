#ifndef PLPLIBRARY_H
#define PLPLIBRARY_H

#include <iostream>
#include <string>
class Pipascr {
public:
    void scr(const std::string& text){std::cout << text;}
    void scr(int number){std::cout << number;}

    void scr_nextl(const std::string& text){std::cout << text << std::endl;}
    void scr_nextl(int number){std::cout << number << std::endl;}
};

static Pipascr value;
#endif
