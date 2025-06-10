//22L-6981 BSCS 6-F  Armeen Fatima
#include "Parser.h"
#include <stdexcept>
#include <iostream>

using namespace std;

int idx = 0;
vector<token> TV;

//Parser Function Prototypes
void Function(string lead = "");
void ArgList(string lead);
void ArgList_(string lead);
void Arg(string lead);
void Declaration(string lead);
void Type(string lead);
void IdentList(string lead);
void IdentList_(string lead);
void Stmt(string lead);
void ForStmt(string lead);
void OptExpr(string lead);
void WhileStmt(string lead);
void IfStmt(string lead);
void ElsePart(string lead);
void CompStmt(string lead);
void StmtList(string lead);
void Expr(string lead);
void Expr_(string lead);
void Rvalue(string lead);
void Compare(string lead);
void Rvalue_(string lead);
void Mag(string lead);
void Mag__(string lead);
void Mag_(string lead);
void Term(string lead);
void Term__(string lead);
void Term_(string lead);
void Factor(string lead);

token::token(string type)
{
    this->type = type;
}

//////////////////////////////////////////////

void printTerminal(string lead, string v)
{
    cout << lead << "'" << v << "'" << endl;
}

bool check(string lead,string reqT)
{
    if (idx < TV.size() && TV[idx].type == reqT)
    {
        printTerminal(lead + "|___", reqT);
        idx++;
        return 1;
    }
    return 0;
}


/////////////////////////////////////////////////

void Function(string lead)
{
    cout << lead << "Function" << endl;
    Type(lead + "|___");
    if (check(lead,"identifier"))
    {
        if (check(lead,"("))
        {
            ArgList(lead + "|___");
            if (check(lead, ")"))
            {
                CompStmt(lead + "|___");
            }
            else throw runtime_error("Expected ')' in expression");
        }
        else throw runtime_error("Expected '(' in expression");
    }
    else throw runtime_error("Expected identifier in expression");
}

void ArgList(string lead)
{
    cout << lead << "ArgList" << endl;
    Arg(lead + "|___");
    ArgList_(lead + "|___");
}

void ArgList_(string lead)
{
    cout << lead << "ArgList_" << endl;
    if (check(lead,","))
    {
        Arg(lead + "|___");
        ArgList_(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }
}

void Arg(string lead)
{
    cout << lead << "Arg" << endl;
    Type(lead + "|___");
    if (!check(lead,"identifier")) throw runtime_error("Expected identifier in expression");
}

void Declaration(string lead)
{
    cout << lead << "Declaration" << endl;
    Type(lead + "|___");
    IdentList(lead + "|___");
    if (!check(lead, "::")) throw runtime_error("Expected '::' in expression");
}

void Type(string lead)
{
    cout << lead << "Type" << endl;
    if (!(check(lead, "Adadi") || check(lead, "Ashriya") || check(lead, "Harf") || check(lead, "Matn") || check(lead, "Mantiqi")))
        throw runtime_error("Expected 'Type' in expression");
}

void IdentList(string lead)
{
    cout << lead << "IdentList" << endl;
    if (check(lead, "identifier"))
    {
        IdentList_(lead + "|___");
    }
    else throw runtime_error("Expected identifier in expression");
}

void IdentList_(string lead)
{
    cout << lead << "IdentList_" << endl;
    if (check(lead, ","))
    {
        IdentList(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }

}

void Stmt(string lead)
{
    cout << lead << "Stmt" << endl;
    //idx < TV.size() && TV[idx].type == reqT;
    if (idx < TV.size() && TV[idx].type == "for") ForStmt(lead + "|___");
    else if (idx < TV.size() && TV[idx].type == "while") WhileStmt(lead + "|___");
    else if (idx < TV.size() && TV[idx].type == "Agar") IfStmt(lead + "|___");
    else if (idx < TV.size() && TV[idx].type == "{") {CompStmt(lead + "|___"); }
    else if (check(lead, "::")) return;
    else if ((idx < TV.size() && TV[idx].type == "identifier") || (idx < TV.size() && TV[idx].type == "(") || (idx < TV.size() && TV[idx].type == "number"))   ///wow
    {
        Expr(lead + "|___"); 
       // cout << TV[idx].type << endl;
        if (!check(lead, "::")) throw runtime_error("Expected '::' in expression :(");   //CONFIRM
    }
    else if (((idx < TV.size() && TV[idx].type == "Adadi")) || ((idx < TV.size() && TV[idx].type == "Ashriya")) ||((idx < TV.size() && TV[idx].type == "Harf")) || ((idx < TV.size() && TV[idx].type == "Matn")) || ((idx < TV.size() && TV[idx].type == "Mantiqi"))) {Declaration(lead + "|___"); }
    else throw runtime_error("Stmt Error in expression");
}

void ForStmt(string lead)
{
    cout << lead << "ForStmt" << endl;

    if (check(lead, "for"))
    {
        if (check(lead, "("))
        {
            Expr(lead + "|___");
            if (check(lead, "::"))
            {
                OptExpr(lead + "|___");
                if (check(lead, "::"))
                {
                    OptExpr(lead + "|___");
                    if (check(lead, ")"))
                    {
                        Stmt(lead + "|___");
                    }
                    else throw runtime_error("Expected ')' in expression");
                }
                else throw runtime_error("Expected '::' in expression");
            }
            else throw runtime_error("Expected '::' in expression");
        }
        else throw runtime_error("Expected '(' in expression");
    }
    else throw runtime_error("Expected 'for' in expression");
}

void OptExpr(string lead)
{
    cout << lead << "OptExpr" << endl;
    if ((idx < TV.size() && TV[idx].type == "identifier") || (idx < TV.size() && TV[idx].type == "(") || (idx < TV.size() && TV[idx].type == "number"))
    {
        Expr(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }
}

void WhileStmt(string lead)
{
    cout << lead << "WhileStmt" << endl;

    if (check(lead, "while"))
    {
        if (check(lead, "("))
        {
            Expr(lead + "|___");
            if (check(lead, ")"))
            {
                Stmt(lead + "|___");
            }
            else throw runtime_error("Expected ')' in expression");
        }
        else throw runtime_error("Expected '(' in expression");
    }
    else throw runtime_error("Expected 'while' in expression");
}
void IfStmt(string lead)
{
    cout << lead << "IfStmt" << endl;

    if (check(lead, "Agar"))
    {
        if (check(lead, "("))
        {
            Expr(lead + "|___");
            if (check(lead, ")"))
            {
                Stmt(lead + "|___");
                ElsePart(lead + "|___");
            }
            else throw runtime_error("Expected ')' in expression");
        }
        else throw runtime_error("Expected '(' in expression");
    }
    else throw runtime_error("Expected 'Agar' in expression");
}

void ElsePart(string lead)
{
    cout << lead << "ElsePart" << endl;
    if (check(lead, "Wagarna"))
    {
        Stmt(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }
}

void CompStmt(string lead)
{
    cout << lead << "CompStmt" << endl;
    if (check(lead, "{"))
    {
        StmtList(lead + "|___");
        if (!check(lead, "}")) throw runtime_error("Expected '}' in expression");
    }
    else throw runtime_error("Expected '{' in expression");
}

void StmtList(string lead)
{
    cout << lead << "StmtList" << endl;
    if (idx < TV.size() && TV[idx].type != "}")
    {
        Stmt(lead + "|___");
        StmtList(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }
}

void Expr(string lead)
{
    cout << lead << "Expr" << endl;
    if (check(lead, "identifier"))
    {
        Expr_(lead + "|___");
    }
    else if (check(lead, "("))
    {
        Expr(lead + "|___");
        if (check(lead, ")"))
        {
            Term__(lead + "|___");
            Mag__(lead + "|___");
            Rvalue_(lead + "|___");
        }
        else throw runtime_error("Expected ')' in expression");
    }
    else if (check(lead, "number"))
    {
        Term__(lead + "|___");
        Mag__(lead + "|___");
        Rvalue_(lead + "|___");
    }
    else throw runtime_error("Expected identifier or number or '(' in expression");
}

void Expr_(string lead)
{
    cout << lead << "Expr_" << endl;
    if (check(lead, ":="))
    {
        Expr(lead + "|___");
    }
    else
    {
        Term__(lead + "|___");
        Mag__(lead + "|___");
        Rvalue_(lead + "|___");
    }
}

void Rvalue(string lead)
{
    cout << lead << "Rvalue" << endl;
    if (check(lead, "identifier"))
    {
        Term__(lead + "|___");
        Mag__(lead + "|___");
        Rvalue_(lead + "|___");
    }
    else if (check(lead, "("))
    {
        Expr(lead + "|___");
        if (check(lead, ")"))
        {
            Term__(lead + "|___");
            Mag__(lead + "|___");
            Rvalue_(lead + "|___");
        }
        else throw runtime_error("Expected ')' in expression");
    }
    else if (check(lead, "number"))
    {
        Term__(lead + "|___");
        Mag__(lead + "|___");
        Rvalue_(lead + "|___");
    }
    else throw runtime_error("Expected identifier, number or '(' in expression");
}

void Compare(string lead)
{
    cout << lead << "Compare" << endl;
    if (check(lead, "==") || check(lead, "<") || check(lead, ">") || check(lead, "<=") || check(lead, ">=") || check(lead, "!=") || check(lead, "<>"))
    {
        return;
    }
    else throw runtime_error("Expected Comparison Operator in expression");
}
void Rvalue_(string lead)
{
    cout << lead << "Rvalue_" << endl;
    if ((idx < TV.size() && TV[idx].type == "==") || (idx < TV.size() && TV[idx].type == "<") || (idx < TV.size() && TV[idx].type == ">") || (idx < TV.size() && TV[idx].type == "<=") || (idx < TV.size() && TV[idx].type == ">=") || (idx < TV.size() && TV[idx].type == "!=") || (idx < TV.size() && TV[idx].type == "<>"))
    {
        Compare(lead + "|___");
        Mag(lead + "|___");
        Rvalue_(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }
}

void Mag(string lead)
{
    cout << lead << "Mag" << endl;
    if (check(lead, "identifier"))
    {
        Term__(lead + "|___");
        Mag__(lead + "|___");
    }
    else if (check(lead, "("))
    {
        Expr(lead + "|___");
        if (check(lead, ")"))
        {
            Term__(lead + "|___");
            Mag__(lead + "|___");
        }
        else throw runtime_error("Expected ')' in expression");
    }
    else if (check(lead, "number"))
    {
        Term__(lead + "|___");
        Mag__(lead + "|___");
    }
    else throw runtime_error("Expected identifier, number or '(' in expression");
}

void Mag__(string lead)
{
    cout << lead << "Mag__" << endl;
    if (idx < TV.size() && (TV[idx].type == "+" || TV[idx].type == "-"))
    {
        Mag_(lead + "|___");
        Mag__(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }
}

void Mag_(string lead)
{
    cout << lead << "Mag_" << endl;
    if (check(lead, "+") || check(lead, "-"))
    {
        Term(lead + "|___");
    }
    else throw runtime_error("Expected '+' or '-' in expression");
}

void Term(string lead)
{
    cout << lead << "Term" << endl;
    if (check(lead, "identifier")) Term__(lead + "|___");
    else if (check(lead, "("))
    {
        Expr(lead + "|___");
        if (check(lead, ")")) Term__(lead + "|___");
        else throw runtime_error("Expected ')' in expression");
    }
    else if (check(lead, "number")) Term__(lead + "|___");
    else throw runtime_error("Expected identifier, number or '(' in expression");
}

void Term__(string lead)
{
    cout << lead << "Term__" << endl;
    if (idx < TV.size() && (TV[idx].type == "*" || TV[idx].type == "/"))
    {
        Term_(lead + "|___");
        Term__(lead + "|___");
        return;
    }
    else
    {
        cout << lead + "|___" + "null" << endl;
        return;
    }
}

void Term_(string lead)
{
    cout << lead << "Term_" << endl;
    if (check(lead, "*") || check(lead, "/"))
    {
        Factor(lead + "|___");
    }
    else throw runtime_error("Expected '*' or '/' in expression");
}

void Factor(string lead)
{
    cout << lead << "Factor" << endl;
    if (check(lead, "("))
    {
        Expr(lead + "|___");
        if (!check(lead, ")")) throw runtime_error("Expected ')' in expression");
    }
    else if (!check(lead, "identifier") && !check(lead, "number"))
        throw runtime_error("Expected identifier, number or '(' in expression");
}

void Parser()
{
    cout << "\n\n PARSE TREE\n" <<endl;
    Function();
    if (idx < TV.size())
        cout << "Error: Unparsed tokens remain!" << endl;
    else
        cout << "Parsing completed successfully!" << endl;
}
