#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct LinkedList;

enum SymbolType
{
	SymType_Function,
	SymType_FunctionArgument,
	SymType_Variable
};

enum ScopeType
{
	Scope_Global,
	Scope_Function,
	Scope_Block
};

struct Symbol
{
	char* m_Identifier;
	SymbolType m_Type;
	unsigned int m_LineNumber;
};

struct Scope
{
	Scope* m_Parent;
	LinkedList* m_ChildrenList;
	LinkedList* m_SymbolList;
	unsigned int m_Depth;
	unsigned int m_LineNumber;
	ScopeType m_Type;
};

struct SymbolTable
{
	Scope* m_GlobalScope;
	Scope* m_CurScope;
};

SymbolTable* stCreate();
void stDestroy(SymbolTable* table);

Symbol* stFindSymbol(Scope* scope, const char* name);
Symbol* stInsertSymbol_Function(Scope* scope, const char* name, unsigned int lineno);
Symbol* stInsertSymbol_FunctionArgument(Scope* scope, const char* name, unsigned int lineno);
Symbol* stInsertSymbol_Variable(Scope* scope, const char* name, unsigned int lineno);

void stPushScope(SymbolTable* st, ScopeType type, unsigned int lineno);
void stPopScope(SymbolTable* st);

const char* stSymbolTypeToString(SymbolType type);

#endif
