#include <iostream>
#include "mayowencode.cpp"
#include "pipacompiler.cpp"

int main() {
    PipaCompiler PipaCompiler;
    PipaCompiler.PipaToExe("pipalang.pipa", "piplang"); //<- your .pipa file name
    return 0;
}
