#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*Charachter types*/
#define LETTER 0
#define DIGIT 1
#define OTHER 2

/*Token Codes*/
#define STR_LIT 10
#define IDENTIFIER 11
#define KEYWORD_READ 12
#define KEYWORD_WRITE 13
#define KEYWORD_TO 14
#define KEYWORD_FROM 15
#define ASSIGNMENT_OP 20
#define LEFT_PAR 21
#define RIGHT_PAR 22
#define EOL 23
#define CONCAT_OP 24
#define TRIM_OP 25
#define TRUNC_OP 26
#define REPL_OP_COLON 27
#define REPL_OP_LESS 28

/*Exception Codes*/
#define EX_LONG_ID 100
#define EX_STRING_UNCLOSED 101
#define EX_COMMENT_UNCLOSED 102
#define EX_ID_DIGIT 103
#define EX_UNKNOWN 104

/*Error codes*/
#define ERROR_ID 105
#define ERROR_UNCL 106

/*Globals*/
int charType;
int nextToken;
int lexLength;
int lineNo = 0;
int errorFlag = 0;
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
void goStablePosition(int errorCode);
void exceptionHandler(int exception);
int isKeyword();

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
        }
        while(nextToken != EOF);

        fclose(stw_fp);
        fclose(tkn_fp);

        if (errorFlag != 0)
        {
            remove("tokens.tkn");
        }
    }
    return 0;
}

/*Adds identifiers name to lexeme.*/
void addCh()
{
    if (lexLength <=19)
    {
        lexeme[lexLength++] = nextChar;
        lexeme[lexLength] = 0;
    }
    else
    {
        exceptionHandler(EX_LONG_ID);
    }
}

/*Tries to go to the next stable position according to error code*/
void goStablePosition(int errorCode)
{
    getCh();
    switch(errorCode)
    {
    case ERROR_ID:/*Identifier related errors*/
        while (charType == LETTER || charType == DIGIT)
        {
            getCh();
        }
        break;

    case ERROR_UNCL:/*Unclosed comment or string literal*/
        while (nextChar== ';' || nextChar == EOF)
        {
            getCh();
        }
        break;
    }
}

/*Writes string literals to the token file*/
void stringLit()
{
    do
    {
        putc(nextChar,tkn_fp);
        getCh();
    }
    while(nextChar != '"');
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

    if(nextChar == '\n')
        lineNo++;
}

/*Calls getCh until nextChar is not space*/
void cleanSpace()
{
    while (isspace(nextChar) || nextChar == '\n')
        getCh();
}

/*Tries to find what is the next token.*/
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
        if(nextChar == '*')/*Is it comment or Trimming OP*/
        {
            do/*Go through EOF*/
            {
                getCh();
                if(nextChar=='*')
                {
                    position2 = ftell(stw_fp);/*Take * character's position*/
                    getCh();
                    if(nextChar == '/')/*If there is / after * this is comment*/
                    {
                        nextToken = 0;/*Comments not included tokens*/
                        break;
                    }
                    else
                        fseek(stw_fp,position2,SEEK_SET);/*there is no / after * character*/
                }
            }
            while(nextChar != EOF);

            if (nextChar == EOF )/*Unclosed comment exception*/
            {
                exceptionHandler(EX_COMMENT_UNCLOSED);
            }
        }
        else/*Trimming Operator*/
        {
            nextToken = TRIM_OP;
            fseek(stw_fp,position,SEEK_SET);
        }
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
        else
        {
            exceptionHandler(EX_UNKNOWN);/*If it doesn't match any*/
        }
        break;

    case '"':/*String literal*/
        position = ftell(stw_fp);/*For backtracing*/

        /*Read through EOF or " */
        do
        {
            getCh();
        }
        while(nextChar!= EOF && nextChar != '"');

        if (nextChar == '"')/*No error Return to the startting point*/
        {
            fseek(stw_fp,position-1,SEEK_SET);
            nextToken = STR_LIT;
        }
        else if(nextChar == EOF)/*Unclosed comment exception*/
        {
            exceptionHandler(EX_STRING_UNCLOSED);
        }
        break;

    default:
        exceptionHandler(EX_UNKNOWN);/*Unknown charachter*/
        break;
    }
    return nextToken;
}

void exceptionHandler(int exception)
{
    switch(exception)
    {
    case EX_LONG_ID:
        printf("E:Line %d: Identifier is too long!\n",lineNo);
        goStablePosition(ERROR_ID);
        break;

    case EX_ID_DIGIT:
        printf("E:Line %d: Identifiers MUST start with a letter!\n",lineNo);
        goStablePosition(ERROR_ID);
        break;

    case EX_COMMENT_UNCLOSED:
        printf("E:Line %d: Unclosed comment!\n",lineNo);
        fseek(stw_fp,position,SEEK_SET);
        goStablePosition(ERROR_UNCL);
        nextToken = 0;
        break;

    case EX_STRING_UNCLOSED:
        printf("E:Line %d: Unclosed String Literal!\n",lineNo);
        fseek(stw_fp,position,SEEK_SET);
        goStablePosition(ERROR_UNCL);
        nextToken = 0;
        break;

    case EX_UNKNOWN:
        printf("E:Line %d: Unknown charachter!\n",lineNo);
        nextToken = 0;
        break;

    }

    errorFlag = 1;
}

/*Defines if the lexeme is a keyword or identifier*/
int isKeyword()
{
    if(strcmp(lexeme,"READ") == 0)
        return KEYWORD_READ;
    else if (strcmp(lexeme,"WRITE") == 0)
        return KEYWORD_WRITE;
    else if (strcmp(lexeme, "TO") == 0)
        return KEYWORD_TO;
    else if (strcmp(lexeme,"FROM") == 0)
        return KEYWORD_FROM;

    return IDENTIFIER;
}
/*Looks lexemes and decides the token charachters.*/
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
        nextToken = isKeyword();
//        nextToken = IDENTIFIER;
        break;

    case DIGIT:
        exceptionHandler(EX_ID_DIGIT);
        break;

    case OTHER:
        opTokenizer(nextChar);
        getCh();
        break;

    case EOF:
        nextToken = EOF;
        break;

    }/*End of switch*/
    if (nextToken != 0)
    {
        fprintf(tkn_fp,"%d",nextToken);
        if (nextToken == STR_LIT)
        {
            stringLit();
        }
        else if (nextToken == IDENTIFIER)
        {
            putc('"',tkn_fp);
            fputs(lexeme, tkn_fp);
            putc('"',tkn_fp);
        }
        putc(',',tkn_fp);
    }

    return nextToken;
}
