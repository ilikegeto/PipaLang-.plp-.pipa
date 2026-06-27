#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <map>
#include <limits>

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
