#include <iostream>
#include <fstream>
using namespace std;

/* Global declarations for variables */
int charClass;
char lexeme [100];
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
// assign -> [unsigned/signed] (byte| short | int | long ) <ident> (= | += | -= | *= | /= | %= ) <expression> ;


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

    if (nextChar == '\n') {
        step = 1; //reset step count for new expression 
    }
    
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
         nextToken = IDENT;
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
             getChar();
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
    cout << step << ". Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
   return nextToken;
} /* End of function lex */

int main () {
   /* Open input file and process contents */
   inputFile.open("front.in");

   if (!inputFile.is_open()) {
      cout << "ERROR - cannot open front.in \n";
      return 1;
   }
    getChar();
    do {
        step++;
        lex();
    } while (nextToken != EOF);

   inputFile.close();
   return 0;
}
