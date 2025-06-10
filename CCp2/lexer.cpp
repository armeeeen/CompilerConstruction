//22L-6981 BSCS 6-F  Armeen Fatima
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <set>
#include <cctype>
#include <vector>
#include <stdexcept>
#include "Parser.h"


using namespace std;
const int BUFFER_SIZE = 25;  // set to 25 


void writeToBuf(char* buffer, ifstream& inputFile, bool& SFlag)
{
    char ch;
    int i = 0;
    while ((i < BUFFER_SIZE - 1) && inputFile.get(ch))
    {
        //cout<< ch<<" ";
        if (ch == ' ')
        {
            if (!SFlag)
            {
                buffer[i] = ch;
                i++;
                SFlag = 1;
            }
        }
        else {
            buffer[i] = ch;
            i++;
            SFlag = 0;
        }
    }
    buffer[i] = '\0';
}

char getNextCh(char* buf1, char* buf2, int& fp, ifstream& inputFile, int& BID, char* arr, int& count, bool& SFlag)  //count initially set to zero
{
    if (count != 0)
    {
        count--;
        return arr[count];

    }
    char* currBuff;
    if (BID == 0)
    {
        currBuff = buf1;
    }
    else
    {
        currBuff = buf2;
    }

    // buff Switch case
    if (fp >= BUFFER_SIZE - 1)
    {
        if (BID == 0)
        {
            BID = 1;
            currBuff = buf2;
        }
        else
        {
            BID = 0;
            currBuff = buf1;
        }

        //fill new buf
        writeToBuf(currBuff, inputFile, SFlag);
        fp = 0;
    }

    if (inputFile.eof() && fp >= (BUFFER_SIZE - 1))
    {
        return '\0';  // End of file
    }
    //cout << "BID;" << BID << endl;
    //cout << "Buff1:" << buf1 << endl;
    //cout << "Buff2:" << buf2 << endl;
    //cout << "fp-to be inc:" << fp << endl;
    return currBuff[fp++];
}
class hashFunc
{
public:
    template <class c1, class c2>
    std::size_t operator()(const std::pair<c1, c2>& p) const
    {
        auto A = std::hash<c1>{}(p.first);
        auto B = std::hash<c2>{}(p.second);
        return A ^ (B << 1);
    }
};
class BaseTable
{
protected:
    set<char> letter{ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
    set<char> letter_;
    set<char> digit{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    set<char> sigma;
    set<char> O1, O2, O3, O4, O5, O6, O7, O8;

public:
    BaseTable()
    {
        letter_ = letter;
        letter_.insert('_');

        sigma.insert(letter_.begin(), letter_.end());
        sigma.insert(digit.begin(), digit.end());

        //set<string> operators = { "<", ">", "<>", ":=", "==", "+", "-", "++", "+=", "<=", ">=", "%", "||", "&&", "!=", "*", "/", "“", "”", "<<", ">>" };
        set<char> operators = { '<','>',':','=','+','-','%','|','&','!','*','"','/','.' };

        for (char c : operators)
        {
            sigma.insert(c);
        }

        string punctuation = "[{()}]::,";  //milo
        for (char c : punctuation)
        {
            sigma.insert(c);
        }

        sigma.insert(' ');
        sigma.insert('\t');
        sigma.insert('\n');
        sigma.insert('\r');
        sigma.insert('\0');

        O1 = sigma;
        for (char c : digit) O1.erase(c);
        O1.erase('.');
        O1.erase('E');
        O1.erase('e');

        O2 = sigma;
        for (char c : digit) O2.erase(c);
        O2.erase('E');
        O2.erase('e');

        O3 = sigma;
        for (char c : digit) O3.erase(c);

        O4 = sigma;
        O4.erase('=');
        O4.erase('>');
        O4.erase('<');

        O5 = sigma;
        O5.erase('=');
        O5.erase('>');

        O6 = sigma;
        O6.erase('+');
        O6.erase('=');
        O6.erase('.');
        for (char c : digit) O6.erase(c);

        O7 = sigma;
        O7.erase('=');
        O7.erase('>');
        for (char c : digit) O7.erase(c);
        O7.erase('.');

        O8 = sigma;
        for (char c : letter_) O8.erase(c);
        for (char c : digit) O8.erase(c);
    }

    set<char>& getLetterSet() { return letter; }
    set<char>& getDigitSet() { return digit; }
    set<char>& getSigmaSet() { return sigma; }
    set<char>& getO1Set() { return O1; }
    set<char>& getO2Set() { return O2; }
    set<char>& getO3Set() { return O3; }
    set<char>& getO4Set() { return O4; }
    set<char>& getO5Set() { return O5; }
    set<char>& getO6Set() { return O6; }
    set<char>& getO7Set() { return O7; }
    set<char>& getO8Set() { return O8; }
};

class TransitionTable : public BaseTable
{
    std::unordered_map<std::pair<string, char>, string, hashFunc > table;
public:
    void addTransition(string state, char input, string nextState)
    {
        table[{state, input}] = nextState;
    }

    void addTransitionForSet(string state, set<char>& inputSet, string nextState)
    {
        for (char c : inputSet)
        {
            addTransition(state, c, nextState);
        }
    }

    string getNextState(string state, char input)
    {
        if (table.find({ state, input }) != table.end())
        {
            return table[{state, input}];
        }
        return "Invalid";
    }

    void printTable()
    {
        for (auto& entry : table)
        {
            cout << "(" << entry.first.first << ", " << entry.first.second << ") -> " << entry.second << endl;
        }
    }
};

class advTable : public BaseTable
{
    std::unordered_map<std::pair<string, char>, int, hashFunc > table;
public:
    void addTransition(string state, char input, int val)
    {
        table[{state, input}] = val;
    }

    void addTransitionForSet(string state, set<char>& inputSet, int val)
    {
        for (char c : inputSet)
        {
            addTransition(state, c, val);
        }
    }

    int getAdvStatus(string state, char input)
    {
        if (table.find({ state, input }) != table.end())
        {
            return table[{state, input}];
        }
        return -1;
    }

    void printTable()
    {
        for (auto& entry : table)
        {
            cout << "(" << entry.first.first << ", " << entry.first.second << ") -> " << entry.second << endl;
        }
    }
};

class FinalStatus
{
    std::unordered_map<string, int> table;
public:
    void addTransition(string state, int status) //status - 0 for not Final, 1 for Final
    {
        table[state] = status;
    }

    int getFinalStatus(string state)
    {
        if (table.find(state) != table.end())
        {
            return table[state];
        }
        return -1;  //Return -1 if state doesn't exist
    }

    void printTable()
    {
        for (auto& entry : table)
        {
            cout << entry.first << "-> " << entry.second << endl;
        }
    }
};
class resWord
{
    std::unordered_map<string, int> table;
public:
    void addTransition(string kw, int pos)
    {
        table[kw] = pos;
    }

    int getPos(string kw)
    {
        if (table.find(kw) != table.end())
        {
            return table[kw];
        }
        return -1;  //Return -1 if keyWord doesn't Exist in table
    }

    void printTable()
    {
        for (auto& entry : table)
        {
            cout << entry.first << "-> " << entry.second << endl;
        }
    }
};

string getLexemeType(const string& finalState)
{
    if (finalState == "F1")
    {
        return "Number";
    }
    else if (finalState == "F2")
    {
        return "Punctuation";
    }
    else if (finalState == "F3" || finalState == "F4")
    {
        return "Operator";
    }
    else if (finalState == "F5")
    {
        return "Identifier";
    }
    else if (finalState == "F6")
    {
        return "Keyword";
    }
    else
    {
        return "Not FOUND";
    }
}

int main()
{
    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error opening input file!" << endl;
        return 1;
    }
    ofstream S_File("symbolTable.txt", ios::app);
    if (!S_File.is_open())
    {
        cerr << "Error opening symbol file!" << endl;
        return 1;
    }
    ofstream T_File("Tokens.txt", ios::app);
    if (!T_File.is_open())
    {
        cerr << "Error opening tokens file!" << endl;
        return 1;
    }
    ofstream E_File("Errors.txt", ios::app);
    if (!E_File.is_open())
    {
        cerr << "Error opening errors file!" << endl;
        return 1;
    }
    ofstream L_File("literalTable.txt", ios::app);
    if (!L_File.is_open())
    {
        cerr << "Error opening LIteral file!" << endl;
        return 1;
    }
    std::unordered_map<string, int>  st;
    std::unordered_map<string, int>  lt;
    int idCount = 1;
    bool LFlag = 0;      //Literal flag
    int LCount = 1;
    bool CFlag1 = 0;      //comment flag for single-line comment
    bool CFlag2 = 0;      //comment flag for multi-line comment

    // Two buffers, each of size 25
    char buf1[BUFFER_SIZE];
    char buf2[BUFFER_SIZE];
    char arr[BUFFER_SIZE];               //Change to 25
    int count = 0;

    int lb = 0;   //lexeme begin ptr
    int fp = 0;  // Forward ptr
    int BID = 0;  // 0 for buf1, 1 for buf2
    bool SFlag = 0;  //Space Flag

    writeToBuf(buf1, inputFile, SFlag);

    //KeyWords table
    resWord rw;
    rw.addTransition("asm", 0);
    rw.addTransition("Wagarna", 1);
    rw.addTransition("new", 2);
    rw.addTransition("this", 3);
    rw.addTransition("auto", 4);
    rw.addTransition("enum", 5);
    rw.addTransition("operator", 6);
    rw.addTransition("throw", 7);
    rw.addTransition("Mantiqi", 8);
    rw.addTransition("explicit", 9);
    rw.addTransition("private", 10);
    rw.addTransition("True", 11);
    rw.addTransition("break", 12);
    rw.addTransition("export", 13);
    rw.addTransition("protected", 14);
    rw.addTransition("try", 15);
    rw.addTransition("case", 16);
    rw.addTransition("extern", 17);
    rw.addTransition("public", 18);
    rw.addTransition("typedef", 19);
    rw.addTransition("catch", 20);
    rw.addTransition("False", 21);
    rw.addTransition("register", 22);
    rw.addTransition("typeid", 23);
    rw.addTransition("Harf", 24);
    rw.addTransition("Ashriya", 25);
    rw.addTransition("typename", 26);
    rw.addTransition("Adadi", 27);
    rw.addTransition("class", 28);
    rw.addTransition("for", 29);
    rw.addTransition("Wapas", 30);
    rw.addTransition("union", 31);
    rw.addTransition("const", 32);
    rw.addTransition("dost", 33);
    rw.addTransition("short", 34);
    rw.addTransition("unsigned", 35);
    rw.addTransition("goto", 36);
    rw.addTransition("signed", 37);
    rw.addTransition("using", 38);
    rw.addTransition("continue", 39);
    rw.addTransition("Agar", 40);
    rw.addTransition("sizeof", 41);
    rw.addTransition("virtual", 42);
    rw.addTransition("default", 43);
    rw.addTransition("inline", 44);
    rw.addTransition("static", 45);
    rw.addTransition("Khali", 46);
    rw.addTransition("delete", 47);
    rw.addTransition("volatile", 48);
    rw.addTransition("do", 49);
    rw.addTransition("long", 50);
    rw.addTransition("struct", 51);
    rw.addTransition("double", 52);
    rw.addTransition("mutable", 53);
    rw.addTransition("switch", 54);
    rw.addTransition("while", 55);
    rw.addTransition("namespace", 56);
    rw.addTransition("template", 57);
    rw.addTransition("Marqazi", 58);
    rw.addTransition("Matn", 59);
    rw.addTransition("input->", 60);
    rw.addTransition("output<-", 61);



    //tables mapping
    FinalStatus ft;
    ft.addTransition("S", 0);
    ft.addTransition("A", 0);
    ft.addTransition("B", 0);
    ft.addTransition("C", 0);
    ft.addTransition("D", 0);
    ft.addTransition("E", 0);
    ft.addTransition("G", 0);
    ft.addTransition("H", 0);
    ft.addTransition("I", 0);
    ft.addTransition("J", 0);
    ft.addTransition("L", 0);
    ft.addTransition("M", 0);
    ft.addTransition("N", 0);
    ft.addTransition("P", 0);
    ft.addTransition("X", 0);
    ft.addTransition("Z", 0);
    ft.addTransition("V", 0);
    ft.addTransition("F1", 1);
    ft.addTransition("F2", 1);
    ft.addTransition("F3", 1);
    ft.addTransition("F4", 1);
    ft.addTransition("F5", 1);
    ft.addTransition("F6", 1);
    /////////////////////////////////////////////////////////////////////////////////
    TransitionTable tt;
    //Identifier
    tt.addTransitionForSet("S", tt.getLetterSet(), "P");
    tt.addTransition("S", '_', "Z");
    tt.addTransitionForSet("P", tt.getDigitSet(), "P");
    tt.addTransitionForSet("P", tt.getLetterSet(), "P");
    tt.addTransition("P", '_', "Z");
    tt.addTransitionForSet("Z", tt.getLetterSet(), "Z");
    tt.addTransitionForSet("Z", tt.getDigitSet(), "Z");
    tt.addTransition("Z", '_', "Z");
    tt.addTransitionForSet("Z", tt.getO8Set(), "F5");

    //Punctuation
    tt.addTransition("S", ':', "M");
    tt.addTransition("S", '}', "F2");
    tt.addTransition("S", ')', "F2");
    tt.addTransition("S", ']', "F2");
    tt.addTransition("S", '(', "F2");
    tt.addTransition("S", '{', "F2");
    tt.addTransition("S", '[', "F2");
    tt.addTransition("M", ':', "F2");
    tt.addTransition("S", ',', "F2");  //milo

    //Operators
    tt.addTransition("M", '=', "F3");
    tt.addTransition("S", '>', "I");
    tt.addTransition("I", '=', "F3");
    tt.addTransition("I", '>', "F3");
    tt.addTransitionForSet("I", tt.getO5Set(), "F4");

    tt.addTransition("S", '<', "H");
    tt.addTransition("H", '=', "F3");
    tt.addTransition("H", '>', "F3");
    tt.addTransition("H", '<', "F3");
    tt.addTransitionForSet("H", tt.getO4Set(), "F4");

    tt.addTransition("S", '!', "N");
    tt.addTransition("N", '=', "F3");

    tt.addTransition("S", '&', "T");
    tt.addTransition("T", '&', "F3");

    tt.addTransition("S", '|', "L");
    tt.addTransition("L", '|', "F3");

    tt.addTransition("S", '=', "J");
    tt.addTransition("J", '=', "F3");

    tt.addTransition("S", '%', "F3");
    tt.addTransition("S", '*', "F3");
    tt.addTransition("S", '/', "F3");
    //tt.addTransition("S", '"', "F3");  //Close quotation Mark??

    tt.addTransition("S", '-', "X");
    tt.addTransitionForSet("X", tt.getO7Set(), "F4");

    tt.addTransition("S", '+', "B");
    tt.addTransition("B", '+', "F3");
    tt.addTransition("B", '=', "F3");
    tt.addTransitionForSet("B", tt.getO6Set(), "F4");  //++


    //Numbers
    tt.addTransitionForSet("S", tt.getDigitSet(), "A");
    tt.addTransitionForSet("B", tt.getDigitSet(), "A");
    tt.addTransitionForSet("X", tt.getDigitSet(), "A");
    tt.addTransition("S", '.', "V");
    tt.addTransition("B", '.', "V");
    tt.addTransition("X", '.', "V");
    tt.addTransitionForSet("A", tt.getDigitSet(), "A");
    tt.addTransitionForSet("V", tt.getDigitSet(), "C");
    tt.addTransitionForSet("A", tt.getO1Set(), "F1");
    tt.addTransition("A", '.', "C");
    tt.addTransition("A", 'E', "D");
    tt.addTransition("A", 'e', "D");
    tt.addTransitionForSet("C", tt.getDigitSet(), "C");
    tt.addTransitionForSet("C", tt.getO2Set(), "F1");
    tt.addTransition("C", 'E', "D");
    tt.addTransition("C", 'e', "D");

    tt.addTransition("D", '+', "G");
    tt.addTransition("D", '-', "G");
    tt.addTransitionForSet("D", tt.getDigitSet(), "E");
    tt.addTransitionForSet("E", tt.getDigitSet(), "E");
    tt.addTransitionForSet("E", tt.getO3Set(), "F1");
    tt.addTransitionForSet("G", tt.getDigitSet(), "E");
    //May be keyword                //++
    tt.addTransitionForSet("P", tt.getO8Set(), "F6");
    //////////////////////////////////////////////////////////////////////////
    advTable at;

    //Identifier
    at.addTransitionForSet("S", at.getLetterSet(), 1);
    at.addTransitionForSet("P", at.getDigitSet(), 1);
    at.addTransitionForSet("P", at.getLetterSet(), 1);
    at.addTransition("P", '_', 1);
    at.addTransitionForSet("Z", at.getLetterSet(), 1);
    at.addTransitionForSet("Z", at.getDigitSet(), 1);
    at.addTransition("Z", '_', 1);
    at.addTransitionForSet("Z", at.getO8Set(), 0);

    //Punctuation
    at.addTransition("S", ':', 1);
    at.addTransition("S", '}', 1);
    at.addTransition("S", ')', 1);
    at.addTransition("S", ']', 1);
    at.addTransition("S", '(', 1);
    at.addTransition("S", '{', 1);
    at.addTransition("S", '[', 1);
    at.addTransition("M", ':', 1);
    at.addTransition("S", ',', 1);

    //Operators
    at.addTransition("M", '=', 1);
    at.addTransition("S", '>', 1);
    at.addTransition("I", '=', 1);
    at.addTransition("I", '>', 1);
    at.addTransitionForSet("I", at.getO5Set(), 0);

    at.addTransition("S", '<', 1);
    at.addTransition("H", '=', 1);
    at.addTransition("H", '>', 1);
    at.addTransition("H", '<', 1);
    at.addTransitionForSet("H", at.getO4Set(), 0);

    at.addTransition("S", '!', 1);
    at.addTransition("N", '=', 1);

    at.addTransition("S", '&', 1);
    at.addTransition("T", '&', 1);

    at.addTransition("S", '|', 1);
    at.addTransition("L", '|', 1);

    at.addTransition("S", '=', 1);
    at.addTransition("J", '=', 1);

    at.addTransition("S", '%', 1);
    at.addTransition("S", '*', 1);
    at.addTransition("S", '/', 1);
    //at.addTransition("S", '"', 1);  //Close quotation Mark??

    at.addTransition("S", '-', 1);
    at.addTransitionForSet("X", at.getO7Set(), 0);

    at.addTransition("S", '+', 1);
    at.addTransition("B", '+', 1);
    at.addTransition("B", '=', 1);
    at.addTransitionForSet("B", at.getO6Set(), 0);  //++

    //Numbers
    at.addTransitionForSet("S", at.getDigitSet(), 1);
    at.addTransitionForSet("B", at.getDigitSet(), 1);
    at.addTransitionForSet("X", at.getDigitSet(), 1);
    at.addTransition("S", '.', 1);
    at.addTransition("B", '.', 1);
    at.addTransition("X", '.', 1);
    at.addTransitionForSet("A", at.getDigitSet(), 1);
    at.addTransitionForSet("V", at.getDigitSet(), 1);
    at.addTransitionForSet("A", at.getO1Set(), 0);
    at.addTransition("A", '.', 1);
    at.addTransition("A", 'E', 1);
    at.addTransition("A", 'e', 1);
    at.addTransitionForSet("C", at.getDigitSet(), 1);
    at.addTransitionForSet("C", at.getO2Set(), 0);
    at.addTransition("C", 'E', 1);
    at.addTransition("C", 'e', 1);

    at.addTransition("D", '+', 1);
    at.addTransition("D", '-', 1);
    at.addTransitionForSet("D", at.getDigitSet(), 1);
    at.addTransitionForSet("E", at.getDigitSet(), 1);
    at.addTransitionForSet("E", at.getO3Set(), 0);
    at.addTransitionForSet("G", at.getDigitSet(), 1);

    /////////         ++
    at.addTransitionForSet("F1", at.getSigmaSet(), 0);
    at.addTransitionForSet("F2", at.getSigmaSet(), 0);
    at.addTransitionForSet("F3", at.getSigmaSet(), 0);
    at.addTransitionForSet("F4", at.getSigmaSet(), 0);
    at.addTransitionForSet("F5", at.getSigmaSet(), 0);
    //May be keyword  //++
    at.addTransitionForSet("F6", at.getSigmaSet(), 0);

    //May be keyword                       //++
    at.addTransitionForSet("P", at.getO8Set(), 0);
    //////////
    // Lexical analysis
    string state = "S";
    string lexeme = "";  // To store current lex
    char c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
    while (c != '\0')
    {
        //Handling comments

        // 1-Handling single-Line comments
        if (CFlag1 && c == '\n')
        {

            CFlag1 = 0;
            state = "S";
            lexeme = "";
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }
        else if (CFlag1)
        {
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }
        // 2-Handling Multi-Line comments
        /////////////////////
        //sp
        if (CFlag2 && c == '*')
        {
            char c1 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            char c2 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            //check
            cout << "c=" << c << endl;
            cout << "c1=" << (int)c1 << endl;
            if (c1 == '/')
            {
                CFlag2 = 0;
                state = "S";
                lexeme = "";
                c = c2;
                continue;
            }
            else if (c1 != '\0' && c1 != 0)
            {
                arr[count] = c1;
                count++;
                arr[count] = c2;
                count++;
                continue;

            }
            else
            {
                arr[count] = c1;
                count++;
                continue;
            }

        }
        ///////////ep
        ////////////
        else if (CFlag2)
        {
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }

        //Handling Literals
        //sp
        if (c == '"' && !LFlag)
        {
            LFlag = 1;
            lexeme = ""; //lex for Literal
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }
        else if (c == '"' && LFlag)
        {
            LFlag = 0;
            // Check if the literal is already in the literal table
            if (lt.find(lexeme) == lt.end())
            {
                lt[lexeme] = LCount;
                L_File << LCount << " " << lexeme << endl; // Write to Litral file
                LCount++;
            }
            T_File << "<literal," << lt[lexeme] << ">" << endl; // Write token to Token File
            TV.push_back(token("literal"));      //%%%
            cout << "Lexeme: " << lexeme << "\nType = Literal, Token = <literal," << lt[lexeme] << ">" << endl;
            lexeme = "";
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }
        else if (LFlag && c == '\n') // Handle end of line inside a literal
        {
            cout << "Error: Unterminated Literal at end of line!" << endl;
            E_File << "Unterminated Literal at end of line: " << lexeme << endl;
            LFlag = 0;
            lexeme = "";
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }
        else if (LFlag)
        {
            lexeme += c;
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }
        //ep
        //cout << "Next Char:" << c << endl;
        if (!(tt.getSigmaSet().find(c) != tt.getSigmaSet().end()))
        {
            //check
            cout << "Errenous Character:" << c << endl;

            E_File << c << endl;

            c = ' ';
        }
        string newState = tt.getNextState(state, c);
        if (newState == "Invalid")
        {
            //check
            if (lexeme != " " && lexeme != "\t" && lexeme != "\n" && lexeme != "\r" && lexeme != "\0")
            {
                cout << "Invalid input at character: " << c << endl;
                cout << "Resultant invalid lexeme= " << lexeme << endl;  //Lexical Error
                E_File << lexeme << endl;
            }

            lexeme = "";                 //MM
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\0')
            {
                lexeme += c;
            }
            state = "S";
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
            continue;
        }

        if (at.getAdvStatus(state, c) == 1)
        {
            lexeme += c;
            c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
        }

        state = newState;

        if (ft.getFinalStatus(state) == 1)
        {
            if (!(tt.getSigmaSet().find(c) != tt.getSigmaSet().end()))
            {
                //check
                cout << "Errenous Character:" << c << endl;

                E_File << c << endl;

                c = ' ';
            }
            //lexeme += c;  //--s
            string lexemeType = getLexemeType(state);
            if (lexemeType == "Keyword")
            {
                //check
                cout << "Test Lex for keyWord=" << lexeme << endl;

                if (rw.getPos(lexeme) != -1)
                {
                    //check
                    cout << "Type= Keyword" << endl;
                    cout << "<keyword," << rw.getPos(lexeme) << ">" << endl;  //Token to be returned

                    T_File << "<keyword," << rw.getPos(lexeme) << ">" << endl;
                    TV.push_back(token(lexeme));      //%%%
                }
                else if (lexeme == "input" || lexeme == "output")
                {
                    //handle case if input-> or output<-
                    if (lexeme == "input")       //CORRRECT CASE
                    {
                        char c1 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        char c2 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        cout << "c=" << c << endl;
                        cout << "c1=" << (int)c1 << endl;
                        if (c == '-' && c1 == '>')
                        {
                            //check
                            cout << "Type= Keyword" << endl;
                            cout << "<keyword," << rw.getPos("input->") << ">" << endl;

                            T_File << "<keyword," << rw.getPos("input->") << ">" << endl;
                            TV.push_back(token("input->"));      //%%%

                            c = c2;
                        }
                        else if (c1 != '\0' && c1 != 0)
                        {
                            //check
                            cout << "Lexical Error!--- input22" << endl;

                            E_File << lexeme << endl;


                            arr[count] = c1;
                            count++;
                            arr[count] = c2;
                            count++;
                            lexeme = "";
                            state = "S";
                        }
                        else
                        {
                            //check
                            cout << "Invalid Lexeme: " << lexeme << endl;  //Lexical Error

                            E_File << lexeme << endl;

                            arr[count] = c1;
                            count++;

                            lexeme = "";
                            state = "S";
                        }

                    }
                    else
                    {
                        char c1 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        char c2 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        cout << "c=" << c << endl;
                        cout << "c1=" << (int)c1 << endl;
                        if (c == '<' && c1 == '-')
                        {
                            //check
                            cout << "Type= Keyword" << endl;
                            cout << "<keyword," << rw.getPos("output<-") << ">" << endl;

                            T_File << "<keyword," << rw.getPos("output<-") << ">" << endl;
                            TV.push_back(token("output<-"));      //%%%

                            c = c2;
                        }
                        else if (c1 != '\0' && c1 != 0)
                        {
                            //check
                            cout << "Lexical Error!--- output22" << endl;

                            E_File << lexeme << endl;


                            arr[count] = c1;
                            count++;
                            arr[count] = c2;
                            count++;
                            lexeme = "";
                            state = "S";
                        }
                        else
                        {
                            //check
                            cout << "Invalid Lexeme: " << lexeme << endl;  //Lexical Error

                            E_File << lexeme << endl;

                            arr[count] = c1;
                            count++;

                            lexeme = "";
                            state = "S";
                        }

                    }
                }
                else
                {
                    //check;
                    cout << "Neither keyword nor identifier!" << endl;   //Another Lexical Error

                    if (lexeme != " " && lexeme != "\t" && lexeme != "\n" && lexeme != "\r" && lexeme != "\0")
                    {
                        E_File << lexeme << endl;
                    }

                    lexeme = "";
                    state = "S";
                }
            }
            else if (lexemeType == "Identifier")
            {
                if (st.find(lexeme) == st.end())
                {
                    st[lexeme] = idCount;
                    S_File << idCount << " " << lexeme << endl;  //Write to symbol Ttable.txt
                    idCount++;
                }
                T_File << "<id," << st[lexeme] << ">" << endl;
                TV.push_back(token("identifier"));      //%%%
                //check
                cout << "Lexeme: " << lexeme << "\nType = Identifier, Token = <id," << st[lexeme] << ">" << endl;
            }
            else if (lexemeType == "Punctuation" || lexemeType == "Number")
            {
                //check
                cout << "Lexeme: " << lexeme << "\nType = " << lexemeType << endl;

                T_File << "<" << lexeme << ">" << endl;
                if (lexemeType == "Punctuation")
                {
                    TV.push_back(token(lexeme));      //%%%
                }
                else
                {
                    TV.push_back(token("number"));      //%%%
                }
            }
            else if (lexemeType == "Operator")
            {
                //check
                cout << "Lexeme: " << lexeme << "\nType = " << lexemeType << endl;
                //sp
                if (lexeme == "/")   //Handling case of comments
                {
                    cout << "c=" << c << endl;
                    if (c == '/')  //case of single-line comment
                    {
                        //check
                        cout << "Type= Single Line Comment" << endl;

                        CFlag1 = 1;
                        c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        continue;
                    }
                    else if (c == '*')    //Handling case of multi-line comments
                    {
                        //check
                        cout << "Type= Multi Line Comment" << endl;

                        CFlag2 = 1;
                        c = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        continue;
                    }
                    else
                    {
                        T_File << "<" << lexeme << ">" << endl;
                        TV.push_back(token(lexeme));      //%%%
                        lexeme = "";
                        state = "S";
                    }
                }
                //ep
                else
                {
                    T_File << "<" << lexeme << ">" << endl;
                    TV.push_back(token(lexeme));    //%%%%
                }
            }
            else   //Tbr  -this case never arises
            {
                //check
                cout << "Lexeme: " << lexeme << "\nType = " << lexemeType << endl;

            }
            state = "S";
            lexeme = "";
        }
    }

    if (c == '\0')
    {
        ////////////////////////////
        //sp
        if (CFlag1 || CFlag2)
        {
            //Do nothing
            state = "S";
            lexeme = "";
        }
        if (LFlag)
        {
            cout << "Error: Unterminated Literal at eof!" << endl;
            E_File << "Unterminated Literal at eof: " << lexeme << endl;
        }

        //ep
        //////////////////////////////
        string newState = tt.getNextState(state, c);
        if (newState == "Invalid")
        {

            if (lexeme != " " && lexeme != "\t" && lexeme != "\n" && lexeme != "\r" && lexeme != "\0" && !LFlag)
            {
                //check
                cout << "Invalid input at character: " << c << endl;
                cout << "Resultant invalid lexeme= " << lexeme << endl;  //Lexical Error
                E_File << lexeme << endl;
            }

            lexeme = "";                 //MM
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\0')
            {
                lexeme += c;
            }
            state = "S";
        }
        state = newState;

        if (ft.getFinalStatus(state) == 1)
        {
            string lexemeType = getLexemeType(state);
            if (lexemeType == "Keyword")
            {
                //check
                cout << "Test Lex for keyWord=" << lexeme << endl;

                if (rw.getPos(lexeme) != -1)
                {
                    //check
                    cout << "Type= Keyword" << endl;
                    cout << "<keyword," << rw.getPos(lexeme) << ">" << endl;  //Token to be returned

                    T_File << "<keyword," << rw.getPos(lexeme) << ">" << endl;
                    TV.push_back(token(lexeme));     //%%%%
                }
                else if (lexeme == "input" || lexeme == "output")
                {
                    //handle case if input-> or output<-
                    if (lexeme == "input")       //CORRRECT CASE
                    {
                        char c1 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        char c2 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        cout << "c=" << c << endl;
                        cout << "c1=" << (int)c1 << endl;
                        if (c == '-' && c1 == '>')
                        {
                            //check
                            cout << "Type= Keyword" << endl;
                            cout << "<keyword," << rw.getPos("input->") << ">" << endl;

                            T_File << "<keyword," << rw.getPos("input->") << ">" << endl;
                            TV.push_back(token("input->"));     //%%%%

                            c = c2;
                        }
                        else if (c1 != '\0' && c1 != 0)
                        {
                            //check
                            cout << "Lexical Error!--- input22" << endl;

                            E_File << lexeme << endl;


                            arr[count] = c1;
                            count++;
                            arr[count] = c2;
                            count++;
                            lexeme = "";
                            state = "S";
                        }
                        else
                        {
                            //check
                            cout << "Invalid Lexeme: " << lexeme << endl;  //Lexical Error

                            E_File << lexeme << endl;


                            lexeme = "";
                            state = "S";
                        }

                    }
                    else
                    {
                        char c1 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        char c2 = getNextCh(buf1, buf2, fp, inputFile, BID, arr, count, SFlag);
                        cout << "c=" << c << endl;
                        cout << "c1=" << (int)c1 << endl;
                        if (c == '<' && c1 == '-')
                        {
                            //check
                            cout << "Type= Keyword" << endl;
                            cout << "<keyword," << rw.getPos("output<-") << ">" << endl;

                            T_File << "<keyword," << rw.getPos("output<-") << ">" << endl;
                            TV.push_back(token("output<-"));     //%%%%

                            c = c2;
                        }
                        else if (c1 != '\0' && c1 != 0)
                        {
                            //check
                            cout << "Lexical Error!--- output22" << endl;

                            E_File << lexeme << endl;


                            arr[count] = c1;
                            count++;
                            arr[count] = c2;
                            count++;
                            lexeme = "";
                            state = "S";
                        }
                        else
                        {
                            //check
                            cout << "Invalid Lexeme: " << lexeme << endl;  //Lexical Error

                            E_File << lexeme << endl;


                            lexeme = "";
                            state = "S";
                        }

                    }
                }
                else
                {
                    //check;
                    cout << "Neither keyword nor identifier!" << endl;   //Another Lexical Error

                    if (lexeme != " " && lexeme != "\t" && lexeme != "\n" && lexeme != "\r" && lexeme != "\0")
                    {
                        E_File << lexeme << endl;
                    }

                    lexeme = "";
                    state = "S";
                }
            }
            else if (lexemeType == "Identifier")
            {
                if (st.find(lexeme) == st.end())
                {
                    st[lexeme] = idCount;
                    S_File << idCount << " " << lexeme << endl;  //Write to symbol Ttable.txt
                    idCount++;
                }
                T_File << "<id," << st[lexeme] << ">" << endl;
                TV.push_back(token("identifier"));     //%%%%
                //check
                cout << "Lexeme: " << lexeme << "\nType = Identifier, Token = <id," << st[lexeme] << ">" << endl;
            }
            else if (lexemeType == "Punctuation" || lexemeType == "Operator" || lexemeType == "Number")
            {
                //check
                cout << "Lexeme: " << lexeme << "\nType = " << lexemeType << endl;

                T_File << "<" << lexeme << ">" << endl;
                if (lexemeType == "Punctuation")
                {
                    TV.push_back(token(lexeme));      //%%%
                }
                else
                {
                    TV.push_back(token("number"));      //%%%
                }
            }
            else   //To be removed
            {
                //check
                cout << "Lexeme: " << lexeme << "\nType = " << lexemeType << endl;
            }
            state = "S";
            lexeme = "";
        }
    }
    ///////////////////////////////////////////////////////////////////////
    ///PARSER SECTION
    cout << "Printing Tokens Vector" << endl;
    for (int j = 0; j < TV.size(); j++)
    {
        cout << "Type: " << TV[j].type << endl;
    }
    try 
    {
        Parser();
    }
    catch (const runtime_error& e) 
    {
        cerr << "Parser Exception: " << e.what() << endl;
    }
    ////////////////////////////////////////////////////////////////////////
    inputFile.close();
    S_File.close();
    T_File.close();
    E_File.close();
    L_File.close();
    return 0;
}




