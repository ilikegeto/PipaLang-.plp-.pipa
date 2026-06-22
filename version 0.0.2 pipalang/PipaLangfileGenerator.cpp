#include <iostream>
#include "mayowencode.cpp"
#include "pipacompiler.cpp"

int main() {
    PipaCompiler PipaCompiler;
    PipaCompiler.PipaToExe("pipalang.pipa", "piplang"); //<- ("your .pipa file name", "your pipa name .exe (customizable)")
    return 0;
}
