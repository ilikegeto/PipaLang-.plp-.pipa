#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <map>

using namespace std;
map<string, int> McodeMemory;

string version = "0.0.1 startup";

string tokenTot(string kata){
    if(kata.empty()) return "Empty Code";
    if (kata.length() >= 4 && kata.substr(0, 2) == "(\"" && kata.substr(kata.length() - 2) == "\")"){
        return "String";
    }
    if (kata == "int" || kata == "float" || kata == "char" || kata == "if" || kata == "return" || kata == "value.scr") {
       return "Keyword";
    } else if (isdigit(kata[0])) {
        return "Number";
    } else {
        return "Identifier";
    }
}

void evaluator(vector<string> valueList){
    string varName = valueList[1];
    if (valueList.size() < 4) return;
    int finalValue = 0;
    string firstr = valueList[3];

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

void evaluatorscr(string tokentype, string tokenval){
    if (tokentype == "Number"){
        cout << tokenval <<"\n";
    } else if (tokentype == "String"){
        if (tokenval.length() >= 4 && tokenval.substr(0, 2) == "(\"" && tokenval.substr(tokenval.length() - 2) == "\")"){
            string strscr = tokenval.substr(2, tokenval.length() - 4);
            cout << strscr << "\n";
        }
        else{
            string strscr = tokenval.substr(2, tokenval.length() - 4);
            cout << strscr << "\n";
        }
        
    } else if (tokentype == "Identifier"){
        if (McodeMemory.find(tokenval) != McodeMemory.end()){
            cout << McodeMemory[tokenval] << "\n";
        } else {
            cout << "Token [Error][[Variable Not found]]\n";
        }
    }
}

void parser(vector<string> TokenList,vector<string> valueList, string CodeTintung) {

    /*cout << "[DEBUG] Total Token: " << TokenList.size() << "\n";
    if (!TokenList.empty()) {
        cout << "[DEBUG] Token Pertama: " << TokenList.front() << " (" << valueList.front() << ")\n";
        cout << "[DEBUG] Token Terakhir: " << TokenList.back() << " (" << valueList.back() << ")\n";
    }*/

    if (TokenList.empty() || valueList.empty()) return;
    //cout << "Parser [Start] <<> \n";
    bool yesYes = true;

    if (valueList[0] == "value.scr"){
        if (TokenList.back() != "Symbol" || valueList.back() != ";"){cout << "Token [Error][[(;)Symbol Not found]]\n"; return;}
        if (TokenList.size() < 3){cout << "Token [Error][[No argument to screen]]\n"; return;}
        if (TokenList.size() == 3){
            if (TokenList[1] != "Identifier" && TokenList[1] != "Number" && TokenList[1] != "String"){cout << "Token [Error][[Data not supported for value.scr()]]" << TokenList[1] <<"\n";}
            else {evaluatorscr(TokenList[1], valueList[1]);};  
        }
        else if (TokenList.size() > 3){
            //cout << "Token [Running][" << CodeTintung << "]\n";

            vector<string> doMathlist;

            doMathlist.push_back(valueList[0]);
            doMathlist.push_back("TUMBAL");
            doMathlist.push_back("=");
            for (int i = 1; i < valueList.size(); i++){doMathlist.push_back(valueList[i]);}
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
            else if(TokenList[TokenList.size() - 2] != "Number" && TokenList[TokenList.size() - 2] != "Identifier") yesYes = false;
            else if (TokenList.back() != "Symbol" || valueList.back() != ";"){
                cout << "Token [Error][[(;)Symbol Not found]]\n";
                yesYes = false;
            }
            if (yesYes){/*cout << "Token [Running][" << CodeTintung << "]\n";*/ evaluator(valueList);}
            else {cout << "Token [Error][[Syntax Error or Variable Error]]\n";}
        } else {cout << "Token [Error][[Too short for declarate]]\n";}
    } else {cout << "Token [Error][[Unknown syntax]]" << valueList[0] << "\n";}
    //cout << "Parser [End] <<> \n";
}

void code(string CtopOneCode){
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
        } else if ((character == ';'|| character == '=' || character == '+' || character == '-' || character == '*' || character == '/') && !Instring){
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

void TerminalCode() {
    string inputCode;
    while (true){
        cout << "Mcode<>> ";
        getline(cin, inputCode);
        if (inputCode == "-@~commandTerminal_line=>(Mcodeterminalcommandeditor.terminallinecode <--->> ~~terminalline.exitterminaleditor(true: exit terminal editor)->executecommand(true)<<->@terminallinecodecommand.quiteditor;);McodeLauncher.command@McodeTerminalcommand:exitOrQuiteditor(terminal.stop(run.editor))-" || inputCode == "exit pls"){
            cout << "Token [Command][[Exit Mcode terminal]]\n";
            break;
        } else if (inputCode == "Mcode <->> version-"){
            cout << "Token [Command][[Version]] " << version <<"\n";
            continue;
        }
        if (inputCode.empty()) continue;
        code(inputCode);
        //cout << "\n"; 
    }
}