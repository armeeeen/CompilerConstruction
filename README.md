This project implements the Lexical Analyzer and Parser phases of a compiler using C++, based on a custom programming language grammar and specifications.
Description:-
This compiler front-end is built to scan and validate source code written in a custom-designed language, which includes unique keywords, identifiers, data types, and control structures. It consists of:
1)A Lexical Analyzer that tokenizes the input stream.
2)A Parser that checks syntactic correctness based on the specified grammar.

(A)Lexical Analyzer (Scanner)
 Features
Identifiers: Start with a letter or underscore and must include at least one underscore.
Numbers: Supports signed/unsigned integers, floating point numbers, and exponential notation (e.g., -10.5E+12).
Punctuations: [ , { , ( , ) , } , ], ::
Operators: <, >, <>, :=, ==, +, -, ++, +=, <=, >=, %, ||, &&, !=, *, “, ”, /, <<, >>
Keywords: e.g., Adadi, Mantiqi, Ashriya, Agar, Wagarna, Wapas, input->, output<-, and more.

 Output
A stream of tokens written to a file "tokens.txt"
A list of errors, if invalid tokens are found.
A list of literals written to a file "literal.txt"

(B) Parser
 Grammar Highlights
This custom CFG (Context-Free Grammar) defines functions, conditionals, loops, declarations, and expressions. 
Examples:
Function −> Type identifier ( ArgList ) CompoundStmt
ArgList −> Arg | ArgList , Arg
Arg −> Type identifier
Declaration −> Type IdentList ::
Type −> Adadi | Ashriya | Harf | Matn | Mantiqi
IdentList −> identifier , IdentList | identifier
Stmt −> ForStmt | WhileStmt | Expr :: | IfStmt | CompStmt | Declaration | ::
ForStmt −> for ( Expr :: OptExpr :: OptExpr ) Stmt
OptExpr −> Expr | ⋀
WhileStmt −> while ( Expr ) Stmt
IfStmt −> Agar ( Expr ) Stmt ElsePart
ElsePart −> Wagarna Stmt | ⋀
CompStmt −> { StmtList }
StmtList −> StmtList Stmt | ⋀
Expr −> identifier : = Expr | Rvalue
Rvalue −> Rvalue Compare Mag | Mag
Compare −> == | < | > | <= | >= | ! = | <>
Mag −> Mag + Term | Mag − Term | Term
Term −> Term * Factor | Term / Factor | Factor
Factor −> ( Expr )| identifier| number
 
 Features
1)Implements a top-down parser (recursive descent)
2)Handles nested and compound statements
3)Reports syntax errors clearly with line numbers
