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
#define COMMENT_ST 12
#define COMMENT_END 13
#define ASSIGNMENT_OP 20
#define LEFT_PAR 21
#define RIGHT_PAR 22
#define EOL 23
#define CONCAT_OP 24
#define TRIM_OP 25
#define TRUNC_OP 26
#define REPL_OP_COLON 27
#define REPL_OP_LESS 28


/*Globals*/
int charType;
int nextToken;
int lexLength;
char nextChar;
char lexeme[20];
long int position,position2;
FILE *stw_fp, *tkn_fp, *fopen();

/* Function Prototypes*/
void getCh();
void cleanSpace();
int opTokenizer(char ch);
int lex();
void addCh();

int main()
{
    if((stw_fp=fopen("test.stw","r")) == NULL)/*Try to open test.stw*/
        printf("E: Couldn't open test.stw\n");
    else/*Success*/
    {
        tkn_fp = fopen("tokens.tkn", "w");
        getCh();
        do
        {
            lex();
        }while(nextToken != EOF);
    }
    return 0;
}

/*Adds identifiers name to lexeme.*/
void addCh()
{
    if ( lexLength <=19)
    {
        lexeme[lexLength++] = nextChar;
        lexeme[lexLength] = 0;
    }
//    else
    /* TODO (efegurkan#1#): error handling */
}

void stringLit()
{
    do
    {
        putc(nextChar,tkn_fp);
        getCh();
    }while(nextChar != '"');
    putc(nextChar,tkn_fp);
    getCh();
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
    while (isspace(nextChar) || nextChar == '\n')
        getCh();
}

int opTokenizer(char ch)
{
    switch(ch)
    {
        case '(':
           nextToken = LEFT_PAR;
           break;

        case ')':
            nextToken = RIGHT_PAR;
            break;

        case ';':
            nextToken = EOL;
            break;

        case '+':
            nextToken = CONCAT_OP;
            break;

        case '/':
            position = ftell(stw_fp);
            getCh();
            if(nextChar == '*')
                nextToken = COMMENT_ST;
            else
                nextToken = TRIM_OP;
                fseek(stw_fp,position,SEEK_SET);
            break;

        case '*':
            getCh();
            if (nextChar== '/')
                nextToken = COMMENT_END;
                /* TODO (efegurkan#1#): Else do error handling. */
            break;

        case '<':
            nextToken = REPL_OP_LESS;
            break;

        case ':':
            position = ftell(stw_fp);/*Take current position for possible errors*/
            cleanSpace();
            getCh();

            if (nextChar == '=')/*Assignment*/
                nextToken = ASSIGNMENT_OP;
            else if(nextChar == '/')/*Truncation*/
                nextToken = TRUNC_OP;
            else if (nextToken == '"')/*Replacement.In case of encountering with a string literal*/
                {
                    nextToken = REPL_OP_COLON;
                    fseek(stw_fp,position,SEEK_SET);
                }
            /* TODO (efegurkan#1#): else error message */
            break;

        case '"':
            position = ftell(stw_fp);

            do
            {
                getCh();
            }while(nextChar!= EOF && nextChar != '"');

            if (nextChar == '"')
            {
                fseek(stw_fp,position-1,SEEK_SET);
//                stw_fp--;
                nextToken = STR_LIT;
            }
//            else if(nextChar == EOF)
                /*Error message*/
            break;


    }
    return nextToken;
}

int lex()
{
    lexLength = 0;
    cleanSpace();

    switch(charType)
    {
        /*Identifiers*/
        case LETTER:
            addCh();
            getCh();
            while (charType == LETTER || charType == DIGIT)
            {
                addCh();
                getCh();
            }
            nextToken = IDENTIFIER;
            break;

        case DIGIT:
            /*Error message*/
            break;

        case OTHER:
            opTokenizer(nextChar);
            getCh();
            break;

        case EOF:
            nextToken = EOF;
            break;

    }/*End of switch*/
    fprintf(tkn_fp,"%d",nextToken);
    if (nextToken == STR_LIT)
    {
        stringLit();
    }else if (nextToken == IDENTIFIER)
        {
            putc('"',tkn_fp);
            fputs(lexeme, tkn_fp);
            putc('"',tkn_fp);
        }
    putc(',',tkn_fp);

    return nextToken;
}
