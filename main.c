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
char nextChar;

int main()
{
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
