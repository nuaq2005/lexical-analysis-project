#include <iostream>
#include <fstream>
using namespace std;

/* Global declarations for variables */
int charClass;
char lexeme [100];
char reservedwords[100];
int nextChar; /* nextChar must be int to hold EOF */
int lexLen;
int token;
int nextToken;
int step; /* track each assignment for token and lexeme */
ifstream inputFile;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);
void expr();
void term();
void factor();
void exp();
void assign();

/* Character classes*/
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token Codes*/
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define EXP_OP 25
#define LEFT_PAREN 26
#define RIGHT_PAREN 27
/* Assignment Operators */
#define ASSIGN_ADD_OP 28
#define ASSIGN_SUB_OP 29
#define ASSIGN_MULT_OP 30
#define ASSIGN_DIV_OP 31
/* For assign */
#define UNSIGNED_KEY 40
#define SIGNED_KEY 41
#define BYTE_KEY 42
#define SHORT_KEY 43
#define INT_KEY 44
#define LONG_KEY 45



/* lookup - look up operators and paranthesis and return token */
int lookup(char ch) {
    int peek;
    switch (ch) {
        case '(': 
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            peek = inputFile.peek();
            if (peek == '=') { 
                getChar();
                addChar();
                nextToken = ASSIGN_ADD_OP;
            } else {
                nextToken = ADD_OP;
            }
            break;
        case '-':
            addChar();
            peek = inputFile.peek();
            if (peek == '=') {
                getChar();
                addChar();
                nextToken = ASSIGN_SUB_OP;
            } else {
                nextToken = SUB_OP;
            }
            break;
        case '*':
            addChar();
            peek = inputFile.peek();
            if (peek == '=') {
                getChar();
                addChar();
                nextToken = ASSIGN_MULT_OP;
            } else {
                nextToken = MULT_OP;
            }
            break;
        case '/':
            addChar();
            peek = inputFile.peek();
            if (peek == '=') {
                getChar();
                addChar();
                nextToken = ASSIGN_DIV_OP;
            } else {
                nextToken = DIV_OP;
            }
            break;
        case '^':
            addChar();
            nextToken = EXP_OP;
            break;
        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}


/* addChar - a function to add nextChar to lexeme */
void addChar() {
   if (lexLen <= 98) {
      lexeme[lexLen++] = nextChar;
      lexeme[lexLen] = 0;
   }
   else
      cout << "Error - lexeme is too long \n";
}

/* getChar - a function to get the next character of input 
and determine its character class */

void getChar() {

    if(inputFile.eof()) {
        charClass = EOF;
        nextChar = EOF;
        return;
    }

    nextChar = inputFile.get();

    if (isalpha(nextChar)) {
        charClass = LETTER;
    }
    else if (isdigit(nextChar)){
        charClass = DIGIT;
    } 
    else {
        charClass = UNKNOWN;
    }
}


   /* getNonBlank - a function to call getChar until it returns a non-blank character */
   void getNonBlank() {
    while (isspace(nextChar))
      getChar();
    }

    bool isKeyWord(const char* str, const char* keyword) {
        int i = 0;
        while (str[i] != '\0' && keyword[i] != '\0') {
            if (str[i] != keyword[i]) {
                return false;
            }
            i++;
        }
        return str[i] == '\0' && keyword[i] == '\0';
    }

/* lex - a simple lexical analyzer for arithmetic expressions */

int lex() {
   lexLen = 0;
   getNonBlank();
   switch (charClass) {
      /* Parse identifiers */
      case LETTER: 
         addChar();
         getChar();
         while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
         } //if first lex is a letter, it is treated as identifier, loop breaks once a non-letter/digit is found
         if (isKeyWord(lexeme, "unsigned")){
            nextToken = UNSIGNED_KEY;
         } else if (isKeyWord(lexeme, "signed")){
            nextToken = SIGNED_KEY;
         } else if (isKeyWord(lexeme, "byte")){
            nextToken = BYTE_KEY;
         } else if (isKeyWord(lexeme, "short")){
            nextToken = SHORT_KEY;
         } else if (isKeyWord(lexeme, "int")){
            nextToken = INT_KEY;
         } else if (isKeyWord(lexeme, "long")){
            nextToken = LONG_KEY;
         } else {
            nextToken = IDENT;
         }
         break;

        /* Parse integer literals */
        case DIGIT:
             addChar();
             getChar();
             while (charClass == DIGIT) {
                addChar();
                getChar();
             } //if first lex is a digit, it is treated as integer literal, loop breaks once a non-digit is found
             nextToken = INT_LIT;
             break;

        /* Parentheses and operators */
        case UNKNOWN: //if char is not letter or digit, it is either operator, parenthesis or unknown
             lookup(nextChar);
             if (nextToken != EOF) getChar(); //changed
             break;

        /* EOF */
        case EOF:
             nextToken = EOF;
             lexeme[0] = 'E';
             lexeme[1] = 'O';
             lexeme[2] = 'F';
             lexeme[3] = 0;
             break;
    } /* End of switch */
    cout << step++ << ". Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
   return nextToken;
} /* End of function lex */

/*Syntax Analyzer*/

void exp(){
    cout << step++ << ". Enter <exp> \n";

    //determine which path to take
    if (nextToken == IDENT || nextToken == INT_LIT){
        lex(); //next token
    } else if (nextToken == LEFT_PAREN){
        lex(); //get past '('
        step++;
        expr(); //go to expression
        if (nextToken == RIGHT_PAREN){
            lex(); //get past ')'
        } else {
            cout << "Syntax Error \n";
        }
    } else {
        cout << "Syntax Error \n";
    }
    cout << step++ << ". Exit <exp> \n";
}

void factor() {
    cout << step++ << ". Enter <factor> \n";

    //first we must have a factor
    exp();

    //then we can have 0 or more (^) factor
    while (nextToken == EXP_OP) {
        lex(); //get the operator
        exp(); //get the next factor
    }

    cout << step++ << ". Exit <factor> \n";
}

void term (){
    cout << step++ << ". Enter <term> \n";

    //first we must have a factor
    factor();

    //then we can have 0 or more (* | /) factor
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex(); //get the operator
        factor(); //get the next factor
    }
    cout << step++ << ". Exit <term> \n";
}

/* expr
 Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>}
 */

void expr() {
    cout << step++ << ". Enter <expr> \n";

    //print first term
    term();

    //then we can have 0 or more (+ | - | *) term
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex(); //get the operator
        term(); //get the next term
    }
    cout << step++ << ". Exit <expr> \n";
}

// assign -> [unsigned/signed] (byte| short | int | long ) <ident> (= | += | -= | *= | /= ) <expression> ;

void assign() {
    cout << step++ << ". Enter <assign> \n"; //every line begins here

    // Optional: unsigned or signed
    if (nextToken == UNSIGNED_KEY || nextToken == SIGNED_KEY) {
        lex(); // consume the unsigned/signed keyword
    }

    // Optional: byte, short, int, or long
    if (nextToken == BYTE_KEY || nextToken == SHORT_KEY || 
        nextToken == INT_KEY || nextToken == LONG_KEY) {
        lex(); // consume the type keyword
    }

    //first lexeme must be identifier
    if(nextToken == IDENT){ 
        lex();
    } else {
        cout << "Syntax Error \n";
    }

    //second lexeme must be identifier
    if(nextToken == ASSIGN_OP || nextToken == ASSIGN_ADD_OP || nextToken == ASSIGN_SUB_OP || nextToken == ASSIGN_MULT_OP || nextToken == ASSIGN_DIV_OP){
        lex();
    } else {
        cout << "Syntax Error \n";
    }

    //third lexeme must be expression
    expr(); //we enter expression, which takes us through the terms/factors/operations

    cout << step++ << ". Exit <assign> \n";
}

int main () {
   /* Open input file and process contents */
   inputFile.open("front.in");

   if (!inputFile.is_open()) {
      cout << "ERROR - cannot open front.in \n";
      return 1;
   } else {
    getChar();
    lex();
    do{
        assign();
        cout << '\n';
    } while (nextToken != EOF);
}
   inputFile.close();
   return 0;
}