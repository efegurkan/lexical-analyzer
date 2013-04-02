#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*Charachter types*/
#define LETTER 0
#define DIGIT 1
#define OTHER 2

/*Token Codes*/
#define STR_LIT 10
#define IDENTIFIER 11
#define ASSIGNMENT 20


/*Globals*/
int charType;
int nextToken;
int lexLength;
char nextChar;
FILE *stw_fp, *fopen();

/* Function Prototypes*/
void getCh();
void cleanSpace();
int opTokenizer(char ch);
int lex();

int main()
{
    if((stw_fp=fopen("test.stw","r")) == NULL)/*Try to open test.stw*/
        printf("E: Couldn't open test.stw\n");
    else/*Success*/
    {
        getCh();
        do
        {
/* TODO (efegurkan#1#): lexical analyzer */
        }while(nextToken != EOF);
    }
    return 0;
}

/*Gets next char of input and determines its type*/
void getCh()
{
    if((nextChar = getc(stw_fp)) != EOF)
    {
        if(isalpha(nextChar))
            charType = LETTER;

        else if (isdigit(nextChar))
            charType = DIGIT;

            else
                charType = OTHER;
    }
    else charType = EOF;

}

/*Calls getCh until nextChar is not space*/
void cleanSpace()
{
    while (isspace(nextChar))
        getCh();
}

int opTokenizer(char ch)
{
    /* TODO (efegurkan#1#): Left, Right Paranthesis
concat,trim,replace,eol
assign */
    return 0;
}

int lex()
{
    lexLength = 0;
    cleanSpace();

    switch(charType)
    {
        /*Identifiers*/
        case LETTER:
            /* TODO (efegurkan#1#): add character */
            getCh();
            while (charType == LETTER || charType == DIGIT)
            {
                /* TODO (efegurkan#1#): add character */
                getCh();
            }
            nextToken = IDENTIFIER;
            break;

    }/*End of switch*/

    return nextToken;
}
