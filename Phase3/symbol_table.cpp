#include "symbol_table.h"
#include "linked_list.h"
#include <malloc.h> // malloc(), free()
#include <stdio.h> // fprintf()
#include <string.h> // strcmp()
#include <assert.h> // assert()

Symbol* allocSymbol(Scope* scope, SymbolType type, const char* name, unsigned int lineno)
{
	Symbol* s = (Symbol*)malloc(sizeof(Symbol));
	s->m_Type = type;
	s->m_Identifier = strdup(name);
	s->m_LineNumber = lineno;

	// PHASE3
	s->m_ParentScope = scope;
	if (type == SymType_Variable || type == SymType_TempVariable) {
		s->m_Offset = scope->m_SymbolTable->m_ScopeNumVariables[scope->m_ID]++;
	} else if (type == SymType_FunctionArgument) {
		s->m_Offset = scope->m_SymbolTable->m_ScopeNumFunctionArguments[scope->m_ID]++;
	} else {
		s->m_Offset = ~0u;
	}

	return s;
}

void freeSymbol(Symbol* symbol)
{
	free(symbol->m_Identifier);
	free(symbol);
}

Scope* allocScope(SymbolTable* st, Scope* parent, unsigned int depth, ScopeType type)
{
	st->m_NumScopes++;
	st->m_ScopeNumVariables = (unsigned int*)realloc(st->m_ScopeNumVariables, sizeof(unsigned int) * st->m_NumScopes);
	st->m_ScopeNumFunctionArguments = (unsigned int*)realloc(st->m_ScopeNumFunctionArguments, sizeof(unsigned int) * st->m_NumScopes);

	st->m_ScopeNumVariables[st->m_NumScopes - 1] = 0;
	st->m_ScopeNumFunctionArguments[st->m_NumScopes - 1] = 0;

	Scope* s = (Scope*)malloc(sizeof(Scope));
	s->m_SymbolTable = st;
	s->m_SymbolList = 0;
	s->m_ChildrenList = llCreate();
	s->m_SymbolList = llCreate();
	s->m_Parent = parent;
	s->m_Depth = depth;
	s->m_LineNumber = 0;
	s->m_Type = type;

	if (parent) {
		llPushBack(parent->m_ChildrenList, s);
	}

	// PHASE3
	s->m_TempVariables = llCreate();
	s->m_NextTempVariableNode = 0;
	s->m_NextTempVariableID = 0;

	if (type == Scope_Block) {
		// In case this is a block scope inherit the parent's id.
		s->m_ID = parent->m_ID;
	} else {
		s->m_ID = st->m_NumScopes - 1;
	}

	return s;
}

void freeScope_r(Scope* scope)
{
	// Free children scopes...
	LinkedListNode* childNode = scope->m_ChildrenList->m_Head;
	while (childNode) {
		Scope* child = (Scope*)childNode->m_Item;
		freeScope_r(child);
		childNode = childNode->m_Next;
	}
	llDestroy(scope->m_ChildrenList);

	// Free symbols...
	LinkedListNode* symbolNode = scope->m_SymbolList->m_Head;
	while (symbolNode) {
		Symbol* symbol = (Symbol*)symbolNode->m_Item;
		freeSymbol(symbol);
		symbolNode = symbolNode->m_Next;
	}
	llDestroy(scope->m_SymbolList);

	// Free temp variable linked list.
	// NOTE: Pointers are shared with the symbol list.
	llDestroy(scope->m_TempVariables);

	free(scope);
}

SymbolTable* stCreate()
{
	SymbolTable* st = (SymbolTable*)malloc(sizeof(SymbolTable));
	memset(st, 0, sizeof(SymbolTable));
	st->m_GlobalScope = allocScope(st, 0, 0, Scope_Global);
	st->m_CurScope = st->m_GlobalScope;

	return st;
}

void stDestroy(SymbolTable* st)
{
	// Destroy scopes...
	freeScope_r(st->m_GlobalScope);
	free(st);
}

Symbol* stFindSymbol(Scope* scope, const char* name)
{
	// TODO Hash table?
	LinkedListNode* symbolNode = scope->m_SymbolList->m_Head;
	while (symbolNode) {
		Symbol* symbol = (Symbol*)symbolNode->m_Item;
		if (!strcmp(symbol->m_Identifier, name)) {
			return symbol;
		}

		symbolNode = symbolNode->m_Next;
	}

	return 0;
}

Symbol* stInsertSymbol_Function(Scope* scope, const char* name, unsigned int lineno)
{
	Symbol* symbol = allocSymbol(scope, SymType_Function, name, lineno);
	llPushBack(scope->m_SymbolList, symbol);

	return symbol;
}

Symbol* stInsertSymbol_FunctionArgument(Scope* scope, const char* name, unsigned int lineno)
{
	Symbol* symbol = allocSymbol(scope, SymType_FunctionArgument, name, lineno);
	llPushBack(scope->m_SymbolList, symbol);

	return symbol;
}

Symbol* stInsertSymbol_Variable(Scope* scope, const char* name, unsigned int lineno)
{
	Symbol* symbol = allocSymbol(scope, SymType_Variable, name, lineno);
	llPushBack(scope->m_SymbolList, symbol);

	return symbol;
}

// PHASE3
Symbol* stInsertSymbol_TempVariable(Scope* scope)
{
	LinkedListNode* tmpVarNode = scope->m_NextTempVariableNode;
	if (tmpVarNode == 0) {
		char tmpName[256];
		sprintf(tmpName, "_r%d", scope->m_NextTempVariableID++);
		Symbol* symbol = allocSymbol(scope, SymType_TempVariable, tmpName, ~0u);
		llPushBack(scope->m_SymbolList, symbol);
		llPushBack(scope->m_TempVariables, symbol);

		tmpVarNode = scope->m_TempVariables->m_Tail;
		scope->m_NextTempVariableNode = 0; // same as Tail->Next
	} else {
		scope->m_NextTempVariableNode = scope->m_NextTempVariableNode->m_Next;
	}

	return (Symbol*)tmpVarNode->m_Item;
}

// PHASE3
void stResetTempVariableCounter(Scope* scope)
{
	scope->m_NextTempVariableNode = scope->m_TempVariables->m_Head;
}

// PHASE3
unsigned int stGetNumScopeArguments(Scope* scope)
{
	return scope->m_SymbolTable->m_ScopeNumFunctionArguments[scope->m_ID];
}

// PHASE3
unsigned int stGetNumScopeLocals(Scope* scope)
{
	return scope->m_SymbolTable->m_ScopeNumVariables[scope->m_ID];
}

void stPushScope(SymbolTable* st, ScopeType type, unsigned int lineno)
{
	Scope* childScope = allocScope(st, st->m_CurScope, st->m_CurScope->m_Depth + 1, type);
	childScope->m_LineNumber = lineno;
	st->m_CurScope = childScope;
}

void stPopScope(SymbolTable* st)
{
	assert(st->m_CurScope->m_Parent);
	st->m_CurScope = st->m_CurScope->m_Parent;
}

const char* stSymbolTypeToString(SymbolType type)
{
	if (type == SymType_Function) {
		return "FUNCTION";
	} else if (type == SymType_FunctionArgument) {
		return "FUNC_ARG";
	} else if (type == SymType_Variable) {
		return "VARIABLE";
	} else if (type == SymType_TempVariable) {
		return "TEMP"; // PHASE3
	}

	return "UNKNOWN";
}