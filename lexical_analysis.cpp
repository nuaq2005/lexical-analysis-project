#include <stdio.h>
#include <ctype.h>

/* Global declarations for variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen;

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
//#define ASSIGN_ADD_OP 27
//#define ASSIGN_SUB_OP 28
//#define ASSIGN_MUL_OP 29
//#define ASSIGN_DIV_OP 30
//#define ASSIGN_MOD_OP 31
//#define ASSIGN_ADD_OP 32
// assign -> [unsigned/signed] (byte| short | int | long ) <ident> (= | += | -= | *= | /= | %= ) <expression> ;




int main () {
   /* Open input file and process contents */
   if ((in_fp = fopen("front.in", "r")) == NULL)
   printf("ERROR - cannot open front.in \n");
   else { 
      getChar();
      do {
         lex();
      } while (nextToken != EOF);
   }
}


/* lookup - look up operators and paranthesis and return token */
int lookup(char ch) {
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
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '^':
            addChar();
            nextToken = EXP_OP;
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
      printf("Error - lexeme is too long \n");
}

/* getChar - a function to get the next character of input 
and determine its character class */

void getChar() {
   if ((nextChar = getc(in_fp)) = EOF){
    if(isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
        charClass = DIGIT;
    else 
      charClass = UNKNOWN;
   }
   else
    charClass = EOF;
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
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
   return nextToken;
} /* End of function lex */


