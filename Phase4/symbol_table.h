#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct LinkedList;
struct LinkedListNode; // PHASE3
struct Scope; // PHASE3
struct SymbolTable;

enum SymbolType
{
	SymType_Function,
	SymType_FunctionArgument,
	SymType_Variable,
	SymType_TempVariable // PHASE3
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
	unsigned int m_Offset; // PHASE3
	Scope* m_ParentScope; // PHASE3
};

struct Scope
{
	unsigned int m_ID; // PHASE3: Used to access symbol table's variable/argument counters
	SymbolTable* m_SymbolTable; // PHASE3
	Scope* m_Parent;
	LinkedList* m_ChildrenList;
	LinkedList* m_SymbolList;
	LinkedList* m_TempVariables; // PHASE3
	LinkedListNode* m_NextTempVariableNode; // PHASE3
	unsigned int m_NextTempVariableID; // PHASE3
	unsigned int m_Depth;
	unsigned int m_LineNumber;
	ScopeType m_Type;
};

struct SymbolTable
{
	Scope* m_GlobalScope;
	Scope* m_CurScope;
	unsigned int* m_ScopeNumVariables; // PHASE3
	unsigned int* m_ScopeNumFunctionArguments; // PHASE3
	unsigned int m_NumScopes; // PHASE3
};

SymbolTable* stCreate();
void stDestroy(SymbolTable* table);

Symbol* stFindSymbol(Scope* scope, const char* name);
Symbol* stInsertSymbol_Function(Scope* scope, const char* name, unsigned int lineno);
Symbol* stInsertSymbol_FunctionArgument(Scope* scope, const char* name, unsigned int lineno);
Symbol* stInsertSymbol_Variable(Scope* scope, const char* name, unsigned int lineno);
Symbol* stInsertSymbol_TempVariable(Scope* scope); // PHASE3
void stResetTempVariableCounter(Scope* scope); // PHASE3

unsigned int stGetNumScopeArguments(Scope* scope); // PHASE3
unsigned int stGetNumScopeLocals(Scope* scope); // PHASE3

void stPushScope(SymbolTable* st, ScopeType type, unsigned int lineno);
void stPopScope(SymbolTable* st);

const char* stSymbolTypeToString(SymbolType type);

#endif
