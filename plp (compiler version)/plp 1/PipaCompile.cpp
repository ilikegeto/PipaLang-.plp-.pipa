#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <memory>
#include <fstream>

enum class TokenType{
    KEYWORD,
    IDENTIFIER,
    ASSIGN,
    STRING,
    NUMBER,
    SEMICOLON,
    DOT,
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    UNKOWN
};

struct Token{
    TokenType type;
    std::string value;
};

bool isKeyword(const std::string& str){
    return (str == "str" || str == "int" || str == "double" || str == "float" ||
    str == "char" || str == "scr" || str == "in" || str == "scr_nextl" || str == "array_str" ||
    str == "array_int" || str == "flow");
}

std::vector<Token> Tokenize(const std::string& source){
    std::vector<Token> tokens;
    size_t pos = 0;
    size_t length = source.length();

    while (pos < length){
        char current = source[pos];

        if (std::isalpha(current) || current == '_'){
            std::string word = "";
            while (pos < length && (std::isalnum(source[pos]) || source[pos] == '_')){
                word += source[pos];
                pos++;
            }
            if (isKeyword(word)){tokens.push_back({TokenType::KEYWORD, word});}
            else{tokens.push_back({TokenType::IDENTIFIER, word});}
            continue;
        }
        if (current == '"'){
            std::string str_val = "\"";
            pos++;
            while (pos < length && source[pos] != '"'){
                str_val += source[pos];
                pos++;
            }
            str_val += "\"";
            pos++;
            tokens.push_back({TokenType::STRING, str_val});
            continue;
        }
        if (std::isdigit(current)){
            std::string num_val = "";
            while (pos < length && (std::isdigit(source[pos]) || source[pos] == '.')){
                num_val += source[pos];
                pos++;
            }
            tokens.push_back({TokenType::NUMBER, num_val});
            continue;
        }


        if (std::isspace(current)){pos++; continue;}
        if (source[pos] == '='){tokens.push_back({TokenType::ASSIGN, "="}); pos++; continue;}
        if (source[pos] == ';'){tokens.push_back({TokenType::SEMICOLON, ";"}); pos++; continue;}
        if (source[pos] == '.'){tokens.push_back({TokenType::DOT, "."}); pos++; continue;}
        if (source[pos] == '('){tokens.push_back({TokenType::LPAREN, "("}); pos++; continue;}
        if (source[pos] == ')'){tokens.push_back({TokenType::RPAREN, ")"}); pos++; continue;}
        if (source[pos] == '{'){tokens.push_back({TokenType::LBRACE, "{"}); pos++; continue;}
        if (source[pos] == '}'){tokens.push_back({TokenType::RBRACE, "}"}); pos++; continue;}
        if (source[pos] == '+'){tokens.push_back({TokenType::UNKOWN, "+"}); pos++; continue;}
        if (source[pos] == '-'){tokens.push_back({TokenType::UNKOWN, "-"}); pos++; continue;}
        if (source[pos] == '*'){tokens.push_back({TokenType::UNKOWN, "*"}); pos++; continue;}
        if (source[pos] == '/'){tokens.push_back({TokenType::UNKOWN, "/"}); pos++; continue;}
        tokens.push_back({TokenType::UNKOWN, std::string(1, current)});
        pos++;
    }
    return tokens;
}

class ASTnode{
    public:
        virtual ~ASTnode() = default;
};
class VarDeclNode : public ASTnode {
    public:
        std::string type;
        std::string name;
        std::string value;

        VarDeclNode(std::string t, std::string n, std::string v):
            type(t), name(n), value(v) {}
};
class ProgramNode : public ASTnode {
    public:
        std::vector<std::unique_ptr<ASTnode>> statements;
        std::vector<std::string> declaredvar;
};
//idk vvv
class PrintNode : public ASTnode {
public:
    std::string target;
    bool isNewLine;
    PrintNode(std::string t, bool nl) : target(t), isNewLine(nl) {}
};
class AddNode : public ASTnode {
public:
    std::string filename;
    AddNode(std::string file) : filename(file) {}
};


class parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;

    Token peek() {
        if (current < tokens.size()) return tokens[current];
        return {TokenType::UNKOWN, ""};
    }
    Token consume(TokenType expectedType) {
        if (peek().type == expectedType){return tokens[current++];}
        //std::cout << "Token [Error][[Syntax error]]\n";
        std::cout << "Syntax Error: expectedType [" << static_cast<int>(expectedType) 
              << "], but [" << peek().value << "] and type [" 
              << static_cast<int>(peek().type) << "]\n";
        exit(1);
    }
public:
    parser(std::vector<Token> scrTokens) : tokens(scrTokens){}
    std::unique_ptr<ProgramNode> parseProgram(){
        auto program = std::make_unique<ProgramNode>();
        if (tokens[current].value != "main_code"){std::cout << "Compiler [Error][[can't find \"main_code()\"]]\n"; exit(1);}
        current++;
        if (tokens[current].value != "("){std::cout << "Compiler [Error][[can't find \"main_code()\" -> (]]\n"; exit(1);}
        current++;
        if (tokens[current].value != ")"){std::cout << "Compiler [Error][[can't find \"main_code()\" -> )]]\n"; exit(1);}
        current++;
        if (tokens[current].value != "{"){std::cout << "Compiler [Error][[can't find \"main_code()\" -> {]]\n"; exit(1);}
        current++;
        // idk what is that ^^^
        while (current < tokens.size()){
            if (peek().type == TokenType::KEYWORD){
                if (current + 1 < tokens.size() && tokens[current + 1].type == TokenType::IDENTIFIER){
                    program->declaredvar.push_back(tokens[current + 1].value);
                }
                program->statements.push_back(parseVarDecl());
            }else if (peek().type == TokenType::IDENTIFIER && peek().value == "value"){
                consume(TokenType::IDENTIFIER);
                consume(TokenType::DOT);
                Token funcToken = consume(TokenType::KEYWORD);
                bool isNewLine = (funcToken.value == "scr_nextl");
                consume(TokenType::LPAREN);
                std::string printTarget = "";

                while (current < tokens.size() && tokens[current].type != TokenType::RPAREN){
                    printTarget += peek().value + " ";
                    current++;
                }

                /*if (peek().type == TokenType::IDENTIFIER || peek().type == TokenType::STRING || peek().type == TokenType::NUMBER){
                    printTarget = peek().value;
                    current++;
                }*/
                consume(TokenType::RPAREN);
                consume(TokenType::SEMICOLON);
                program->statements.push_back(std::make_unique<PrintNode>(printTarget, isNewLine));
            }
            else{
                if (tokens[current].value == "}") {break;}
                current++;
            }
        }
        if (current >= tokens.size() || tokens[current].value != "}"){std::cout << "Compiler [Error][[can't find \"main_code()\" -> }]]\n"; exit(1);}
        current++;
        return program;
    }
private:
    std::unique_ptr<VarDeclNode> parseVarDecl(){
        Token typetoken = consume(TokenType::KEYWORD);
        Token nameToken = consume(TokenType::IDENTIFIER);
        consume(TokenType::ASSIGN);
        std::string val = "";
        while (current < tokens.size() && tokens[current].type != TokenType::SEMICOLON){
            //if (tokens[current].type == TokenType::STRING){val += "\"" + tokens[current].value + "\"";}
            //else{val += tokens[current].value + " ";}
            val += tokens[current].value + " ";
            current++;
        }
        
        if (val.empty()) {
            std::cout << "Token [Error][[variable doesn't have value]] -> " << nameToken.value << "\n";
            exit(1);
        }
        consume(TokenType::SEMICOLON);
        return std::make_unique<VarDeclNode>(typetoken.value, nameToken.value, val);
    }
};
class codeGenerator {
public:
    void generate(ProgramNode* program, const std::string& outputFilename){
        std::ofstream ppfile(outputFilename);
        ppfile << "#include <iostream>\n";
        ppfile << "#include <string>\n";                            
        ppfile << "#include \"plplibrary.h\"\n\n";

        ppfile << "void main_code(){\n";

        for (const auto& node : program->statements){
            if (auto varDecl = dynamic_cast<VarDeclNode*>(node.get())){
                std::string cppType = varDecl->type;
                if (cppType == "str") {
                    cppType = "std::string";
                    if (varDecl->value.front() != '"'){ppfile << "    " << cppType << " " << varDecl->name << " = \"" << varDecl->value << "\";\n";}
                    else {ppfile << "    " << cppType << " " << varDecl->name << " = " << varDecl->value << ";\n";}
                } else {ppfile << "    " << cppType << " " << varDecl->name << " = " << varDecl->value << ";\n";}
            }
            else if (auto Printnode = dynamic_cast<PrintNode*>(node.get())){
                std::string scrfunc = Printnode->isNewLine? "    value.scr_nextl(" : "    value.scr(";
                ppfile << scrfunc;
                if (!Printnode->target.empty()){
                    /*bool isvar = false;
                    for (const auto& varname : program->declaredvar) {
                        if (Printnode->target == varname){
                            isvar = true;
                            break;
                        }
                    }
                    if (isvar){ppfile << Printnode->target;}
                    else{ppfile << "\"" << Printnode->target << "\"";}*/
                    ppfile << Printnode->target;
                } else {ppfile << "\"\"";}
                ppfile << ");\n";
            }
        }

        ppfile << "}\n";
        ppfile << "int main() {\n";
        ppfile << "    main_code();\n";
        ppfile << "    return 0;\n";
        ppfile << "}\n";
        ppfile.close();
        std::cout << "Compiler [Running][[Success create your file]] -> " << outputFilename << "\n";
    }
};


/*int main() {
    std::string pipalangCode = 
        "main_code(){"
        "    str halo = \"Sintaks formal PipaLang sukses, bung!\";"
        "    value.scr_nextl(halo);"
        "}";

    std::vector<Token> tokens = Tokenize(pipalangCode);
    for (const auto& t : tokens) {
        std::cout << "Token: [" << t.value << "]\n";
    }
    while (pos < length && source[pos] != '"'){r parserInstance(tokens);
    std::unique_ptr<ProgramNode> ast = parserInstance.parseProgram();

    codeGenerator generator;
    generator.generate(ast.get(), "output_pipalang.cpp");

    std::cout << "\ncompile your code -> here (.exe)...\n";

    int compileResult = std::system("g++ output_pipalang.cpp -o plpPrev.exe");

    if (compileResult == 0) {
        std::cout << "finish compile your code yipe\n";
        std::cout << "run................................................\n";
        std::cout << "-------------------------------------------\n";
        
        std::system(".\\plpPrev.exe");
        
        std::cout << "\n-------------------------------------------\n";
    } else {
        std::cout << "i can't compile your code\n";
    }

    return 0;
}*/
