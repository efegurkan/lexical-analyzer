#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*Charachter types*/
#define LETTER 0
#define OPERATOR 1

/*Token Codes*/
#define IDENTIFIER 10
#define ASSIGNMENT 11

/*Globals*/
int charType;
int nextToken;
char nextChar;
FILE *stw_fp, *fopen();

/* Function Prototypes*/
void getCh();
void cleanSpace();
int opTokenizer(char ch);

int main()
{
    if((stw_fp=fopen("test.stw","r")) == NULL)
        printf("E: Couldn't open test.stw\n");
    else
    {
        getCh();
        do
        {
/* TODO (efegurkan#1#): lexical analyzer */
        }while(nextToken != EOF);
    }
    return 0;
}

void getCh()
{
    if((nextChar = getc(stw_fp)) != EOF)
    {
        if(isalpha(nextChar))
            charType = LETTER;
        else charType = OPERATOR;
    }
    else charType = EOF;

}

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
