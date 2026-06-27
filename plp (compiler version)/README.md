#WELCOME TO README.md
---
#SOME NOTE

* this language is still Work In Progress(WiP)
* this version still 6.3.0 friendly so don't worry but on newest version maybe you must update you C++ version
---
#HOW I RUN MY .PLP FILE?

1. first thing you must to do is: go to `plptestcpp.cpp` and run or compile it so `PipaCompile.cpp` can update your `plptestcpp.cpp`
2. to run it write
    ```bash
    .\plptestcpp <Your-Plp-File-Name.plp>
     ```
    example:
    ```bash
    .\plptestcpp plptest.plp
    ```
   
11. just wait until your file got run :D
---
#HELLO WORLD

to print `hello world` simply use this code
```cpp
add "plplibrary.h";

main_code(){
    str helloWorld = "Hello World";
    value.scr_nextl(helloWorld);
    //str = string(variables)
    //value.scr = cout with no endl
    //value.scr_nextl = cout with endl
}
```
or you can use this
```cpp
add "plplibrary.h";

main_code(){
    value.scr_nextl("hello World"); 
    //value.scr = cout with no endl
    //value.scr_nextl = cout with endl
}
```
---
#END AND NOTE
> C++ version -> 6.3.0
> discord server `Together C & C++`
> forum:
> ```bash
> https://discord.com/channels/331718482485837825/1520296776345587824
> ```
