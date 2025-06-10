#ifndef PARSER_H
#define PARSER_H

#include<iostream>
#include <string>
#include <vector>
using namespace std;

class token 
{
public:
    string type;
    token(string type);
};

extern vector<token> TV;
extern int idx;

void Parser();

#endif
