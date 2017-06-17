#include "symbol_table.h"
#include "linked_list.h"
#include <malloc.h> // malloc(), free()
#include <stdio.h> // fprintf()
#include <string.h> // strcmp()
#include <assert.h> // assert()

Symbol* allocSymbol(SymbolType type, const char* name, unsigned int lineno)
{
	Symbol* s = (Symbol*)malloc(sizeof(Symbol));
	s->m_Type = type;
	s->m_Identifier = strdup(name);
	s->m_LineNumber = lineno;
	return s;
}

void freeSymbol(Symbol* symbol)
{
	free(symbol->m_Identifier);
	free(symbol);
}

Scope* allocScope(Scope* parent, unsigned int depth, ScopeType type)
{
	Scope* s = (Scope*)malloc(sizeof(Scope));
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

	// Free symbols...
	LinkedListNode* symbolNode = scope->m_SymbolList->m_Head;
	while (symbolNode) {
		Symbol* symbol = (Symbol*)symbolNode->m_Item;
		freeSymbol(symbol);
		symbolNode = symbolNode->m_Next;
	}

	free(scope);
}

SymbolTable* stCreate()
{
	SymbolTable* st = (SymbolTable*)malloc(sizeof(SymbolTable));
	st->m_GlobalScope = allocScope(0, 0, Scope_Global);
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
	Symbol* symbol = allocSymbol(SymType_Function, name, lineno);
	llPushBack(scope->m_SymbolList, symbol);

	return symbol;
}

Symbol* stInsertSymbol_FunctionArgument(Scope* scope, const char* name, unsigned int lineno)
{
	Symbol* symbol = allocSymbol(SymType_FunctionArgument, name, lineno);
	llPushBack(scope->m_SymbolList, symbol);

	return symbol;
}

Symbol* stInsertSymbol_Variable(Scope* scope, const char* name, unsigned int lineno)
{
	Symbol* symbol = allocSymbol(SymType_Variable, name, lineno);
	llPushBack(scope->m_SymbolList, symbol);

	return symbol;
}

void stPushScope(SymbolTable* st, ScopeType type, unsigned int lineno)
{
	Scope* childScope = allocScope(st->m_CurScope, st->m_CurScope->m_Depth + 1, type);
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
	}

	return "UNKNOWN";
}