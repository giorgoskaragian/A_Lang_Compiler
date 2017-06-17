#include "ast.h"
#include "symbol_table.h"
#include "linked_list.h"
#include <malloc.h> // malloc(), free()
#include <string.h> // strdup()
#include <assert.h> // assert()
#include <stdio.h> // fprintf()
#include <stdarg.h> // va_list, va_start, va_arg, va_end

#define LOOP_CONDITION_NEW_SCOPE 0

//////////////////////////////////////////////////////////////////////////
// Internal
//
struct State
{
	bool m_IsInsideFunctionBody;
	bool m_IsInsideLoopBody;
	bool m_IsFunctionCall;
};

struct StateStack
{
	State* m_States;
	unsigned int m_StateCapacity;
	unsigned int m_NumStates;
};

void initStateStack(StateStack* ss);
void destroyStateStack(StateStack* ss);
State* getState(StateStack* ss);
State* pushState(StateStack* ss);
void popState(StateStack* ss);

void printError(const char* msg, ...);

bool checkStatement(StateStack* stateStack, ASTNodeStatement* statement, SymbolTable* symTable);
bool checkExpression(StateStack* stateStack, ASTNodeExpression* expression, SymbolTable* symTable);
bool checkFunctionDefinition(StateStack* stateStack, ASTNodeFuncDef* funcDef, SymbolTable* symTable);
bool checkLValue(StateStack* stateStack, ASTNodeLValue* lvalue, SymbolTable* symTable);
bool checkCall(StateStack* stateStack, ASTNodeCall* call, SymbolTable* symTable);
bool checkObjectDefinition(StateStack* stateStack, ASTNodeExpressionObjDef* objDef, SymbolTable* symTable);
bool checkIndexedElement(StateStack* stateStack, ASTNodeIndexedElement* indexedElement, SymbolTable* symTable);

void astDestroyStatement(ASTNodeStatement* statement);
void astDestroyExpression(ASTNodeExpression* expression);
void astDestroyFunctionDefinition(ASTNodeFuncDef* funcDef);
void astDestroyLValue(ASTNodeLValue* lvalue);
void astDestroyObjectDefinition(ASTNodeExpressionObjDef* objDef);
void astDestroyCall(ASTNodeCall* call);
void astDestroyIdentifier(ASTNodeIdentifier* id);
void astDestroyIndexedElement(ASTNodeIndexedElement* indexedElem);

bool isLibraryFunction(Symbol* symbol)
{
	if (!symbol) {
		return false;
	}

	return symbol && symbol->m_Type == SymType_Function && symbol->m_LineNumber == 0;
}

bool checkStatement(StateStack* stateStack, ASTNodeStatement* statement, SymbolTable* symTable)
{
	const StatementType type = statement->m_StatementType;
	bool noErrors = true;

	if (type == ST_Expression) {
		ASTNodeStatementExpression* node = (ASTNodeStatementExpression*)statement;
		assert(node->m_Expr->m_Type == AST_Expression);
		if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Expr, symTable)) {
			noErrors = false;
		}
	} else if (type == ST_If) {
		ASTNodeStatementIf* node = (ASTNodeStatementIf*)statement;
		assert(node->m_Cond->m_Type == AST_Expression);
		assert(node->m_Then->m_Type == AST_Statement);
		if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Cond, symTable)) {
			noErrors = false;
		}
		if (!checkStatement(stateStack, (ASTNodeStatement*)node->m_Then, symTable)) {
			noErrors = false;
		}

		if (node->m_Else) {
			assert(node->m_Else->m_Type == AST_Statement);
			if (!checkStatement(stateStack, (ASTNodeStatement*)node->m_Else, symTable)) {
				noErrors = false;
			}
		}
	} else if (type == ST_While) {
		ASTNodeStatementWhile* node = (ASTNodeStatementWhile*)statement;
		assert(node->m_Cond->m_Type == AST_Expression);
		assert(node->m_Body->m_Type == AST_Statement);

		State* state = pushState(stateStack);
#if LOOP_CONDITION_NEW_SCOPE
		stPushScope(symTable, Scope_Block, node->m_LineNumber);
#endif

		if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Cond, symTable)) {
			noErrors = false;
		}
		
		state->m_IsInsideLoopBody = true;
		if (!checkStatement(stateStack, (ASTNodeStatement*)node->m_Body, symTable)) {
			noErrors = false;
		}

#if LOOP_CONDITION_NEW_SCOPE
		stPopScope(symTable);
#endif
		popState(stateStack);
	} else if (type == ST_For) {
		ASTNodeStatementFor* node = (ASTNodeStatementFor*)statement;
		
		State* state = pushState(stateStack);

#if LOOP_CONDITION_NEW_SCOPE
		stPushScope(symTable, Scope_Block, node->m_LineNumber);
#endif

		LinkedListNode* initNode = node->m_Init->m_Head;
		while (initNode) {
			ASTNode* child = (ASTNode*)initNode->m_Item;
			assert(child->m_Type == AST_Expression);
			
			if (!checkExpression(stateStack, (ASTNodeExpression*)child, symTable)) {
				noErrors = false;
			}

			initNode = initNode->m_Next;
		}

		assert(node->m_Cond->m_Type == AST_Expression);
		if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Cond, symTable)) {
			noErrors = false;
		}

		LinkedListNode* iterNode = node->m_Iter->m_Head;
		while (iterNode) {
			ASTNode* child = (ASTNode*)iterNode->m_Item;
			assert(child->m_Type == AST_Expression);

			if (!checkExpression(stateStack, (ASTNodeExpression*)child, symTable)) {
				noErrors = false;
			}

			iterNode = iterNode->m_Next;
		}

		state->m_IsInsideLoopBody = true;
		
		assert(node->m_Body->m_Type == AST_Statement);
		if (!checkStatement(stateStack, (ASTNodeStatement*)node->m_Body, symTable)) {
			noErrors = false;
		}

#if LOOP_CONDITION_NEW_SCOPE
		stPopScope(symTable);
#endif
		popState(stateStack);
	} else if (type == ST_Return) {
		ASTNodeStatementReturn* node = (ASTNodeStatementReturn*)statement;
		State* state = getState(stateStack);
		if (!state->m_IsInsideFunctionBody) {
			printError("SYNTAX ERROR (%d): return statement outside function body.", node->m_LineNumber);
			noErrors = false;
		} else {
			if (node->m_Expr) {
				assert(node->m_Expr->m_Type == AST_Expression);
				if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Expr, symTable)) {
					noErrors = false;
				}
			}
		}
	} else if (type == ST_Block) {
		ASTNodeStatementBlock* node = (ASTNodeStatementBlock*)statement;

		State* state = pushState(stateStack);
		stPushScope(symTable, Scope_Block, node->m_LineNumber);
		{
			LinkedListNode* llNode = node->m_Body->m_Head;
			while (llNode) {
				ASTNode* astNode = (ASTNode*)llNode->m_Item;
				assert(astNode->m_Type == AST_Statement);

				ASTNodeStatement* statementNode = (ASTNodeStatement*)astNode;
				if (!checkStatement(stateStack, statementNode, symTable)) {
					noErrors = false;
				}

				// Move on to the next node...
				llNode = llNode->m_Next;
			}
		}
		stPopScope(symTable);
		popState(stateStack);
	} else if (type == ST_FuncDef) {
		ASTNodeStatementFuncDef* node = (ASTNodeStatementFuncDef*)statement;
		assert(node->m_FuncDef->m_Type == AST_FunctionDefinition);

		if (!checkFunctionDefinition(stateStack, (ASTNodeFuncDef*)node->m_FuncDef, symTable)) {
			noErrors = false;
		}
	} else if (type == ST_Break) {
		State* state = getState(stateStack);
		if (!state->m_IsInsideLoopBody) {
			printError("SYNTAX ERROR (%d): break statement outside loop body.", statement->m_LineNumber);
			noErrors = false;
		}
	} else if (type == ST_Continue) {
		State* state = getState(stateStack);
		if (!state->m_IsInsideLoopBody) {
			printError("SYNTAX ERROR (%d): continue statement outside loop body.", statement->m_LineNumber);
			noErrors = false;
		}
	} else if (type == ST_Empty) {
		// Do nothing.
	} else {
		assert(false); // Unknown statement type.
	}

	return noErrors;
}

bool checkExpression(StateStack* stateStack, ASTNodeExpression* expression, SymbolTable* symTable)
{
	const ExpressionType type = expression->m_ExpressionType;
	bool noErrors = true;

	if (type == ET_Assignment) {
		ASTNodeExpressionAssignment* node = (ASTNodeExpressionAssignment*)expression;
		assert(node->m_LValue->m_Type == AST_LValue);
		assert(node->m_Expression->m_Type == AST_Expression);

		ASTNodeExpression* expr = (ASTNodeExpression*)node->m_Expression;
		if (!checkExpression(stateStack, expr, symTable)) {
			noErrors = false;
		}

		ASTNodeLValue* lvalue = (ASTNodeLValue*)node->m_LValue;
		if (!checkLValue(stateStack, lvalue, symTable)) {
			noErrors = false;
		} else {
			const Symbol* symbol = lvalue->m_Symbol;
			if (symbol && symbol->m_Type == SymType_Function) {
				printError("SYNTAX ERROR (%d): Cannot assign a value to a function.", expression->m_LineNumber);
				noErrors = false;
			}
		}
	} else if(type == ET_BinaryOp || type == ET_LogicOp) {
		ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)expression;
		assert(node->m_Left->m_Type == AST_Expression);
		assert(node->m_Right->m_Type == AST_Expression);
		if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Left, symTable)) {
			noErrors = false;
		}
		if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Right, symTable)) {
			noErrors = false;
		}
	} else if (type == ET_UnaryOp) {
		ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)expression;
		assert(node->m_Right == 0);
		if (node->m_Left->m_Type == AST_Expression) {
			// Unary minus, not
			if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Left, symTable)) {
				noErrors = false;
			}
		} else if (node->m_Left->m_Type == AST_LValue) {
			// Pre/Post inc/dec or lvalue
			ASTNodeLValue* lvalue = (ASTNodeLValue*)node->m_Left;
			if (!checkLValue(stateStack, lvalue, symTable)) {
				noErrors = false;
			} else {
				const Symbol* symbol = lvalue->m_Symbol;
				if (symbol && symbol->m_Type == SymType_Function) {
					printError("SYNTAX ERROR (%d): Function cannot be used as an lvalue.", expression->m_LineNumber);
					noErrors = false;
				}
			}
		}
	} else if (type == ET_Nested) {
		ASTNodeExpressionNested* node = (ASTNodeExpressionNested*)expression;
		assert(node->m_Expr->m_Type == AST_Expression);
		if (!checkExpression(stateStack, (ASTNodeExpression*)node->m_Expr, symTable)) {
			noErrors = false;
		}
	} else if (type == ET_Const) {
		// Nothing to do here at the moment.
//		ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)expression;
	} else if (type == ET_ObjectDefinition) {
		ASTNodeExpressionObjDef* node = (ASTNodeExpressionObjDef*)expression;
		if (!checkObjectDefinition(stateStack, node, symTable)) {
			noErrors = false;
		}
	} else if (type == ET_FunctionDefinition) {
		ASTNodeExpressionFuncDef* node = (ASTNodeExpressionFuncDef*)expression;
		assert(node->m_FuncDef->m_Type == AST_FunctionDefinition);
		if (!checkFunctionDefinition(stateStack, (ASTNodeFuncDef*)node->m_FuncDef, symTable)) {
			noErrors = false;
		}
	} else if (type == ET_LValue) {
		ASTNodeExpressionLValue* node = (ASTNodeExpressionLValue*)expression;
		assert(node->m_LValue->m_Type == AST_LValue);
		if (!checkLValue(stateStack, (ASTNodeLValue*)node->m_LValue, symTable)) {
			noErrors = false;
		}
	} else if (type == ET_Call) {
		ASTNodeExpressionCall* node = (ASTNodeExpressionCall*)expression;
		assert(node->m_Call->m_Type == AST_Call);
		if (!checkCall(stateStack, (ASTNodeCall*)node->m_Call, symTable)) {
			noErrors = false;
		}
	} else {
		assert(false); // Unknown expression type.
	}

	return noErrors;
}

bool checkFunctionDefinition(StateStack* stateStack, ASTNodeFuncDef* funcDef, SymbolTable* symTable)
{
	bool noErrors = true;

	// Check if a symbol with the same name as the function already exists at this scope.
	char* funcName = 0;
	if (funcDef->m_Identifier) {
		funcName = funcDef->m_Identifier->m_Name;
	} else {
		char name[256];
		sprintf(name, "$AnonymousFunc_%p", funcDef);
		funcName = strdup(name);
	}

	Symbol* funcSymbol = stFindSymbol(symTable->m_CurScope, funcName);
	if (funcSymbol) {
		if (isLibraryFunction(funcSymbol)) {
			printError("SYNTAX ERROR (%d): Function \"%s\" shadows library function", funcDef->m_LineNumber, funcName);
		} else if (funcSymbol->m_Type == SymType_Function) {
			printError("SYNTAX ERROR (%d): Function \"%s\" already defined at line %d", funcDef->m_LineNumber, funcName, funcSymbol->m_LineNumber);
		} else {
			// existing symbol is a variable or function argument.
			printError("SYNTAX ERROR (%d): Cannot redefine variable \"%s\" as function; previous definition at line %d", funcDef->m_LineNumber, funcName, funcSymbol->m_LineNumber);
		}

		noErrors = false;
	} else {
		if (symTable->m_CurScope != symTable->m_GlobalScope) {
			// Check if there's a collision with a library function.
			funcSymbol = stFindSymbol(symTable->m_GlobalScope, funcName);
			if (isLibraryFunction(funcSymbol)) {
				printError("SYNTAX ERROR (%d): Function \"%s\" shadows library function", funcDef->m_LineNumber, funcName);
				noErrors = false;
			}
		}
	}

	if (noErrors) {
		// Insert new function into current scope.
		funcSymbol = stInsertSymbol_Function(symTable->m_CurScope, funcName, funcDef->m_LineNumber);
	} else {
		// If there's an error with the function definition, skip the rest.
		return false;
	}

	stPushScope(symTable, Scope_Function, funcDef->m_LineNumber);
	State* state = pushState(stateStack);
	{
		// Arguments
		LinkedListNode* argNode = funcDef->m_ArgList->m_Head;
		while (argNode) {
			ASTNode* child = (ASTNode*)argNode->m_Item;
			assert(child->m_Type == AST_Identifier);

			ASTNodeIdentifier* idNode = (ASTNodeIdentifier*)child;

			// Check if identifier already defined (e.g. multiple args with the same name).
			Symbol* argSymbol = stFindSymbol(symTable->m_CurScope, idNode->m_Name);
			if (argSymbol) {
				// TODO: Line number should be the lineno of the identifier
				printError("SYNTAX ERROR (%d): Multiple definitions of function argument \"%s\"", idNode->m_LineNumber, idNode->m_Name);
				noErrors = false;
			} else {
				// Check if there's a collision with a library function.
				argSymbol = stFindSymbol(symTable->m_GlobalScope, idNode->m_Name);
				if (isLibraryFunction(argSymbol)) {
					// TODO: Line number should be the lineno of the identifier
					printError("SYNTAX ERROR (%d): Function argument \"%s\" shadows library function", idNode->m_LineNumber, idNode->m_Name);
					noErrors = false;
				} else {
					// Insert new function argument to symbol table.
					// TODO: Line number should be the lineno of the identifier
					argSymbol = stInsertSymbol_FunctionArgument(symTable->m_CurScope, idNode->m_Name, idNode->m_LineNumber);
				}
			}

			argNode = argNode->m_Next;
		}

		// Func body.
		state->m_IsInsideFunctionBody = true;

		LinkedListNode* bodyNode = funcDef->m_Body->m_Head;
		while (bodyNode) {
			ASTNode* child = (ASTNode*)bodyNode->m_Item;
			assert(child->m_Type == AST_Statement);

			if (!checkStatement(stateStack, (ASTNodeStatement*)child, symTable)) {
				noErrors = false;
			}

			bodyNode = bodyNode->m_Next;
		}
	}
	popState(stateStack);
	stPopScope(symTable);

	// Free temp name
	if (!funcDef->m_Identifier) {
		free(funcName);
	}

	return noErrors;
}

bool checkLValue(StateStack* stateStack, ASTNodeLValue* lvalue, SymbolTable* symTable)
{
	const LValueType type = lvalue->m_LValueType;
	bool noErrors = true;

	State* state = getState(stateStack);

	if (type == LValue_Identifier) {
		ASTNodeLValueIdentifier* node = (ASTNodeLValueIdentifier*)lvalue;
		const IdentifierScopeType scopeType = node->m_ScopeType;
		if (scopeType == IST_Unknown) {
			// lvalue: IDENTIFIER
			Symbol* idSymbol = stFindSymbol(symTable->m_CurScope, node->m_Identifier->m_Name);
			Scope* idScope = idSymbol ? symTable->m_CurScope : 0;
			unsigned int numParentScopeFunctions = 0;
			if (!idSymbol) {
				numParentScopeFunctions += symTable->m_CurScope->m_Type == Scope_Function ? 1 : 0;

				Scope* curScope = symTable->m_CurScope->m_Parent;
				while (curScope) {
					idSymbol = stFindSymbol(curScope, node->m_Identifier->m_Name);
					if (idSymbol) {
						// Symbol found in a parent scope. Stop looking.
						idScope = curScope;
						break;
					}

					numParentScopeFunctions += curScope->m_Type == Scope_Function ? 1 : 0;
					curScope = curScope->m_Parent;
				}
			}
			
			if (!idSymbol) {
				// Symbol not found. Create a new variable in the current scope.
				idSymbol = stInsertSymbol_Variable(symTable->m_CurScope, node->m_Identifier->m_Name, node->m_Identifier->m_LineNumber);
			} else if (idSymbol) {
				assert(idScope);

				// Symbol found in a parent scope. Check if we have access to it.
				// If this is a variable or a function argument, check to see if there's a function block 
				// between the current scope and the symbol's scope.
				// Otherwise, it's a function, and all functions are accessible.
				if (idScope->m_Type != Scope_Global && (idSymbol->m_Type == SymType_Variable || idSymbol->m_Type == SymType_FunctionArgument)) {
					if (numParentScopeFunctions != 0) {
						// There's a function block between the declaration and the usage. It's an error.
						printError("SYNTAX ERROR (%d): Cannot access variable \"%s\" declared at line %d", node->m_LineNumber, node->m_Identifier->m_Name, idSymbol->m_LineNumber);
						noErrors = false;
					}
				}
			}

			if (noErrors) {
				lvalue->m_Symbol = idSymbol;
			} else {
				lvalue->m_Symbol = 0;
			}
		} else if (scopeType == IST_Local) {
			// lvalue: local IDENTIFIER
			Symbol* idSymbol = stFindSymbol(symTable->m_CurScope, node->m_Identifier->m_Name);
			if (!idSymbol) {
				// Symbol does not exist. Check if there's a collision with a library function.
				if (symTable->m_CurScope != symTable->m_GlobalScope) {
					idSymbol = stFindSymbol(symTable->m_GlobalScope, node->m_Identifier->m_Name);
					if (isLibraryFunction(idSymbol)) {
						printError("SYNTAX ERROR (%d): Identifier shadows library function \"%s\"", node->m_Identifier->m_LineNumber, node->m_Identifier->m_Name);
						noErrors = false;
					} else {
						// Reset symbol to force creating a new one in the current scope (below).
						idSymbol = 0;
					}
				}
			}

			if (noErrors) {
				// There are no errors and there's no symbol. Insert a new variable to the current scope.
				if (!idSymbol) {
					idSymbol = stInsertSymbol_Variable(symTable->m_CurScope, node->m_Identifier->m_Name, node->m_Identifier->m_LineNumber);
				}

				lvalue->m_Symbol = idSymbol;
			} else {
				lvalue->m_Symbol = 0;
			}
		} else if (scopeType == IST_Global) {
			// lvalue: ::IDENTIFIER
			Symbol* idSymbol = stFindSymbol(symTable->m_GlobalScope, node->m_Identifier->m_Name);
			if (!idSymbol) {
				// Symbol doesn't exist in the global scope. 
				printError("SYNTAX ERROR (%d): Undeclared global identifier \"%s\"", node->m_Identifier->m_LineNumber, node->m_Identifier->m_Name);
			}

			lvalue->m_Symbol = idSymbol;
		}
	} else if (type == LValue_ObjectMember) {
		ASTNodeLValueObjectMember* node = (ASTNodeLValueObjectMember*)lvalue;

		if (node->m_Object->m_Type == AST_LValue) {
			ASTNodeLValue* obj = (ASTNodeLValue*)node->m_Object;
			if (!checkLValue(stateStack, obj, symTable)) {
				noErrors = false;
			} else {
				const Symbol* symbol = obj->m_Symbol;
				if (symbol && symbol->m_Type == SymType_Function) {
					printError("SYNTAX ERROR (%d): Identifier \"%s\" is not an object; declared as function at line %d", node->m_LineNumber, symbol->m_Identifier, symbol->m_LineNumber);
					noErrors = false;
				}
			}
		} else if (node->m_Object->m_Type == AST_Call) {
			ASTNodeCall* call = (ASTNodeCall*)node->m_Object;
			if (!checkCall(stateStack, call, symTable)) {
				noErrors = false;
			}
		}
	} else if (type == LValue_ArrayElement) {
		ASTNodeLValueArrayElement* node = (ASTNodeLValueArrayElement*)lvalue;
		assert(node->m_Array->m_Type == AST_LValue);
		assert(node->m_Index->m_Type == AST_Expression);
		ASTNodeExpression* expr = (ASTNodeExpression*)node->m_Index;

		if (!checkExpression(stateStack, expr, symTable)) {
			noErrors = false;
		}

		ASTNodeLValue* arr = (ASTNodeLValue*)node->m_Array;
		if (!checkLValue(stateStack, arr, symTable)) {
			noErrors = false;
		} else {
			const Symbol* symbol = arr->m_Symbol;
			if (symbol && symbol->m_Type == SymType_Function) {
				printError("SYNTAX ERROR (%d): Identifier \"%s\" is not an array; declared as function at line %d", node->m_LineNumber, symbol->m_Identifier, symbol->m_LineNumber);
				noErrors = false;
			}
		}
	} else {
		// Unknown lvalue type.
		assert(false);
	}

	return noErrors;
}

bool checkCall(StateStack* stateStack, ASTNodeCall* call, SymbolTable* symTable)
{
	const CallType type = call->m_CallType;
	bool noErrors = true;

	State* state = pushState(stateStack);
	state->m_IsFunctionCall = true;

	if (type == Call_ReturnValue) {
		assert(call->m_Func->m_Type == AST_Call);
		if (!checkCall(stateStack, (ASTNodeCall*)call->m_Func, symTable)) {
			noErrors = false;
		}
	} else if (type == Call_Normal) {
		assert(call->m_Func->m_Type == AST_LValue);
		ASTNodeLValue* funcNode = (ASTNodeLValue*)call->m_Func;
		if (!checkLValue(stateStack, funcNode, symTable)) {
			noErrors = false;
		}
	} else if (type == Call_Method) {
		assert(call->m_Func->m_Type == AST_LValue);
		ASTNodeLValue* funcNode = (ASTNodeLValue*)call->m_Func;
		if (!checkLValue(stateStack, funcNode, symTable)) {
			noErrors = false;
		}
	} else if (type == Call_Lambda) {
		assert(call->m_Func->m_Type == AST_FunctionDefinition);
		if (!checkFunctionDefinition(stateStack, (ASTNodeFuncDef*)call->m_Func, symTable)) {
			noErrors = false;
		}
	} else {
		// Unknown call type.
		assert(false);
	}

	// Check arguments
	LinkedListNode* argNode = call->m_Args->m_Head;
	while (argNode) {
		ASTNode* child = (ASTNode*)argNode->m_Item;
		assert(child->m_Type == AST_Expression);

		if (!checkExpression(stateStack, (ASTNodeExpression*)child, symTable)) {
			noErrors = false;
		}

		argNode = argNode->m_Next;
	}

	popState(stateStack);

	return noErrors;
}

bool checkObjectDefinition(StateStack* stateStack, ASTNodeExpressionObjDef* objDef, SymbolTable* symTable)
{
	const ObjectDefitionType type = objDef->m_ObjDefType;
	bool noErrors = true;

	if (type == ObjDef_ExpressionList) {
		LinkedListNode* llNode = objDef->m_List->m_Head;
		while (llNode) {
			ASTNode* child = (ASTNode*)llNode->m_Item;
			assert(child->m_Type == AST_Expression);

			if (!checkExpression(stateStack, (ASTNodeExpression*)child, symTable)) {
				noErrors = false;
			}

			llNode = llNode->m_Next;
		}
	} else if (type == ObjDef_IndexedElementList) {
		LinkedListNode* llNode = objDef->m_List->m_Head;
		while (llNode) {
			ASTNode* child = (ASTNode*)llNode->m_Item;
			assert(child->m_Type == AST_IndexedElement);

			if (!checkIndexedElement(stateStack, (ASTNodeIndexedElement*)child, symTable)) {
				noErrors = false;
			}

			llNode = llNode->m_Next;
		}
	} else {
		// Unknown object definition type
		assert(false);
	}

	return noErrors;
}

bool checkIndexedElement(StateStack* stateStack, ASTNodeIndexedElement* indexedElement, SymbolTable* symTable)
{
	bool noErrors = true;
	assert(indexedElement->m_Index->m_Type == AST_Expression);
	assert(indexedElement->m_Elem->m_Type == AST_Expression);

	if (!checkExpression(stateStack, (ASTNodeExpression*)indexedElement->m_Index, symTable)) {
		noErrors = false;
	}
	if (!checkExpression(stateStack, (ASTNodeExpression*)indexedElement->m_Elem, symTable)) {
		noErrors = false;
	}

	return noErrors;
}

//////////////////////////////////////////////////////////////////////////
// Program
//
Program* progCreate(LinkedList* statementList)
{
	Program* prog = (Program*)malloc(sizeof(Program));
	prog->m_StatementList = statementList;
	prog->m_SymbolTable = 0;
	return prog;
}

void progDestroy(Program* prog)
{
	// Destroy statement list...
	LinkedListNode* statementNode = prog->m_StatementList->m_Head;
	while (statementNode) {
		ASTNode* astNode = (ASTNode*)statementNode->m_Item;
		assert(astNode->m_Type == AST_Statement);

		astDestroyStatement((ASTNodeStatement*)astNode);
		
		statementNode = statementNode->m_Next;
	}
	llDestroy(prog->m_StatementList);

	if (prog->m_SymbolTable) {
		stDestroy(prog->m_SymbolTable);
	}

	free(prog);
}

bool progBuildSymbolTable(Program* prog)
{
	static const char* libraryFunctions[] = {
		"print",
		"input",
		"objectmemberkeys",
		"objecttotalmembers",
		"objectcopy",
		"totalarguments",
		"argument",
		"typeof",
		"strtonum",
		"sqrt",
		"cos",
		"sin"
	};

	if (prog->m_SymbolTable) {
		return false;
	}

	prog->m_SymbolTable = stCreate();
	
	unsigned int numLibraryFunctions = sizeof(libraryFunctions) / sizeof(libraryFunctions[0]);
	for (unsigned int i = 0; i < numLibraryFunctions; ++i) {
		stInsertSymbol_Function(prog->m_SymbolTable->m_GlobalScope, libraryFunctions[i], 0);
	}

	bool hasErrors = false;
	
	StateStack stateStack;
	initStateStack(&stateStack);

	LinkedListNode* llNode = prog->m_StatementList->m_Head;
	while (llNode) {
		ASTNode* astNode = (ASTNode*)llNode->m_Item;
		assert(astNode->m_Type == AST_Statement);

		ASTNodeStatement* statementNode = (ASTNodeStatement*)astNode;
		bool statementError = checkStatement(&stateStack, statementNode, prog->m_SymbolTable);
		hasErrors = hasErrors || statementError;

		// Move on to the next node...
		llNode = llNode->m_Next;
	}

	destroyStateStack(&stateStack);

	return hasErrors;
}

const char* scopeTypeToString(ScopeType t)
{
	if (t == Scope_Global) {
		return "Global";
	} else if (t == Scope_Function) {
		return "Function";
	} else if (t == Scope_Block) {
		return "Block";
	}

	return "Unknown";
}

void printScopeSymbols_r(Scope* scope)
{
	fprintf(stdout, "%-*sScope (L:%d, D:%d, T:%s) {\n", scope->m_Depth, "", scope->m_LineNumber, scope->m_Depth, scopeTypeToString(scope->m_Type));

	// Print symbols...
	LinkedListNode* symbolNode = scope->m_SymbolList->m_Head;
	while (symbolNode) {
		Symbol* symbol = (Symbol*)symbolNode->m_Item;
		fprintf(stdout, "%-*s%s %d %s\n", scope->m_Depth + 1, "", stSymbolTypeToString(symbol->m_Type), symbol->m_LineNumber, symbol->m_Identifier);

		symbolNode = symbolNode->m_Next;
	}
	fprintf(stdout, "\n");

	// Print children...
	LinkedListNode* childNode = scope->m_ChildrenList->m_Head;
	while (childNode) {
		printScopeSymbols_r((Scope*)childNode->m_Item);
		childNode = childNode->m_Next;
	}

	fprintf(stdout, "%-*s}\n", scope->m_Depth, "");
}

void progPrintSymbolTable(Program* prog)
{
	assert(prog->m_SymbolTable);
	assert(prog->m_SymbolTable->m_GlobalScope);

	printScopeSymbols_r(prog->m_SymbolTable->m_GlobalScope);
}

//////////////////////////////////////////////////////////////////////////
// AST Nodes
//
const char* astGetIdentifierName(ASTNode* id)
{
	if (id && id->m_Type == AST_Identifier) {
		return ((ASTNodeIdentifier*)id)->m_Name;
	}

	return "INVALID_IDENTIFIER_NODE";
}

ASTNode* astCreateIdentifier(unsigned int lineno, char* name)
{
	ASTNodeIdentifier* node = (ASTNodeIdentifier*)malloc(sizeof(ASTNodeIdentifier));
	node->m_Type = AST_Identifier;
	node->m_LineNumber = lineno;
	node->m_Name = strdup(name);
	return node;
}

ASTNode* astCreateStatement_Expression(unsigned int lineno, ASTNode* expression)
{
	ASTNodeStatementExpression* node = (ASTNodeStatementExpression*)malloc(sizeof(ASTNodeStatementExpression));
	node->m_Type = AST_Statement;
	node->m_LineNumber = lineno;
	node->m_StatementType = ST_Expression;
	node->m_Expr = expression;
	return node;
}

ASTNode* astCreateStatement_If(unsigned int lineno, ASTNode* conditionalExpression, ASTNode* thenStatement, ASTNode* elseStatement)
{
	ASTNodeStatementIf* node = (ASTNodeStatementIf*)malloc(sizeof(ASTNodeStatementIf));
	node->m_Type = AST_Statement;
	node->m_LineNumber = lineno;
	node->m_StatementType = ST_If;
	node->m_Cond = conditionalExpression;
	node->m_Then = thenStatement;
	node->m_Else = elseStatement;
	return node;
}

ASTNode* astCreateStatement_While(unsigned int lineno, ASTNode* conditionalExpression, ASTNode* statement)
{
	ASTNodeStatementWhile* node = (ASTNodeStatementWhile*)malloc(sizeof(ASTNodeStatementWhile));
	node->m_Type = AST_Statement;
	node->m_LineNumber = lineno;
	node->m_StatementType = ST_While;
	node->m_Cond = conditionalExpression;
	node->m_Body = statement;
	return node;
}

ASTNode* astCreateStatement_For(unsigned int lineno, LinkedList* initExpressionList, ASTNode* conditionalExpression, LinkedList* iterExpression, ASTNode* statement)
{
	ASTNodeStatementFor* node = (ASTNodeStatementFor*)malloc(sizeof(ASTNodeStatementFor));
	node->m_Type = AST_Statement;
	node->m_LineNumber = lineno;
	node->m_StatementType = ST_For;
	node->m_Init = initExpressionList;
	node->m_Cond = conditionalExpression;
	node->m_Iter = iterExpression;
	node->m_Body = statement;
	return node;
}

ASTNode* astCreateStatement_Return(unsigned int lineno, ASTNode* expression)
{
	ASTNodeStatementReturn* node = (ASTNodeStatementReturn*)malloc(sizeof(ASTNodeStatementReturn));
	node->m_Type = AST_Statement;
	node->m_LineNumber = lineno;
	node->m_StatementType = ST_Return;
	node->m_Expr = expression;
	return node;
}

ASTNode* astCreateStatement_Generic(unsigned int lineno, StatementType type)
{
	ASTNodeStatement* node = (ASTNodeStatement*)malloc(sizeof(ASTNodeStatement));
	node->m_Type = AST_Statement;
	node->m_LineNumber = lineno;
	node->m_StatementType = type;
	return node;
}

ASTNode* astCreateStatement_Block(unsigned int lineno, LinkedList* statementList)
{
	ASTNodeStatementBlock* node = (ASTNodeStatementBlock*)malloc(sizeof(ASTNodeStatementBlock));
	node->m_Type = AST_Statement;

	// The line number of the block is the line number of the first statement (if there's any).
	// Otherwise (unfortunately) it's the line number of the closing curly bracket.
	if (statementList->m_Head) {
		lineno = ((ASTNode*)statementList->m_Head->m_Item)->m_LineNumber;
	}
	node->m_LineNumber = lineno;
	node->m_StatementType = ST_Block;
	node->m_Body = statementList;
	return node;
}

ASTNode* astCreateStatement_FuncDef(unsigned int lineno, ASTNode* funcDef)
{
	ASTNodeStatementFuncDef* node = (ASTNodeStatementFuncDef*)malloc(sizeof(ASTNodeStatementFuncDef));
	node->m_Type = AST_Statement;
	node->m_LineNumber = lineno;
	node->m_StatementType = ST_FuncDef;
	node->m_FuncDef = funcDef;
	return node;
}

ASTNode* astCreateExpression_Assignment(unsigned int lineno, ASTNode* lvalue, ASTNode* expression)
{
	ASTNodeExpressionAssignment* node = (ASTNodeExpressionAssignment*)malloc(sizeof(ASTNodeExpressionAssignment));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Assignment;
	node->m_LValue = lvalue;
	node->m_Expression = expression;
	return node;
}

ASTNode* astCreateExpression_Op(unsigned int lineno, ASTNode* l, ASTNode* r, OperatorType type)
{
	ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)malloc(sizeof(ASTNodeExpressionOp));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_BinaryOp;
	node->m_Op = type;
	node->m_Left = l;
	node->m_Right = r;
	return node;
}

ASTNode* astCreateExpression_LogicOp(unsigned int lineno, ASTNode* l, ASTNode* r, OperatorType type)
{
	ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)malloc(sizeof(ASTNodeExpressionOp));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_LogicOp;
	node->m_Op = type;
	node->m_Left = l;
	node->m_Right = r;
	return node;
}

ASTNode* astCreateExpression_UnaryOp(unsigned int lineno, ASTNode* expression, OperatorType type)
{
	ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)malloc(sizeof(ASTNodeExpressionOp));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_UnaryOp;
	node->m_Op = type;
	node->m_Left = expression;
	node->m_Right = 0;
	return node;
}

ASTNode* astCreateExpression_Nested(unsigned int lineno, ASTNode* expression)
{
	ASTNodeExpressionNested* node = (ASTNodeExpressionNested*)malloc(sizeof(ASTNodeExpressionNested));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Nested;
	node->m_Expr = expression;
	return node;
}

ASTNode* astCreateExpression_lValue(unsigned int lineno, ASTNode* lvalue)
{
	ASTNodeExpressionLValue* node = (ASTNodeExpressionLValue*)malloc(sizeof(ASTNodeExpressionLValue));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_LValue;
	node->m_LValue = lvalue;
	return node;
}

ASTNode* astCreateExpression_Call(unsigned int lineno, ASTNode* call)
{
	ASTNodeExpressionCall* node = (ASTNodeExpressionCall*)malloc(sizeof(ASTNodeExpressionCall));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Call;
	node->m_Call = call;
	return node;
}

ASTNode* astCreateExpression_ObjDef(unsigned int lineno, LinkedList* list, ObjectDefitionType type)
{
	ASTNodeExpressionObjDef* node = (ASTNodeExpressionObjDef*)malloc(sizeof(ASTNodeExpressionObjDef));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_ObjectDefinition;
	node->m_ObjDefType = type;
	node->m_List = list;
	return node;
}

ASTNode* astCreateExpression_FuncDef(unsigned int lineno, ASTNode* funcDef)
{
	ASTNodeExpressionFuncDef* node = (ASTNodeExpressionFuncDef*)malloc(sizeof(ASTNodeExpressionFuncDef));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_FunctionDefinition;
	node->m_FuncDef = funcDef;
	return node;
}

ASTNode* astCreateExpression_ConstInt(unsigned int lineno, int value)
{
	ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)malloc(sizeof(ASTNodeExpressionConst));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Const;
	node->m_ConstType = Const_Int;
	node->m_IntConst = value;
	return node;
}

ASTNode* astCreateExpression_ConstReal(unsigned int lineno, double value)
{
	ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)malloc(sizeof(ASTNodeExpressionConst));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Const;
	node->m_ConstType = Const_Real;
	node->m_RealConst = value;
	return node;
}

ASTNode* astCreateExpression_ConstString(unsigned int lineno, const char* value)
{
	ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)malloc(sizeof(ASTNodeExpressionConst));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Const;
	node->m_ConstType = Const_String;
	node->m_StringConst = strdup(value);
	return node;
}

ASTNode* astCreateExpression_ConstNil(unsigned int lineno)
{
	ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)malloc(sizeof(ASTNodeExpressionConst));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Const;
	node->m_ConstType = Const_Nil;
	return node;
}

ASTNode* astCreateExpression_ConstTrue(unsigned int lineno)
{
	ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)malloc(sizeof(ASTNodeExpressionConst));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Const;
	node->m_ConstType = Const_Bool;
	node->m_BoolConst = true;
	return node;
}

ASTNode* astCreateExpression_ConstFalse(unsigned int lineno)
{
	ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)malloc(sizeof(ASTNodeExpressionConst));
	node->m_Type = AST_Expression;
	node->m_LineNumber = lineno;
	node->m_ExpressionType = ET_Const;
	node->m_ConstType = Const_Bool;
	node->m_BoolConst = false;
	return node;
}

ASTNode* astCreateFuncDef(unsigned int lineno, ASTNode* id, LinkedList* argumentList, LinkedList* body)
{
	assert(!id || (id && id->m_Type == AST_Identifier));

	if (id) {
		lineno = id->m_LineNumber;
	} else {
		if (argumentList->m_Head) {
			lineno = ((ASTNode*)argumentList->m_Head->m_Item)->m_LineNumber;
		}
	}

	ASTNodeFuncDef* node = (ASTNodeFuncDef*)malloc(sizeof(ASTNodeFuncDef));
	node->m_Type = AST_FunctionDefinition;
	node->m_LineNumber = lineno;
	node->m_Identifier = (ASTNodeIdentifier*)id;
	node->m_Symbol = 0;
	node->m_ArgList = argumentList;
	node->m_Body = body;
	return node;
}

ASTNode* astCreateIndexedElement(unsigned int lineno, ASTNode* expr1, ASTNode* expr2)
{
	ASTNodeIndexedElement* node = (ASTNodeIndexedElement*)malloc(sizeof(ASTNodeIndexedElement));
	node->m_Type = AST_IndexedElement;
	node->m_LineNumber = lineno;
	node->m_Elem = expr1;
	node->m_Index = expr2;
	return node;
}

ASTNode* astCreateLValue_Identifier(unsigned int lineno, ASTNode* id)
{
	assert(id->m_Type == AST_Identifier);
	ASTNodeLValueIdentifier* node = (ASTNodeLValueIdentifier*)malloc(sizeof(ASTNodeLValueIdentifier));
	node->m_Type = AST_LValue;
	node->m_LineNumber = lineno;
	node->m_LValueType = LValue_Identifier;
	node->m_Symbol = 0;
	node->m_ScopeType = IST_Unknown;
	node->m_Identifier = (ASTNodeIdentifier*)id;
	return node;
}

ASTNode* astCreateLValue_LocalIdentifier(unsigned int lineno, ASTNode* id)
{
	assert(id->m_Type == AST_Identifier);
	ASTNodeLValueIdentifier* node = (ASTNodeLValueIdentifier*)malloc(sizeof(ASTNodeLValueIdentifier));
	node->m_Type = AST_LValue;
	node->m_LineNumber = lineno;
	node->m_LValueType = LValue_Identifier;
	node->m_Symbol = 0;
	node->m_ScopeType = IST_Local;
	node->m_Identifier = (ASTNodeIdentifier*)id;
	return node;
}

ASTNode* astCreateLValue_GlobalIdentifier(unsigned int lineno, ASTNode* id)
{
	assert(id->m_Type == AST_Identifier);
	ASTNodeLValueIdentifier* node = (ASTNodeLValueIdentifier*)malloc(sizeof(ASTNodeLValueIdentifier));
	node->m_Type = AST_LValue;
	node->m_LineNumber = lineno;
	node->m_LValueType = LValue_Identifier;
	node->m_Symbol = 0;
	node->m_ScopeType = IST_Global;
	node->m_Identifier = (ASTNodeIdentifier*)id;
	return node;
}

ASTNode* astCreateLValue_ObjectMember(unsigned int lineno, ASTNode* obj, ASTNode* id)
{
	assert(id->m_Type == AST_Identifier);

	ASTNodeLValueObjectMember* node = (ASTNodeLValueObjectMember*)malloc(sizeof(ASTNodeLValueObjectMember));
	node->m_Type = AST_LValue;
	node->m_LineNumber = lineno;
	node->m_LValueType = LValue_ObjectMember;
	node->m_Symbol = 0;
	node->m_Object = obj;
	node->m_Member = (ASTNodeIdentifier*)id;
	return node;
}

ASTNode* astCreateLValue_ArrayElement(unsigned int lineno, ASTNode* arr, ASTNode* expression)
{
	ASTNodeLValueArrayElement* node = (ASTNodeLValueArrayElement*)malloc(sizeof(ASTNodeLValueArrayElement));
	node->m_Type = AST_LValue;
	node->m_LineNumber = lineno;
	node->m_LValueType = LValue_ArrayElement;
	node->m_Symbol = 0;
	node->m_Array = arr;
	node->m_Index = expression;
	return node;
}

ASTNode* astCreateLValue_CallObjectMember(unsigned int lineno, ASTNode* call, ASTNode* id)
{
	assert(id->m_Type == AST_Identifier);
	ASTNodeLValueObjectMember* node = (ASTNodeLValueObjectMember*)malloc(sizeof(ASTNodeLValueObjectMember));
	node->m_Type = AST_LValue;
	node->m_LineNumber = lineno;
	node->m_LValueType = LValue_ObjectMember;
	node->m_Symbol = 0;
	node->m_Object = call;
	node->m_Member = (ASTNodeIdentifier*)id;
	return node;
}

ASTNode* astCreateLValue_CallArrayElement(unsigned int lineno, ASTNode* call, ASTNode* expression)
{
	ASTNodeLValueArrayElement* node = (ASTNodeLValueArrayElement*)malloc(sizeof(ASTNodeLValueArrayElement));
	node->m_Type = AST_LValue;
	node->m_LineNumber = lineno;
	node->m_LValueType = LValue_ArrayElement;
	node->m_Symbol = 0;
	node->m_Array = call;
	node->m_Index = expression;
	return node;
}

ASTNode* astCreateCall_Call(unsigned int lineno, ASTNode* call, LinkedList* args)
{
	ASTNodeCall* node = (ASTNodeCall*)malloc(sizeof(ASTNodeCall));
	node->m_Type = AST_Call;
	node->m_LineNumber = lineno;
	node->m_CallType = Call_ReturnValue;
	node->m_Func = call;
	node->m_Args = args;
	return node;
}

ASTNode* astCreateCall_Normal(unsigned int lineno, ASTNode* lvalue, LinkedList* args)
{
	ASTNodeCall* node = (ASTNodeCall*)malloc(sizeof(ASTNodeCall));
	node->m_Type = AST_Call;
	node->m_LineNumber = lineno;
	node->m_CallType = Call_Normal;
	node->m_Func = lvalue;
	node->m_Args = args;
	return node;
}

ASTNode* astCreateCall_Method(unsigned int lineno, ASTNode* lvalue, ASTNode* id, LinkedList* args)
{
	assert(id->m_Type == AST_Identifier);
	ASTNodeCallMethod* node = (ASTNodeCallMethod*)malloc(sizeof(ASTNodeCallMethod));
	node->m_Type = AST_Call;
	node->m_LineNumber = lineno;
	node->m_CallType = Call_Method;
	node->m_Func = lvalue;
	node->m_Args = args;
	node->m_Identifier = (ASTNodeIdentifier*)id;
	return node;
}

ASTNode* astCreateCall_Lambda(unsigned int lineno, ASTNode* funcDef, LinkedList* args)
{
	ASTNodeCall* node = (ASTNodeCall*)malloc(sizeof(ASTNodeCall));
	node->m_Type = AST_Call;
	node->m_LineNumber = lineno;
	node->m_CallType = Call_Lambda;
	node->m_Func = funcDef;
	node->m_Args = args;
	return node;
}

void astDestroyStatement(ASTNodeStatement* statement)
{
	const StatementType type = statement->m_StatementType;

	if (type == ST_Expression) {
		ASTNodeStatementExpression* node = (ASTNodeStatementExpression*)statement;
		assert(node->m_Expr->m_Type == AST_Expression);
		astDestroyExpression((ASTNodeExpression*)node->m_Expr);
	} else if (type == ST_If) {
		ASTNodeStatementIf* node = (ASTNodeStatementIf*)statement;
		assert(node->m_Cond->m_Type == AST_Expression);
		assert(node->m_Then->m_Type == AST_Statement);
		astDestroyExpression((ASTNodeExpression*)node->m_Cond);
		astDestroyStatement((ASTNodeStatement*)node->m_Then);
		if (node->m_Else) {
			assert(node->m_Else->m_Type == AST_Statement);
			astDestroyStatement((ASTNodeStatement*)node->m_Else);
		}
	} else if (type == ST_While) {
		ASTNodeStatementWhile* node = (ASTNodeStatementWhile*)statement;
		assert(node->m_Cond->m_Type == AST_Expression);
		assert(node->m_Body->m_Type == AST_Statement);
		astDestroyExpression((ASTNodeExpression*)node->m_Cond);
		astDestroyStatement((ASTNodeStatement*)node->m_Body);
	} else if (type == ST_For) {
		ASTNodeStatementFor* node = (ASTNodeStatementFor*)statement;

		LinkedListNode* initNode = node->m_Init->m_Head;
		while (initNode) {
			ASTNode* child = (ASTNode*)initNode->m_Item;
			assert(child->m_Type == AST_Expression);
			astDestroyExpression((ASTNodeExpression*)child);
			initNode = initNode->m_Next;
		}
		llDestroy(node->m_Init);

		assert(node->m_Cond->m_Type == AST_Expression);
		astDestroyExpression((ASTNodeExpression*)node->m_Cond);

		LinkedListNode* iterNode = node->m_Iter->m_Head;
		while (iterNode) {
			ASTNode* child = (ASTNode*)iterNode->m_Item;
			assert(child->m_Type == AST_Expression);
			astDestroyExpression((ASTNodeExpression*)child);
			iterNode = iterNode->m_Next;
		}
		llDestroy(node->m_Iter);

		assert(node->m_Body->m_Type == AST_Statement);
		astDestroyStatement((ASTNodeStatement*)node->m_Body);
	} else if (type == ST_Return) {
		ASTNodeStatementReturn* node = (ASTNodeStatementReturn*)statement;
		if (node->m_Expr) {
			assert(node->m_Expr->m_Type == AST_Expression);
			astDestroyExpression((ASTNodeExpression*)node->m_Expr);
		}
	} else if (type == ST_Block) {
		ASTNodeStatementBlock* node = (ASTNodeStatementBlock*)statement;
		LinkedListNode* llNode = node->m_Body->m_Head;
		while (llNode) {
			ASTNode* astNode = (ASTNode*)llNode->m_Item;
			assert(astNode->m_Type == AST_Statement);
			astDestroyStatement((ASTNodeStatement*)astNode);

			// Move on to the next node...
			llNode = llNode->m_Next;
		}
		llDestroy(node->m_Body);
	} else if (type == ST_FuncDef) {
		ASTNodeStatementFuncDef* node = (ASTNodeStatementFuncDef*)statement;
		assert(node->m_FuncDef->m_Type == AST_FunctionDefinition);
		astDestroyFunctionDefinition((ASTNodeFuncDef*)node->m_FuncDef);
	} else if (type == ST_Break) {
	} else if (type == ST_Continue) {
	} else if (type == ST_Empty) {
	} else {
		assert(false); // Unknown statement type.
	}

	free(statement);
}

void astDestroyExpression(ASTNodeExpression* expression)
{
	const ExpressionType type = expression->m_ExpressionType;

	if (type == ET_Assignment) {
		ASTNodeExpressionAssignment* node = (ASTNodeExpressionAssignment*)expression;
		assert(node->m_LValue->m_Type == AST_LValue);
		assert(node->m_Expression->m_Type == AST_Expression);

		astDestroyExpression((ASTNodeExpression*)node->m_Expression);
		astDestroyLValue((ASTNodeLValue*)node->m_LValue);
	} else if (type == ET_BinaryOp || type == ET_LogicOp) {
		ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)expression;
		assert(node->m_Left->m_Type == AST_Expression);
		assert(node->m_Right->m_Type == AST_Expression);
		astDestroyExpression((ASTNodeExpression*)node->m_Left);
		astDestroyExpression((ASTNodeExpression*)node->m_Right);
	} else if (type == ET_UnaryOp) {
		ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)expression;
		assert(node->m_Right == 0);
		if (node->m_Left->m_Type == AST_Expression) {
			// Unary minus, not
			astDestroyExpression((ASTNodeExpression*)node->m_Left);
		} else if (node->m_Left->m_Type == AST_LValue) {
			// Pre/Post inc/dec or lvalue
			astDestroyLValue((ASTNodeLValue*)node->m_Left);
		}
	} else if (type == ET_Nested) {
		ASTNodeExpressionNested* node = (ASTNodeExpressionNested*)expression;
		assert(node->m_Expr->m_Type == AST_Expression);
		astDestroyExpression((ASTNodeExpression*)node->m_Expr);
	} else if (type == ET_Const) {
		ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)expression;
		if (node->m_ConstType == Const_String) {
			free(node->m_StringConst);
		}
	} else if (type == ET_ObjectDefinition) {
		ASTNodeExpressionObjDef* node = (ASTNodeExpressionObjDef*)expression;
		astDestroyObjectDefinition(node);
		expression = 0;
	} else if (type == ET_FunctionDefinition) {
		ASTNodeExpressionFuncDef* node = (ASTNodeExpressionFuncDef*)expression;
		assert(node->m_FuncDef->m_Type == AST_FunctionDefinition);
		astDestroyFunctionDefinition((ASTNodeFuncDef*)node->m_FuncDef);
	} else if (type == ET_LValue) {
		ASTNodeExpressionLValue* node = (ASTNodeExpressionLValue*)expression;
		assert(node->m_LValue->m_Type == AST_LValue);
		astDestroyLValue((ASTNodeLValue*)node->m_LValue);
	} else if (type == ET_Call) {
		ASTNodeExpressionCall* node = (ASTNodeExpressionCall*)expression;
		assert(node->m_Call->m_Type == AST_Call);
		astDestroyCall((ASTNodeCall*)node->m_Call);
	} else {
		assert(false); // Unknown expression type.
	}

	if (expression) {
		free(expression);
	}
}

void astDestroyFunctionDefinition(ASTNodeFuncDef* funcDef)
{
	if (funcDef->m_Identifier) {
		astDestroyIdentifier(funcDef->m_Identifier);
	}

	// Arguments
	LinkedListNode* argNode = funcDef->m_ArgList->m_Head;
	while (argNode) {
		ASTNode* child = (ASTNode*)argNode->m_Item;
		assert(child->m_Type == AST_Identifier);
		astDestroyIdentifier((ASTNodeIdentifier*)child);
		argNode = argNode->m_Next;
	}
	llDestroy(funcDef->m_ArgList);

	LinkedListNode* bodyNode = funcDef->m_Body->m_Head;
	while (bodyNode) {
		ASTNode* child = (ASTNode*)bodyNode->m_Item;
		assert(child->m_Type == AST_Statement);
		astDestroyStatement((ASTNodeStatement*)child);
		bodyNode = bodyNode->m_Next;
	}
	llDestroy(funcDef->m_Body);

	free(funcDef);
}

void astDestroyLValue(ASTNodeLValue* lvalue)
{
	const LValueType type = lvalue->m_LValueType;

	if (type == LValue_Identifier) {
		ASTNodeLValueIdentifier* node = (ASTNodeLValueIdentifier*)lvalue;
		astDestroyIdentifier(node->m_Identifier);
	} else if (type == LValue_ObjectMember) {
		ASTNodeLValueObjectMember* node = (ASTNodeLValueObjectMember*)lvalue;
		if (node->m_Object->m_Type == AST_LValue) {
			astDestroyLValue((ASTNodeLValue*)node->m_Object);
		} else if (node->m_Object->m_Type == AST_Call) {
			astDestroyCall((ASTNodeCall*)node->m_Object);
		}
	} else if (type == LValue_ArrayElement) {
		ASTNodeLValueArrayElement* node = (ASTNodeLValueArrayElement*)lvalue;
		assert(node->m_Array->m_Type == AST_LValue);
		assert(node->m_Index->m_Type == AST_Expression);
		astDestroyExpression((ASTNodeExpression*)node->m_Index);
		astDestroyLValue((ASTNodeLValue*)node->m_Array);
	} else {
		// Unknown lvalue type.
		assert(false);
	}

	free(lvalue);
}

void astDestroyObjectDefinition(ASTNodeExpressionObjDef* objDef)
{
	const ObjectDefitionType type = objDef->m_ObjDefType;

	if (type == ObjDef_ExpressionList) {
		LinkedListNode* llNode = objDef->m_List->m_Head;
		while (llNode) {
			ASTNode* child = (ASTNode*)llNode->m_Item;
			assert(child->m_Type == AST_Expression);
			astDestroyExpression((ASTNodeExpression*)child);
			llNode = llNode->m_Next;
		}
		llDestroy(objDef->m_List);
	} else if (type == ObjDef_IndexedElementList) {
		LinkedListNode* llNode = objDef->m_List->m_Head;
		while (llNode) {
			ASTNode* child = (ASTNode*)llNode->m_Item;
			assert(child->m_Type == AST_IndexedElement);
			astDestroyIndexedElement((ASTNodeIndexedElement*)child);
			llNode = llNode->m_Next;
		}
		llDestroy(objDef->m_List);
	} else {
		// Unknown object definition type
		assert(false);
	}

	free(objDef);
}

void astDestroyCall(ASTNodeCall* call)
{
	const CallType type = call->m_CallType;

	if (type == Call_ReturnValue) {
		assert(call->m_Func->m_Type == AST_Call);
		astDestroyCall((ASTNodeCall*)call->m_Func);
	} else if (type == Call_Normal) {
		assert(call->m_Func->m_Type == AST_LValue);
		astDestroyLValue((ASTNodeLValue*)call->m_Func);
	} else if (type == Call_Method) {
		assert(call->m_Func->m_Type == AST_LValue);
		astDestroyLValue((ASTNodeLValue*)call->m_Func);
	} else if (type == Call_Lambda) {
		assert(call->m_Func->m_Type == AST_FunctionDefinition);
		astDestroyFunctionDefinition((ASTNodeFuncDef*)call->m_Func);
	} else {
		// Unknown call type.
		assert(false);
	}

	// Check arguments
	LinkedListNode* argNode = call->m_Args->m_Head;
	while (argNode) {
		ASTNode* child = (ASTNode*)argNode->m_Item;
		assert(child->m_Type == AST_Expression);
		astDestroyExpression((ASTNodeExpression*)child);
		argNode = argNode->m_Next;
	}
	llDestroy(call->m_Args);

	free(call);
}

void astDestroyIdentifier(ASTNodeIdentifier* id)
{
	free(id->m_Name);
	free(id);
}

void astDestroyIndexedElement(ASTNodeIndexedElement* indexedElement)
{
	assert(indexedElement->m_Index->m_Type == AST_Expression);
	assert(indexedElement->m_Elem->m_Type == AST_Expression);

	astDestroyExpression((ASTNodeExpression*)indexedElement->m_Index);
	astDestroyExpression((ASTNodeExpression*)indexedElement->m_Elem);

	free(indexedElement);
}

//////////////////////////////////////////////////////////////////////////
// State stack
//
void initStateStack(StateStack* ss)
{
	ss->m_StateCapacity = 8;
	ss->m_States = (State*)malloc(sizeof(State) * ss->m_StateCapacity);
	ss->m_NumStates = 1;
	
	State* s = getState(ss);
	s->m_IsInsideFunctionBody = false;
	s->m_IsInsideLoopBody = false;
	s->m_IsFunctionCall = false;
}

void destroyStateStack(StateStack* ss)
{
	free(ss->m_States);
	ss->m_States = 0;
	ss->m_NumStates = 0;
	ss->m_StateCapacity = 0;
}

State* getState(StateStack* ss)
{
	return &ss->m_States[ss->m_NumStates - 1];
}

State* pushState(StateStack* ss)
{
	if (ss->m_NumStates + 1 > ss->m_StateCapacity) {
		ss->m_StateCapacity += 8;
		ss->m_States = (State*)realloc(ss->m_States, sizeof(State) * ss->m_StateCapacity);
	}

	memcpy(&ss->m_States[ss->m_NumStates], &ss->m_States[ss->m_NumStates - 1], sizeof(State));
	ss->m_NumStates++;

	return &ss->m_States[ss->m_NumStates - 1];
}

void popState(StateStack* ss)
{
	assert(ss->m_NumStates > 0);
	ss->m_NumStates--;
}

//////////////////////////////////////////////////////////////////////////
// Helpers
//
void printError(const char* msg, ...)
{
	va_list argptr;
	va_start(argptr, msg);
	vfprintf(stderr, msg, argptr);
	va_end(argptr);
	fprintf(stderr, "\n");
}
