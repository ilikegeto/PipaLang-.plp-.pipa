#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <map>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class CpipaLang {
    private:
        map<string, int> McodeMemory;
        map<string, string> PPstrMemory;
        void parser(vector<string> TokenList,vector<string> valueList, string CodeTintung);
        string tokenTot(string kata);
        void evaluator(vector<string> valueList);
        vector<string> valueOrdering(string rawvalue);
        vector<string> createTokenList(vector<string> valueList);

    public:
        void evaluatorscr(string tokentype, string tokenval);
        void code(string CtopOneCode);
        void TerminalCode();
        //to other file
        void scr(string Ptype, string Pval){
            evaluatorscr(Ptype, Pval);
        }

};


vector<string> ppscript;
bool isWriting = false;

string version = "0.0.2 startup[ Mcode -> CPipaLang]";

string CpipaLang::tokenTot(string kata){
    if(kata.empty()) return "Empty Code";
    if (kata == ";") return "Symbol";
    if (kata == "=") return "Operator";
    if(kata == "(" || kata == ")") return "Symbol";
    if(kata == "{" || kata == "}") return "Symbol";

    if (kata.length() >= 2 && kata.front() == '"' && kata.back() == '"'){
        return "String";
    }
    if (kata == "int" || kata == "float" || kata == "char" || kata == "str" || kata == "if" || kata == "return" || kata == "value.scr") {
       return "Keyword";
    } else if (isdigit(kata[0])) {
        return "Number";
    } else {
        return "Identifier";
    }
}

void CpipaLang::evaluator(vector<string> valueList){
    string keywordtype = valueList[0];
    string varName = valueList[1];
    if (valueList.size() < 4) return;

    if (keywordtype == "str") {
        string rawstr = valueList[3];
        //cout << "[DEBUG EVALUATOR] rawstr terdeteksi: " << rawstr << " | Ukuran: " << rawstr.length() << "\n";

        if (rawstr == "value.in" /*&& valueList.size() >= 6 && valueList[4] == "." && valueList[5] == "in"*/){
            string userInstr;
            cout << "Token [Input] <->> " << varName << " str\n";
            getline(cin, userInstr);

            PPstrMemory[varName] = userInstr;
            //cout << "[DEBUG EVALUATOR] BERHASIL MASUK MEMORI! PPstrMemory[" << varName << "] = " << PPstrMemory[varName] << "\n";
            return;
        }

        if (rawstr.length() >= 2 && rawstr.front() == '"' && rawstr.back() == '"'){
            string strval = rawstr.substr(1, rawstr.length() - 2);
            PPstrMemory[varName] = strval;
            //cout << "[DEBUG EVALUATOR] BERHASIL MASUK MEMORI! PPstrMemory[" << varName << "] = " << PPstrMemory[varName] << "\n";
            return;
        } else if (PPstrMemory.find(rawstr) != PPstrMemory.end()){PPstrMemory[varName] = PPstrMemory[rawstr]; return;}
        else {cout << "Token [Error][[Sring variable not found]]" << rawstr << "\n"; return;} 
    }

    int finalValue = 0;
    string firstr = valueList[3];

    if (firstr == "value.in" /*&& valueList.size() >= 6 && valueList[4] == "." && valueList[5] == "in"*/)
    {
        int userinint;
        cout << "Token [Input] <->> " << firstr << " int\n";
        cin >> userinint;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        McodeMemory[varName] = userinint;
        return;
    }

    if (isdigit(firstr[0])){
        finalValue = stoi(firstr);
    } else {
        if (McodeMemory.find(firstr) != McodeMemory.end()){
            finalValue = McodeMemory[firstr];
        } else {
            cout << "Token [Error][[Variable Not found]]" << firstr << "\n";
            return;
        }
    }
    for (int i = 4; i < valueList.size(); i += 2){
        if (valueList[1] == ";" || (i + 1) >= valueList.size()){
            break;
        }
        string op = valueList[i];
        string nextstr = valueList[i + 1];
        int nextvalue = 0;

        if (isdigit(nextstr[0])){
            nextvalue = stoi(nextstr);
        } else if (nextstr != ";"){
            if (McodeMemory.find(nextstr) != McodeMemory.end()){
                nextvalue = McodeMemory[nextstr];
            } else{
                cout << "Token [Error][[Variable Not found]]" << firstr << "\n";
                return;
            }
        } else{
            break;
        }

        if (op == "+") finalValue += nextvalue;
        else if (op == "-") finalValue -= nextvalue;
        else if (op == "*") finalValue *= nextvalue;
        else if (op == "/"){
            if (nextvalue != 0) finalValue /= nextvalue;
            else {
                cout << "Token [Error][[Can't divide .../0]]";
                return;
            }
        }
    }
    McodeMemory[varName] = finalValue;
    if (varName != "TUMBAL") {
        //cout << "[MOC Memory] " << varName << " berhasil disimpan dengan nilai: " << McodeMemory[varName] << "\n";
    }
    //cout << "Mcode var" << varName << McodeMemory[varName] << "\n";
}

void CpipaLang::evaluatorscr(string tokentype, string tokenval){
    if (tokentype == "Number"){
        cout << tokenval <<"\n";
    } else if (tokentype == "String"){
        if (tokenval.length() >= 2 && tokenval.front() == '"' && tokenval.back() == '"'){
            string strscr = tokenval.substr(1, tokenval.length() - 2);
            cout << strscr << "\n";
        } else if (PPstrMemory.find(tokenval) != PPstrMemory.end()) {cout << PPstrMemory[tokenval] <<"\n";}
        else{
            string strscr = tokenval.substr(1, tokenval.length() - 2);
            cout << strscr << "\n";
        }
        
    } else if (tokentype == "Identifier"){
        if (McodeMemory.find(tokenval) != McodeMemory.end()){cout << McodeMemory[tokenval] << "\n";}
        else if (PPstrMemory.find(tokenval) != PPstrMemory.end()){cout << PPstrMemory[tokenval] << "\n";}
        else {cout << "Token [Error][[Variable Not found]]\n";}
    }
}

void CpipaLang::parser(vector<string> TokenList,vector<string> valueList, string CodeTintung) {

    //cout << "[DEBUG] Total Token: " << TokenList.size() << "\n";
    if (!TokenList.empty()) {
        //cout << "[DEBUG] Token Pertama: " << TokenList.front() << " (" << valueList.front() << ")\n";
        //cout << "[DEBUG] Token Terakhir: " << TokenList.back() << " (" << valueList.back() << ")\n";
    }

    if (TokenList.empty() || valueList.empty()) return;
    //cout << "Parser [Start] <<> \n";
    bool yesYes = true;

    if (valueList[0] == "value.scr"){
        if (TokenList.back() != "Symbol" || valueList.back() != ";"){cout << "Token [Error][[(;)Symbol Not found]]\n"; return;}
        if (TokenList.size() < 3){cout << "Token [Error][[No argument to screen]]\n"; return;}
        if (TokenList.size() == 5){
            if (valueList[1] == "(" && valueList[3] == ")"){
                string strtype = TokenList[2];
                string strvalue = valueList[2];
                
                if (strtype == "Identifier" || strtype == "Number" || strtype == "String"){evaluatorscr(strtype, strvalue);}
                else {cout << "Token [Error][[Data not supported for value.scr()]]" << TokenList[1] <<"\n";}
            }
        }     
        else if (TokenList.size() == 3){
            if (TokenList[1] != "Identifier" && TokenList[1] != "Number" && TokenList[1] != "String"){cout << "Token [Error][[Data not supported for value.scr()]]" << TokenList[1] <<"\n";}
            else {evaluatorscr(TokenList[1], valueList[1]);};  
        }
        else if (TokenList.size() > 3){
            //cout << "Token [Running][" << CodeTintung << "]\n";

            vector<string> doMathlist;

            doMathlist.push_back(valueList[0]);
            doMathlist.push_back("TUMBAL");
            doMathlist.push_back("=");
            for (int i = 1; i < valueList.size(); i++){
                if (valueList[i] != "(" && valueList[i] != ")" && valueList[i] != ";"){doMathlist.push_back(valueList[i]);}    
            }
            doMathlist.push_back(";");

            evaluator(doMathlist);
            evaluatorscr("Identifier", "TUMBAL");
            McodeMemory.erase("TUMBAL");
        }
        
    } else if (TokenList[0] == "Identifier"){

        if (TokenList.size() > 3 || valueList[1] != "="){cout << "Token [Error][[Invalid Identifier]]\n";return;}
        
        string originalvar = valueList[0];
        vector<string> domathlist;
        domathlist.push_back("mathmessage");
        domathlist.push_back("TUMBALV2");
        domathlist.push_back("=");

        for (int i = 0; i < valueList.size(); i++){domathlist.push_back(valueList[i]);}

        evaluator(domathlist);
        McodeMemory[originalvar] = McodeMemory["TUMBALV2"];
        McodeMemory.erase("TUMBALV2");
    } else if (TokenList[0] == "Keyword"){
        if (TokenList.size() >= 5){
            if(TokenList[0] != "Keyword") yesYes = false;
            else if(TokenList[1] != "Identifier") yesYes = false;
            else if(TokenList[2] != "Operator" || valueList[2] != "=") yesYes = false;
            
            else if (TokenList.back() != "Symbol" || valueList.back() != ";"){
                cout << "Token [Error][[(;)Symbol Not found]]\n";
                yesYes = false;
            }

            else if (valueList[3] == "value.in"){
                yesYes = true;
            } else {
                if(TokenList[TokenList.size() - 2] != "Number" && TokenList[TokenList.size() - 2] != "Identifier" && TokenList[TokenList.size() - 2] != "String") yesYes = false;
            }

            if (yesYes){/*cout << "Token [Running][" << CodeTintung << "]\n";*/ evaluator(valueList);}
            else {cout << "Token [Error][[Syntax Error or Variable Error]]\n";}
        } else {cout << "Token [Error][[Too short for declarate]]\n";}
    } else {cout << "Token [Error][[Unknown syntax]]" << valueList[0] << "\n";}
    //cout << "Parser [End] <<> \n";
}

void CpipaLang::code(string CtopOneCode){
    while (!CtopOneCode.empty() && (CtopOneCode.back() == ' ' || CtopOneCode.back() == '\r' || CtopOneCode.back() == '\n')){CtopOneCode.pop_back();}

    string usercode = CtopOneCode;
    //cout << "<>>Token [Start]<<>" << usercode << "\n\n";
    string codeStrorage = "";
    vector<string> tokenOrder;
    vector<string> ValueOrder;
    bool Instring = false;

    for (int i = 0; i < usercode.length(); i++){
        char character = usercode[i];
        if (character == '"'){
            Instring = !Instring;
            codeStrorage += character;
            if (!Instring){
                tokenOrder.push_back("String");
                ValueOrder.push_back(codeStrorage);
                codeStrorage = "";
            }
            continue;
        }
        if (character == ' '){
            if (Instring){
                codeStrorage += character;
            } else if (!codeStrorage.empty()){
                string Ctype = tokenTot(codeStrorage);
                //cout << "Token [" << Ctype << "] <->> " << codeStrorage << "\n";
                tokenOrder.push_back(Ctype);
                ValueOrder.push_back(codeStrorage);
                codeStrorage = "";
            }
        } else if ((character == ';'|| character == '=' || character == '+' || character == '-' || character == '*' || character == '/' || character == '(' || character == ')') && !Instring){
            if (!codeStrorage.empty()){
                string Ctype = tokenTot(codeStrorage);
                //cout << "Token [" << Ctype << "] <->> " << codeStrorage << "\n";
                tokenOrder.push_back(Ctype);
                ValueOrder.push_back(codeStrorage);
                codeStrorage = "";
            }
            if (character == ';'){
                //cout << "Token [Symbol] <->> ; \n";
                tokenOrder.push_back("Symbol");
                ValueOrder.push_back(";");
            }
            else{
                //cout << "Token [Operator] <->>" << character << "\n";
                tokenOrder.push_back("Operator");
                string op(1, character);
                ValueOrder.push_back(op);
            }
        } else {
            codeStrorage += character;
        }    
    } 
    if (!codeStrorage.empty()){
        string Ctype = tokenTot(codeStrorage);
        //cout << "Token [" << Ctype << "] <->> " << codeStrorage << "\n";
        tokenOrder.push_back(Ctype);
        ValueOrder.push_back(codeStrorage);
        codeStrorage = "";
    }
    parser(tokenOrder,ValueOrder, usercode);
    //cout << "\n<>>Token [End]<<>\n";
}

vector<string> CpipaLang::valueOrdering(string rawvalue) {
    vector<string> result;
    string emtyline = "";
    bool instring = false;

    for (int i = 0; i < rawvalue.length(); i++){
        char ppC = rawvalue[i];

        if (ppC == '"'){
            instring = !instring;
            emtyline += ppC;
            if (!instring){
                result.push_back(emtyline);
                emtyline = "";
            }
            continue;
        }
        if (instring){emtyline += ppC; continue;}
        if (ppC == ' ' || ppC == ';' || ppC == '=' || ppC == '(' || ppC == ')' || ppC == '{' || ppC == '}'){
            if (!emtyline.empty()){result.push_back(emtyline); emtyline = "";}
            if (ppC != ' '){string scSymbol(1, ppC); result.push_back(scSymbol);}    
        } else {emtyline += ppC;}  
    }
    if (!emtyline.empty()) result.push_back(emtyline);
    return result;
}

vector<string> CpipaLang::createTokenList(vector<string> valueList){
    vector<string> tokenList;
    for (string lineCode : valueList){
        string tokenType = tokenTot(lineCode);
        tokenList.push_back(tokenType);
    } 
    return tokenList;
}

void CpipaLang::TerminalCode() {
    while (true){
        string inputCode;
        cout << "PPterminal<>> ";
        getline(cin, inputCode);

        if (inputCode.empty()) continue;

        if (inputCode == "ppscript-run") {
            if (ppscript.empty()){cout << "Token [Error][[edit history not found]]\n"; continue;}
            cout << "Token [Running][[Run your script]]\n";
            isWriting = false;

            for (int i = 0; i < ppscript.size(); i++){
                string linecode = ppscript[i];

                vector<string> values = valueOrdering(linecode);
                vector<string> tokens = createTokenList(values);

                if (!tokens.empty() && !values.empty())parser(tokens, values, linecode);
            }
            cout << "Token [Running][[Finished]]\n";
            ppscript.clear();
            continue;
        }

        if (inputCode == "ppscript-edit"){isWriting = true; continue;}
        if (inputCode == "-@~commandTerminal_line=>(Pipalangterminalcommandeditor.terminallinecode <--->> ~~terminalline.exitterminaleditor(true: exit terminal editor)->executecommand(true)<<->@terminallinecodecommand.quiteditor;);PipeLangLauncher.command@McodeTerminalcommand:exitOrQuiteditor(terminal.stop(run.editor))-" || inputCode == "ppscript-exit"){
            cout << "Token [Command][[Exit PipaLang terminal]]\n";
            break;
        }
        if (inputCode == "ppscript-clear"){ppscript.clear(); continue;}
        if (inputCode == "Mcode <->> version-" || inputCode == "ppscript-version"){cout << "Token [Command][[Version]] " << version <<"\n"; continue;} 

        if (!isWriting){ ppscript.clear();}
        if (isWriting){ppscript.push_back(inputCode);}
        else{
            vector<string> values = valueOrdering(inputCode);
            vector<string> tokens = createTokenList(values);
            parser(tokens, values, inputCode);
        }
        //cout << "\n"; 
    }
}

class PipaCompiler {
    public:
        void PipaToExe(string PipaName, string PipaExename){
            ifstream Pipafile(PipaName);
            if (!Pipafile.is_open()){
                cout << "Compiler [Error][[No file to run]] " << PipaName <<"\n";
                return;
            }
            ofstream filecpp("PipaLang.cpp");

            filecpp << "#include <iostream>\n";
            filecpp << "#include <string>\n";
            filecpp << "#include <cstdlib>\n"; 
            filecpp << "#include <ctime>\n";   
            filecpp << "#include <chrono>\n";  
            filecpp << "#include <vector>\n";  
            filecpp << "using namespace std;\n\n";
            
            filecpp << "class PipaBase {\n";
            filecpp << "private:\n";
            filecpp << "    chrono::steady_clock::time_point start_time;\n";
            filecpp << "public:\n";
            filecpp << "    PipaBase() { srand(time(0)); }\n"; 
            filecpp << "    void scr(string text) { cout << text; }\n";
            filecpp << "    void scr(int number) { cout << number; }\n";
            filecpp << "    void scr_nextl(string text) { cout << text << endl; }\n";
            filecpp << "    void scr_nextl(int number) { cout << number << endl; }\n";

            filecpp << "    void in(int &var) { cin >> var; }\n";
            filecpp << "    void in(string &var) { getline(cin, var); }\n";
            filecpp << "    string type_content(string var) {\n";
            filecpp << "        if (var.empty()) return \"String\";\n";
            filecpp << "        bool dot = false;\n";
            filecpp << "        bool allNumber = true;\n";
            filecpp << "        for (char c : var) {\n";
            filecpp << "            if (c == '.') {\n";
            filecpp << "                if (dot) { allNumber = false; break; }\n";
            filecpp << "                dot = true;\n";
            filecpp << "            } else if (!isdigit(c)) {\n";
            filecpp << "                allNumber = false;\n";
            filecpp << "                break;\n";
            filecpp << "            }\n";
            filecpp << "        }\n";
            filecpp << "        if (allNumber) {\n";
            filecpp << "            if (dot) {\n";
            filecpp << "                size_t dotpos = var.find('.');\n";
            filecpp << "                size_t setdecimal = var.length() - dotpos - 1;\n";
            filecpp << "                if(setdecimal > 7){\n";
            filecpp << "                    return \"double\";\n";
            filecpp << "                }\n";
            filecpp << "                return \"float\";\n";
            filecpp << "            }\n";
            filecpp << "            return \"int\";\n";
            filecpp << "        }\n";
            filecpp << "        return \"str\";\n";
            filecpp << "    }\n";
            filecpp << "    string type(string var) { return \"str\"; }";
            filecpp << "    string type(int var)    { return \"int\"; }";
            filecpp << "    string type(double var) { return \"double\"; }";
            filecpp << "    string type(float var)  { return \"float\"; }";

            filecpp << "    string replace(string text, string findtext, string changetext){\n";
            filecpp << "        size_t pos = text.find(findtext);\n";
            filecpp << "        while(pos != string::npos){\n";
            filecpp << "            text.replace(pos, findtext.length(), changetext);\n";
            filecpp << "            pos = text.find(findtext, pos + changetext.length());\n";
            filecpp << "        }\n";
            filecpp << "        return text;\n";
            filecpp << "    }\n";

            filecpp << "    bool have(string text, string findText){\n";
            filecpp << "        return text.find(findText) != string::npos; \n";
            filecpp << "    }\n";

            filecpp << "    void measure_start() { start_time = chrono::steady_clock::now(); }\n";
            filecpp << "    void measure_end() {\n";
            filecpp << "        auto end_time = chrono::steady_clock::now();\n";
            filecpp << "        auto diff = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();\n";
            filecpp << "        cout << \"\\n[run time: \" << diff << \" ms]\\n\";\n";
            filecpp << "    }\n";

            filecpp << "    int random(int min, int max) { return min + rand() % (max - min + 1); }\n";
            filecpp << "};\n";
            filecpp << "PipaBase value;\n";

            filecpp << "int to_int(std::string var) {\n";
            filecpp << "    if (value.type(var) == \"double\" || value.type(var) == \"float\") {\n";
            filecpp << "        return (int)std::stod(var);\n";
            filecpp << "    }\n";
            filecpp << "    return std::stoi(var);\n";
            filecpp << "}\n\n";

            filecpp << "float to_float(std::string var) {\n";
            filecpp << "    if (value.type(var) == \"double\") {\n";
            filecpp << "        size_t dotpos = var.find('.');\n";
            filecpp << "        if (var.length() - dotpos - 1 > 7) {\n";
            filecpp << "            var = var.substr(0, dotpos + 8);\n"; 
            filecpp << "        }\n";
            filecpp << "    }\n";
            filecpp << "    return std::stof(var);\n";
            filecpp << "}\n\n";

            filecpp << "#define str string\n\n";
            filecpp << "#define elif else if\n";
            filecpp << "#define array_str vector<string>\n";
            filecpp << "#define array_int vector<int>\n";
            filecpp << "#define push_item push_back\n";
            filecpp << "#define leng() length()\n";

            string linecode;
            while (getline(Pipafile, linecode)){
                if ((linecode.find("int ") != string::npos || 
                     linecode.find("void ") != string::npos || 
                     linecode.find("str ") != string::npos || 
                     linecode.find("dbl ") != string::npos) 
                    && linecode.find("(") != string::npos) {
                    
                    if (linecode.find("main(") == string::npos && 
                        linecode.find("for") == string::npos && 
                        linecode.find("if") == string::npos && 
                        linecode.find("while") == string::npos) {

                        size_t posEqual = linecode.find("=");
                        size_t posBracket = linecode.find("(");

                        if (posEqual == string::npos || posBracket < posEqual) {
                            if (linecode.find("flow ") == string::npos){
                                cout << "Compiler [Error][[no flow statement at your function]]\n";
                                cout << "-> Terjadi pada baris: " << linecode << "\n";
                                Pipafile.close();
                                filecpp.close();
                                system("del PipaLang.cpp");
                                return;
                            }
                        }
                    }
                }

                size_t pos = linecode.find("flow ");
                if (pos != string::npos){
                    linecode.replace(pos, 5, ""); 
                }
                filecpp << linecode << "\n";
            }

            Pipafile.close();
            filecpp.close();

            cout << "Compiler [Running][[Compiling]]\n";
            string CompileCommand = "g++ PipaLang.cpp -o" + PipaExename;
            int status = system(CompileCommand.c_str());

            system("del PipaLang.cpp");

            if (status == 0){
                cout << "Compiler [Finish][[successfully create " << PipaExename << ".exe ]]\n";
            } else {cout << "Compiler [Fail][[Can't run your project please check your code]]";}
        }
}; 
