#include <iostream>
#include "mayowencode.cpp" //<- #include "McodeBase code or name you save when copy McodeBase"

int main() {
    TerminalCode();// <- write code in terminal 
    //                exit terminal = -@~commandTerminal_line=>(Mcodeterminalcommandeditor.terminallinecode <--->> ~~terminalline.exitterminaleditor(true: exit terminal editor)->executecommand(true)<<->@terminallinecodecommand.quiteditor;);McodeLauncher.command@McodeTerminalcommand:exitOrQuiteditor(terminal.stop(run.editor))-

    code("int a = 10;");        //|/
    code("int b = 12;");        //| /
    code("value.scr a + b;");   //|  use code() to run your code
    code("value.scr a - b;");   //|  code("value.scr a + b;"); <- print, code("int a = 10;"); <- variable
    code("value.scr a * b;");   //| /
    code("value.scr a / b;");   //|/

    return 0;
} 

