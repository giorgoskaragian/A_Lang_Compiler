%option noyywrap
%option yylineno

%{
#include "parser.hpp"
#include <malloc.h> // malloc(), free()
#include <stdlib.h> // atoi(), atof()
#include <string.h> // strdup()

// Helper struct for dynamically sized strings (used in multiline_comment() and STRING condition)
typedef struct string_t
{
	char* m_Buffer;
	unsigned int m_Size;
	unsigned int m_Capacity;
} string;

void stringInit(string* str)
{
	str->m_Buffer = 0;
	str->m_Size = 0;
	str->m_Capacity = 0;
}

void stringClear(string* str)
{
	str->m_Size = 0;
}

void stringDestroy(string* str)
{
	free(str->m_Buffer);
	str->m_Buffer = 0;
	str->m_Size = 0;
	str->m_Capacity = 0;
}

void stringPushBack(string* str, char c)
{
	if(str->m_Size + 1 > str->m_Capacity) {
		str->m_Capacity = (str->m_Capacity == 0) ? 16 : ((str->m_Capacity * 3) / 2);
		str->m_Buffer = (char*)realloc(str->m_Buffer, str->m_Capacity);
	}

	str->m_Buffer[str->m_Size++] = c;
}

string g_String;

int yyinput();

int skip_multiline_comment(int lineno)
{
	int c;
	int nestLevel = 1; // Already inside a comment

	while (nestLevel > 0) {
		// Skip all characters until we find:
		// 1) an asterisk which might mark the end of the current comment if the next char is a slash
		// 2) a slash which might mark the start of a nested comment
		// 3) the end-of-file character, which is an error!
		while((c = yyinput()) != '*' && c != EOF && c != '/') {
		}

		if(c == '*') {
			// Skip all extra asterisks 
			while((c = yyinput()) == '*') {
			}

			// If the current character is a slash, the current comment block is terminated.
			if(c == '/') {
				--nestLevel;
			}

			// Otherwise, this was just a series of askerisks so we are still inside the comment block.
		} else if(c == '/') {
			// Skip all extra slashes
			while((c = yyinput()) == '/') {
			}

			// If the current character is an asterisk, start a new nested comment block.
			if(c == '*') {
				++nestLevel;
			}

			// Otherwise, this was just a series of slashes so we are still in the previous comment block.
		}

		// If the last read character is the EOF char report an error
		if(c == EOF) {
			fprintf(stderr, "ERROR: End-of-File reached inside multi-line comment (started at line %d)\n", lineno);
			return 0;
		}
	}

	return 1;
}
%}

digit     [0-9]
id        [a-zA-Z][a-zA-Z_0-9]*
integer   {digit}+
real      {digit}+"."{digit}*
space     [ \t\n\r]

%x STRING
%%

"/*"       {
	if(!skip_multiline_comment(yylineno)) {
		return 0;
	}
}

"//".*     { /* Skip single line comment */ }

"if"       { return IF; }
"else"     { return ELSE; }
"while"    { return WHILE; }
"for"      { return FOR; }
"function" { return FUNCTION; }
"return"   { return RETURN; }
"break"    { return BREAK; }
"continue" { return CONTINUE; }
"and"      { return AND; }
"not"      { return NOT; }
"or"       { return OR; }
"local"    { return LOCAL; }
"true"     { return TRUE; }
"false"    { return FALSE; }
"nil"      { return NIL; }

"++"       { return INCREMENT; }
"--"       { return DECREMENT; }
"<="       { return LESS_THAN_OR_EQUAL; }
">="       { return GREATER_THAN_OR_EQUAL; }
"=="       { return DOUBLE_EQUAL; }
"!="       { return NOT_EQUAL; }
"="        { return EQUAL; }
"+"        { return PLUS; }
"-"        { return MINUS; }
"*"        { return MUL; }
"/"        { return DIV; }
"%"        { return MOD; }
">"        { return GREATER_THAN; }
"<"        { return LESS_THAN; }

"::"       { return DOUBLE_COLON; }
".."       { return DOUBLE_DOT; }
"{"        { return OPEN_CURLY_BRACKET; }
"}"        { return CLOSE_CURLY_BRACKET; }
"["        { return OPEN_BRACKET; }
"]"        { return CLOSE_BRACKET; }
"("        { return OPEN_PARENTHESIS; }
")"        { return CLOSE_PARENTHESIS; }
";"        { return SEMICOLON; }
","        { return COMMA; }
":"        { return COLON; }
"."        { return DOT; }

{real}     { yylval.doubleVal = atof(yytext); return REAL_CONST; }
{integer}  { yylval.intVal = atoi(yytext);    return INTEGER_CONST; }
{id}       { yylval.astNode = astCreateIdentifier(yylineno, yytext);  return IDENTIFIER; }
{space}    { /* Ignore whitespaces */ }

\"         { 
	stringClear(&g_String);

	BEGIN(STRING); 
}

<STRING>\" {
	stringPushBack(&g_String, '\0');
	yylval.strVal = strdup(g_String.m_Buffer);
	stringDestroy(&g_String);

	BEGIN(INITIAL);
	return STRING_CONST;
}

<STRING>\\[^ntr\\\"] {
	fprintf(stderr, "ERROR: Illegal character escape sequence found in string at line %d\n", yylineno);
	return 0;
}

<STRING>\\[ntr\\\"]|. {
	const char* yytextcpy = yytext;
	while(*yytextcpy != '\0') {
		stringPushBack(&g_String, *yytextcpy++);
	}
}

<STRING>\n {
	fprintf(stderr, "ERROR: Newline character found in string at line %d\n", yylineno);
	return 0;
}

<STRING><<EOF>> { 
	fprintf(stderr, "ERROR: End-of-File reached inside string constant\n"); 
	return 0; 
}

. {
	/* Everything else is an unknown token. Stop the lexer */
	fprintf(stderr, "ERROR: Invalid token '%s' found at line %d.\n", yytext, yylineno);
	return 0;
}

%%
