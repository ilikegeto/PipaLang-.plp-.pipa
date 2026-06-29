# PipaLang-my-own-language
---
### **What is Pipalang?**
1. pipalang is a language that's have two different languages types
   * compiler or `.plp`
   * interpenter or `.pipa`
2. this language is code with C++ or `.cpp`
3. for speed these language have near same speed and that's fast like C++
4. for compiler version the language is sthill work in progress and for interpenter vesion now is 0.1.0
---
### **NOTE FOR PLP**
you know this error? <br>
`Syntax Error: expectedType [7], but [.] and type [6]` <br>
so let me tell you what is that error <br>
`expectedType [7]` this tell you what type you must add<br>
`but [.] and type [6]` and this what you are add or type <br>
so this what is that `expectedType [7]` mean
```cpp
enum class TokenType{
    KEYWORD, //<- type[0]
    IDENTIFIER, //<- type[1]
    ASSIGN, //<- type[2]
    STRING, //<- type [3]
    NUMBER, //<- type [4]
    SEMICOLON, //<- type [5]
    DOT, //<- type [6]
    LPAREN //<- type [7], RPAREN, //<- type [8]
    LBRACE //<- type [9], RBRACE, //<- type [10]
    UNKOWN //<- type [11]
};
```
that mean `expectedType [7]` = `expectedType [(]`
