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
        step = 0;
        return;
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
    step++;
    cout << step << ". Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
   return nextToken;
} /* End of function lex */

/*Syntax Analyzer*/

void exp(){
    cout << step << ". Enter <exp> \n";

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
    step++; 
    cout << step << ". Exit <exp> \n";
}

void factor() {
    cout << step << ". Enter <factor> \n";

    //first we must have a factor
    step++;
    exp();

    //then we can have 0 or more (^) factor
    while (nextToken == EXP_OP) {
        lex(); //get the operator
        exp(); //get the next factor
    }
    step++;
    cout << step << ". Exit <factor> \n";
}

void term (){
    cout << step << ". Enter <term> \n";

    //first we must have a factor
    step++;
    factor();

    //then we can have 0 or more (* | /) factor
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex(); //get the operator
        factor(); //get the next factor
    }
    step++; 
    cout << step << ". Exit <term> \n";
}

/* expr
 Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>}
 */

void expr() {
    cout << step << ". Enter <expr> \n";

    //print first term
    step++; 
    term();

    //then we can have 0 or more (+ | - | *) term
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex(); //get the operator
        term(); //get the next term
    }
    step++;
    cout << step << ". Exit <expr> \n";
}

// assign -> [unsigned/signed] (byte| short | int | long ) <ident> (= | += | -= | *= | /= | %= ) <expression> ;
void assign() {
    cout << step << ". Enter <assign> \n"; //every line begins here

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
    step++;
    expr(); //we enter expression, which takes us through the terms/factors/operations

    cout << step << ". Exit <assign> \n";
}

void filltempLexeme(char* source, char* destination) {
    int i = 0;
    while (source[i]) {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

int main () {
   /* Open input file and process contents */
   inputFile.open("front.in");

   if (!inputFile.is_open()) {
      cout << "ERROR - cannot open front.in \n";
      return 1;
   }
   //this is to store the current step and lexeme when deciding between assign() and expr()
   int tempStep = step;
   char tempLexeme[100];
   
   getChar();
   lex();

    while (nextToken != EOF) {
        if (nextToken == IDENT) {
        int i = 0;
        while(lexeme[i]){
            tempLexeme[i] = lexeme[i];
            i++;
        }   
        tempLexeme[i] = '\0';

        lex();
        if (nextToken == ASSIGN_OP || nextToken == ASSIGN_ADD_OP || nextToken == ASSIGN_SUB_OP || nextToken == ASSIGN_MULT_OP || nextToken == ASSIGN_DIV_OP) {
            //restore lexeme and step for assign()
            filltempLexeme(tempLexeme, lexeme);
            step = tempStep;
            nextToken = IDENT; //reset nextToken to IDENT for assign()
            assign();
        } else {
            filltempLexeme(tempLexeme, lexeme);
            step = tempStep;
            nextToken = IDENT; //reset nextToken to IDENT for expr()
            expr();
        }
        } else {
            expr();
        }
        lex();
   }

   inputFile.close();
   return 0;
}