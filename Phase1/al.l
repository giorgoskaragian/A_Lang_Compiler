%option noyywrap
%option yylineno

%{
#include <malloc.h> // malloc(), free()
#include <memory.h> // memset()
#include <stdlib.h> // atoi(), atof()

#define YY_DECL int alpha_lex(void* yylval)

typedef double real;

int g_NextTokenID;

typedef enum TokenType_t
{
	TT_Unknown = 0,

	TT_Keyword,
	TT_Operator,
	TT_Integer,
	TT_Real,
	TT_String,
	TT_Delimiter,
	TT_Identifier,
	TT_Comment
} TokenType;

typedef enum Keyword_t
{
	K_Unknown = 0,

	K_If,
	K_Else,
	K_While,
	K_For,
	K_Function,
	K_Return,
	K_Break,
	K_Continue,
	K_And,
	K_Not,
	K_Or,
	K_Local,
	K_True,
	K_False,
	K_Nil,
} Keyword;

typedef enum Operator_t
{
	O_Unknown = 0,

	O_Equal,
	O_Plus,
	O_Minus,
	O_Mul,
	O_Div,
	O_Mod,
	O_DoubleEqual,
	O_NotEqual,
	O_Increment,
	O_Decrement,
	O_GreaterThan,
	O_LessThan,
	O_GreaterThanOrEqual,
	O_LessThanOrEqual,
} Operator;

typedef enum Delimiter_t
{
	D_Unknown = 0,

	D_CurlyBracketOpen,
	D_CurlyBracketClose,
	D_BracketOpen,
	D_BracketClose,
	D_ParenthesisOpen,
	D_ParenthesisClose,
	D_Semicolon,
	D_Comma,
	D_Colon,
	D_DoubleColon,
	D_Dot,
	D_DoubleDot
} Delimiter;

typedef struct alpha_token_t
{
	int m_LineNo;
	int m_TokenID;
	TokenType m_Type;
	char* m_Text;

	union {
		char* m_String;
		Keyword m_Keyword;
		Operator m_Operator;
		Delimiter m_Delimiter;
		int m_IntConst;
		real m_RealConst;
	};
} alpha_token_t;

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

// strndup replacement for Windows
char* stringndup(const char* text, int textLen)
{
	char* str = (char*)malloc(textLen + 1);
	memcpy(str, text, textLen);
	str[textLen] = '\0';
	return str;
}

void escapeStringInplace(char* str)
{
	char* dst = str;
	char c;
	while((c = *str++) != '\0') {
		if(c == '\\') {
			if(*str == 'n') {
				++str;
				*dst++ = '\n';
			} else if(*str == 't') {
				++str;
				*dst++ = '\t';
			} else if(*str == '\\') {
				++str;
				*dst++ = '\\';
			} else if(*str == '"') {
				++str;
				*dst++ = '\"';
			} else {
				*dst++ = c;
			}
		} else {
			*dst++ = c;
		}
	}

	*dst = '\0';
}

void setToken(alpha_token_t* tok, int lineno, const char* text, int textLen, TokenType type)
{
	tok->m_LineNo = lineno;
	tok->m_TokenID = g_NextTokenID++;
	tok->m_Type = type;
	tok->m_Text = stringndup(text, textLen);
}

void setToken_String(alpha_token_t* tok, int lineno, const char* text, int textLen)
{
	setToken(tok, lineno, text, textLen, TT_String);

	tok->m_String = stringndup(text, textLen);
	escapeStringInplace(tok->m_String);
}

void setToken_Keyword(alpha_token_t* tok, int lineno, const char* text, int textLen, Keyword keyword)
{
	setToken(tok, lineno, text, textLen, TT_Keyword);
	tok->m_Keyword = keyword;
}

void setToken_Operator(alpha_token_t* tok, int lineno, const char* text, int textLen, Operator op)
{
	setToken(tok, lineno, text, textLen, TT_Operator);
	tok->m_Operator = op;
}

void setToken_Delimiter(alpha_token_t* tok, int lineno, const char* text, int textLen, Delimiter del)
{
	setToken(tok, lineno, text, textLen, TT_Delimiter);
	tok->m_Delimiter = del;
}

void setToken_Integer(alpha_token_t* tok, int lineno, const char* text, int textLen)
{
	setToken(tok, lineno, text, textLen, TT_Integer);
	tok->m_IntConst = atoi(text);
}

void setToken_Real(alpha_token_t* tok, int lineno, const char* text, int textLen)
{
	setToken(tok, lineno, text, textLen, TT_Real);
	tok->m_RealConst = (real)atof(text);
}

static int input();

int multiline_comment(string* str, int lineno)
{
	int c;
	int nestLevel = 1; // Already inside a comment

	// Push the opening tag to the string.
	stringPushBack(str, '/');
	stringPushBack(str, '*');

	while (nestLevel > 0) {
		// Skip all characters until we find:
		// 1) an asterisk which might mark the end of the current comment if the next char is a slash
		// 2) a slash which might mark the start of a nested comment
		// 3) the end-of-file character, which is an error!
		while((c = input()) != '*' && c != EOF && c != '/') {
			stringPushBack(str, c);
		}
		stringPushBack(str, c);

		if(c == '*') {
			// Skip all extra asterisks 
			while((c = input()) == '*') {
				stringPushBack(str, c);
			}
			stringPushBack(str, c);

			// If the current character is a slash, the current comment block is terminated.
			if(c == '/') {
				--nestLevel;
			}

			// Otherwise, this was just a series of askerisks so we are still inside the comment block.
		} else if(c == '/') {
			// Skip all extra slashes
			while((c = input()) == '/') {
				stringPushBack(str, c);
			}
			stringPushBack(str, c);

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
	int lineno = yylineno;

	string s;
	stringInit(&s);
	if(!multiline_comment(&s, yylineno)) {
		stringDestroy(&s);
		return TT_Unknown;
	}

	setToken((alpha_token_t*)yylval, lineno, s.m_Buffer, s.m_Size, TT_Comment);
	stringDestroy(&s);

	return TT_Comment;
}

"//".*     { setToken((alpha_token_t*)yylval, yylineno, yytext, yyleng, TT_Comment); return TT_Comment; }

"if"       { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_If); return TT_Keyword; }
"else"     { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Else); return TT_Keyword; }
"while"    { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_While); return TT_Keyword; }
"for"      { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_For); return TT_Keyword; }
"function" { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Function); return TT_Keyword; }
"return"   { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Return); return TT_Keyword; }
"break"    { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Break); return TT_Keyword; }
"continue" { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Continue); return TT_Keyword; }
"and"      { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_And); return TT_Keyword; }
"not"      { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Not); return TT_Keyword; }
"or"       { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Or); return TT_Keyword; }
"local"    { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_Local); return TT_Keyword; }
"true"     { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_True); return TT_Keyword; }
"false"    { setToken_Keyword((alpha_token_t*)yylval, yylineno, yytext, yyleng, K_False); return TT_Keyword; }

"++"       { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Increment); return TT_Operator; }
"--"       { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Decrement); return TT_Operator; }
"<="       { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_LessThanOrEqual); return TT_Operator; }
">="       { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_GreaterThanOrEqual); return TT_Operator; }
"=="       { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_DoubleEqual); return TT_Operator; }
"!="       { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_NotEqual); return TT_Operator; }
"="        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Equal); return TT_Operator; }
"+"        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Plus); return TT_Operator; }
"-"        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Minus); return TT_Operator; }
"*"        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Mul); return TT_Operator; }
"/"        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Div); return TT_Operator; }
"%"        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_Mod); return TT_Operator; }
">"        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_GreaterThan); return TT_Operator; }
"<"        { setToken_Operator((alpha_token_t*)yylval, yylineno, yytext, yyleng, O_LessThan); return TT_Operator; }

"::"       { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_DoubleColon); return TT_Delimiter; }
".."       { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_DoubleDot); return TT_Delimiter; }
"{"        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_CurlyBracketOpen); return TT_Delimiter; }
"}"        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_CurlyBracketClose); return TT_Delimiter; }
"["        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_BracketOpen); return TT_Delimiter; }
"]"        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_BracketClose); return TT_Delimiter; }
"("        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_ParenthesisOpen); return TT_Delimiter; }
")"        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_ParenthesisClose); return TT_Delimiter; }
";"        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_Semicolon); return TT_Delimiter; }
","        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_Comma); return TT_Delimiter; }
":"        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_Colon); return TT_Delimiter; }
"."        { setToken_Delimiter((alpha_token_t*)yylval, yylineno, yytext, yyleng, D_Dot); return TT_Delimiter; }

{real}     { setToken_Real((alpha_token_t*)yylval, yylineno, yytext, yyleng); return TT_Real; }
{integer}  { setToken_Integer((alpha_token_t*)yylval, yylineno, yytext, yyleng); return TT_Integer; }
{id}       { setToken((alpha_token_t*)yylval, yylineno, yytext, yyleng, TT_Identifier); return TT_Identifier; }
{space}    { /* Ignore whitespaces */ }

\"         { 
	stringClear(&g_String);
	stringPushBack(&g_String, '\"');

	BEGIN(STRING); 
}

<STRING>\" {
	stringPushBack(&g_String, '\"');
	setToken_String((alpha_token_t*)yylval, yylineno, g_String.m_Buffer, g_String.m_Size);
	BEGIN(INITIAL);
	return TT_String;
}

<STRING>\\[^ntr\\\"] {
	fprintf(stderr, "ERROR: Illegal character escape sequence found in string at line %d\n", yylineno);
	return TT_Unknown;
}

<STRING>\\[ntr\\\"]|. {
	const char* yytextcpy = yytext;
	while(*yytextcpy != '\0') {
		stringPushBack(&g_String, *yytextcpy++);
	}
}

<STRING>\n {
	fprintf(stderr, "ERROR: Newline character found in string at line %d\n", yylineno);
	return TT_Unknown;
}

<STRING><<EOF>> { 
	fprintf(stderr, "ERROR: End-of-File reached inside string constant\n"); 
	return TT_Unknown; 
}

. {
	/* Everything else is an unknown token. Stop the lexer */
	fprintf(stderr, "ERROR: Invalid token '%s' found at line %d.\n", yytext, yylineno);
	return TT_Unknown;
}

%%

alpha_token_t* allocToken()
{
	alpha_token_t* tok = (alpha_token_t*)malloc(sizeof(alpha_token_t));
	if(tok) {
		memset(tok, 0, sizeof(alpha_token_t));
	}
	return tok;
}

void freeToken(alpha_token_t* tok)
{
	if(tok->m_Type == TT_String) {
		free(tok->m_String);
	}
	free(tok->m_Text);
	free(tok);
}

const char* keywordToString(Keyword kw)
{
	switch(kw) {
	case K_If:
		return "IF";
	case K_Else:
		return "ELSE";
	case K_While:
		return "WHILE";
	case K_For:
		return "FOR";
	case K_Function:
		return "FUNCTION";
	case K_Return:
		return "RETURN";
	case K_Break:
		return "BREAK";
	case K_Continue:
		return "CONTINUE";
	case K_And:
		return "AND";
	case K_Not:
		return "NOT";
	case K_Or:
		return "OR";
	case K_Local:
		return "LOCAL";
	case K_True:
		return "TRUE";
	case K_False:
		return "FALSE";
	case K_Nil:
		return "NIL";
	}

	return "UNKNOWN";
}

void printToken(const alpha_token_t* tok)
{
	fprintf(stdout, "%d:\t#%d\t\"%s\"\t", tok->m_LineNo, tok->m_TokenID, tok->m_Text);

	if(tok->m_Type == TT_Keyword) {
		fprintf(stdout, "KEYWORD %s", keywordToString(tok->m_Keyword));
	} else if(tok->m_Type == TT_Operator) {
		fprintf(stdout, "OPERATOR %s", tok->m_Text);
	} else if(tok->m_Type == TT_Integer) {
		fprintf(stdout, "INTCONST %d", tok->m_IntConst);
	} else if(tok->m_Type == TT_Real) {
		fprintf(stdout, "REALCONST %.15f", tok->m_RealConst);
	} else if(tok->m_Type == TT_String) {
		fprintf(stdout, "STRING %s", tok->m_String);
	} else if(tok->m_Type == TT_Delimiter) {
		fprintf(stdout, "DELIMITER %s", tok->m_Text);
	} else if(tok->m_Type == TT_Identifier) {
		fprintf(stdout, "IDENT \"%s\"", tok->m_Text);
	} else if(tok->m_Type == TT_Comment) {
		fprintf(stdout, "COMMENT");
	} else {
		fprintf(stdout, "UNKNOWN");
	}

	fprintf(stdout, "\n");
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		fprintf(stderr, "Missing input file.\n");
		return 1;
	}

	// Open input file
	if(!(yyin = fopen(argv[1], "r"))) {
		fprintf(stderr, "Cannot read file: %s\n", argv[1]);
		return 1;
	}

	// Redirect stdout to the second (optional) file.
	if(argc == 3) {
		if(!freopen(argv[2], "w", stdout)) {
			fprintf(stderr, "Failed to redirect stdout to %s.\n", argv[2]);
			return 1;
		}
	}
	
	// Reset global token counter.
	g_NextTokenID = 1;

	// Initialize global string.
	stringInit(&g_String);

	// Parse each token in turn until there are no more tokens or an error occurs (see stderr for the error)
	while(1) {
		// Allocate new token
		alpha_token_t* tok = allocToken();
		
		// Parse token
		int tokType = alpha_lex(tok);
		if(tokType == TT_Unknown) {
			freeToken(tok);
			break;
		}
		
		printToken(tok);
		
		// Cleanup
		freeToken(tok);
	}

	// Free global string.
	stringDestroy(&g_String);
	
	// Close stdout
	if(argc == 3) {
		fclose(stdout);
	}
	
	return 0;
}
