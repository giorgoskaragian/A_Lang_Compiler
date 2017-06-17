#include "icode.h"
#include "ast.h"
#include "symbol_table.h"
#include "linked_list.h"
#include <malloc.h> // malloc()/free()
#include <memory.h> // memset()
#include <string.h> // strdup()
#include <stdio.h> // printf()
#include <assert.h>

// Temporary labels for backpatching.
#define UNUSED_LABEL 0xFFFFFFFF
#define TEMP_BOOLEAN_EXPR_EARLY_OUT_LABEL 0xFFFFFFFE
#define TEMP_FALSE_LABEL 0xFFFFFFFD
#define TEMP_TRUE_LABEL 0xFFFFFFFC
#define TEMP_LOOP_END_LABEL 0xFFFFFFFB
#define TEMP_LOOP_CONTINUE_LABEL 0xFFFFFFFA

// Set to 1 to show variable and function addresses and scopes.
#ifndef OUTPUT_SHOW_ADDRESSES
#define OUTPUT_SHOW_ADDRESSES 0
#endif

// Forward declarations of private module functions.
void compileFunctionDefinition(ICode* ic, ASTNodeFuncDef* funcDef);
void compileStatement(ICode* ic, ASTNodeStatement* statement);
void compileCall(ICode* ic, ASTNodeCall* call, Scope* symScope);
ICodeExpression* compileExpression(ICode* ic, ASTNodeExpression* expression, Scope* symScope);

// Prints the variable/function/constant of an expression.
void icodePrintExpression(ICodeExpression* ice)
{
#if OUTPUT_SHOW_ADDRESSES
	if (ice->m_Type == ICET_Variable) {
		assert(ice->m_Symbol != 0);
		assert(ice->m_Symbol->m_Identifier != 0);
		Symbol* sym = ice->m_Symbol;
		if (sym->m_Type == SymType_FunctionArgument) {
			fprintf(stdout, "%s (%s[%d])", sym->m_Identifier, "arg", sym->m_Offset);
		} else {
			assert(sym->m_Type == SymType_Variable || sym->m_Type == SymType_TempVariable);
			assert(sym->m_ParentScope);
			if (sym->m_ParentScope->m_ID == 0) {
				fprintf(stdout, "%s (%s[%d])", sym->m_Identifier, "global", sym->m_Offset);
			} else {
				fprintf(stdout, "%s (%s[%d])", sym->m_Identifier, "local", sym->m_Offset);
			}
		}
	} else if (ice->m_Type == ICET_ProgramFunction) {
		assert(ice->m_Symbol != 0);
		assert(ice->m_Symbol->m_Identifier != 0);
		assert(ice->m_Symbol->m_Type == SymType_Function);
		Symbol* sym = ice->m_Symbol;
		fprintf(stdout, "%s (addr: %d)", sym->m_Identifier, sym->m_Offset);
#else
	if (ice->m_Type == ICET_Variable || ice->m_Type == ICET_ProgramFunction) {
		assert(ice->m_Symbol != 0);
		assert(ice->m_Symbol->m_Identifier != 0);
		Symbol* sym = ice->m_Symbol;
		fprintf(stdout, "%s", sym->m_Identifier);
#endif
	} else if (ice->m_Type == ICET_LibraryFunction ||
		ice->m_Type == ICET_TableItem) {
		assert(ice->m_Symbol != 0);
		assert(ice->m_Symbol->m_Identifier != 0);
		fprintf(stdout, "%s", ice->m_Symbol->m_Identifier);
	} else if (ice->m_Type == ICET_ConstBool) {
		fprintf(stdout, "%s", ice->m_BoolConst == true ? "TRUE" : "FALSE");
	} else if (ice->m_Type == ICET_ConstNum) {
		fprintf(stdout, "%g", ice->m_NumConst);
	} else if (ice->m_Type == ICET_ConstString) {
		fprintf(stdout, "\"%s\"", ice->m_StrConst);
	} else if (ice->m_Type == ICET_Nil) {
		fprintf(stdout, "NIL");
	} else {
		assert(false); // Cannot print other kinds of expressions atm.
	}
}

// Prints the expression's opcode
void icodePrintOp(ICodeOperationType op)
{
	switch (op) {
	case ICOT_Assign:
		fprintf(stdout, "ASSIGN");
		break;
	case ICOT_Add:
		fprintf(stdout, "ADD");
		break;
	case ICOT_Sub:
		fprintf(stdout, "SUB");
		break;
	case ICOT_Mul:
		fprintf(stdout, "MUL");
		break;
	case ICOT_Div:
		fprintf(stdout, "DIV");
		break;
	case ICOT_Mod:
		fprintf(stdout, "MOD");
		break;
	case ICOT_UnaryMinus:
		fprintf(stdout, "UMINUS");
		break;
//	case ICOT_And:
//		fprintf(stdout, "AND");
//		break;
//	case ICOT_Or:
//		fprintf(stdout, "OR");
//		break;
	case ICOT_Not:
		fprintf(stdout, "NOT");
		break;
	case ICOT_IfEqual:
		fprintf(stdout, "IF_EQ");
		break;
	case ICOT_IfNotEqual:
		fprintf(stdout, "IF_NOTEQ");
		break;
	case ICOT_IfLessEqual:
		fprintf(stdout, "IF_LESSEQ");
		break;
	case ICOT_IfGreaterEqual:
		fprintf(stdout, "IF_GREATEREQ");
		break;
	case ICOT_IfLess:
		fprintf(stdout, "IF_LESS");
		break;
	case ICOT_IfGreater:
		fprintf(stdout, "IF_GREATER");
		break;
	case ICOT_Jump:
		fprintf(stdout, "JUMP");
		break;
	case ICOT_Call:
		fprintf(stdout, "CALL");
		break;
	case ICOT_Param:
		fprintf(stdout, "PARAM");
		break;
	case ICOT_Return:
		fprintf(stdout, "RETURN");
		break;
	case ICOT_GetRetValue:
		fprintf(stdout, "GETRETVAL");
		break;
	case ICOT_FunctionStart:
		fprintf(stdout, "FUNCSTART");
		break;
	case ICOT_FunctionEnd:
		fprintf(stdout, "FUNCEND");
		break;
	case ICOT_TableCreate:
		fprintf(stdout, "TABLECREATE");
		break;
	case ICOT_TableGetElement:
		fprintf(stdout, "TABLEGETELEM");
		break;
	case ICOT_TableSetElement:
		fprintf(stdout, "TABLESETELEM");
		break;
	default:
		assert(false); // Unknown expression op type.
	}
}

// Create a new expression with a specific type. 
// This is the only place allocating ICodeExpression objects.
ICodeExpression* icodeCreateExpression(ICodeExpressionType type)
{
	ICodeExpression* ice = (ICodeExpression*)malloc(sizeof(ICodeExpression));
	memset(ice, 0, sizeof(ICodeExpression));
	ice->m_Type = type;
	return ice;
}

// Create a temporary variable expression in the specified scope.
// NOTE: The Scope is responsible for recycling temporary variables.
ICodeExpression* icodeCreateExpression_TempVariable(Scope* scope)
{
	ICodeExpression* ice = icodeCreateExpression(ICET_Variable);
	ice->m_Symbol = stInsertSymbol_TempVariable(scope);
	return ice;
}

// Create a new variable/function expression from the specified Symbol
ICodeExpression* icodeCreateExpression_Variable(Symbol* symbol)
{
	ICodeExpression* ice = 0;
	if (symbol->m_Type == SymType_Function) {
		if (symbol->m_LineNumber == 0) {
			ice = icodeCreateExpression(ICET_LibraryFunction);
		} else {
			ice = icodeCreateExpression(ICET_ProgramFunction);
		}
	} else {
		ice = icodeCreateExpression(ICET_Variable);
	}
	ice->m_Symbol = symbol;
	return ice;
}

// Create a program function expression from the specified AST function definition
// NOTE: Used only for function declarations. The above function (icodeCreateExpression_Variable)
// is used mostly for function calls
ICodeExpression* icodeCreateExpression_ProgramFunction(ASTNodeFuncDef* funcDef)
{
	ICodeExpression* ice = icodeCreateExpression(ICET_ProgramFunction);
	ice->m_Symbol = funcDef->m_Symbol;
	ice->m_FuncScope = funcDef->m_Scope;
	return ice;
}

// Constant number expression
ICodeExpression* icodeCreateExpression_NumConst(double v)
{
	ICodeExpression* ice = icodeCreateExpression(ICET_ConstNum);
	ice->m_NumConst = v;
	return ice;
}

// Constant boolean expression
ICodeExpression* icodeCreateExpression_BoolConst(bool v)
{
	ICodeExpression* ice = icodeCreateExpression(ICET_ConstBool);
	ice->m_BoolConst = v;
	return ice;
}

// Constant string expression.
ICodeExpression* icodeCreateExpression_StringConst(const char* v)
{
	ICodeExpression* ice = icodeCreateExpression(ICET_ConstString);
	ice->m_StrConst = strdup(v);
	return ice;
}

// Inserts a new quad at the end of the ICode's quad list 
unsigned int icodeEmitQuad(ICode* ic, ICodeOperationType op, ICodeExpression* result, ICodeExpression* arg1, ICodeExpression* arg2, unsigned int label, unsigned int line)
{
	if (ic->m_NumQuads + 1 > ic->m_QuadCapacity) {
		ic->m_QuadCapacity += 1024;
		ic->m_Quads = (ICodeQuad*)realloc(ic->m_Quads, sizeof(ICodeQuad) * ic->m_QuadCapacity);
	}

	ICodeQuad* icq = &ic->m_Quads[ic->m_NumQuads++];
	icq->m_Op = op;
	icq->m_Result = result;
	icq->m_Arg1 = arg1;
	icq->m_Arg2 = arg2;
	icq->m_Label = label;
	icq->m_Line = line;

	return ic->m_NumQuads - 1;
}

// Emits a TABLEGETELEM quad if the expression is a TableItem expression. Otherwise it doesn't emit anything
// NOTE: This is used whenever an expression is used as an r-value and we aren't sure about its type.
ICodeExpression* icodeEmitIfTableItem(ICode* ic, ICodeExpression* expr, Scope* symScope)
{
	if (expr->m_Type != ICET_TableItem) {
		return expr;
	}

	ICodeExpression* res = icodeCreateExpression_TempVariable(symScope);
	icodeEmitQuad(ic, ICOT_TableGetElement, res, expr, expr->m_Index, UNUSED_LABEL, ~0u); // TODO: Line number?
	return res;
}

// Replace oldLabel with newLabel starting from the firstQuad until the end of the quad list.
void icodeBackpatchLabel(ICode* ic, unsigned int firstQuad, unsigned int oldLabel, unsigned int newLabel)
{
	const unsigned int numQuads = ic->m_NumQuads;
	for (unsigned int i = firstQuad; i < numQuads; ++i) {
		if (ic->m_Quads[i].m_Label == oldLabel) {
			ic->m_Quads[i].m_Label = newLabel;
		}
	}
}

// Translate an AST logic operation into icode logic operation.
ICodeOperationType icodeLogicOpToICodeOp(OperatorType op)
{
	switch (op) {
	case LogicOp_GreaterThan:
		return ICOT_IfGreater;
	case LogicOp_GreaterThanOrEqual:
		return ICOT_IfGreaterEqual;
	case LogicOp_LessThan:
		return ICOT_IfLess;
	case LogicOp_LessThanOrEqual:
		return ICOT_IfLessEqual;
	case LogicOp_Equal:
		return ICOT_IfEqual;
	case LogicOp_NotEqual:
		return ICOT_IfNotEqual;
	}

	assert(false);
	return ICOT_IfEqual;
}

// Create a new icode expression from the specified AST constant node.
// NOTE: At this point both Real and Int AST constant nodes are mapped to ConstNum icode expressions.
ICodeExpression* compileExpression_Const(ASTNodeExpressionConst* constExpression)
{
	ICodeExpression* ice = 0;

	ConstType type = constExpression->m_ConstType;
	if (type == Const_Int) {
		ice = icodeCreateExpression_NumConst((double)constExpression->m_IntConst);
	} else if (type == Const_Real) {
		ice = icodeCreateExpression_NumConst(constExpression->m_RealConst);
	} else if (type == Const_String) {
		ice = icodeCreateExpression_StringConst(constExpression->m_StringConst);
	} else if (type == Const_Bool) {
		ice = icodeCreateExpression_BoolConst(constExpression->m_BoolConst);
	} else if (type == Const_Nil) {
		ice = icodeCreateExpression(ICET_Nil);
	} else {
		assert(false); // Unknown const type.
	}

	return ice;
}

// Compile an AST object definition node.
// Depending on the type of the object definition (expression list or indexed element list)
// one TABLESETELEM quad is created for each object element. The only difference between the two
// cases is the type of the element index (increasing number in the first case, named key in the 
// second case).
ICodeExpression* compileExpression_ObjectDefinition(ICode* ic, ASTNodeExpressionObjDef* objDef, Scope* symScope)
{
	ICodeExpression* obj = 0;

	ObjectDefitionType type = objDef->m_ObjDefType;
	if (type == ObjDef_ExpressionList) {
		// Create the table and keep it in a temporary variable (this is the expression returned by this function.
		obj = icodeCreateExpression_TempVariable(symScope);
		icodeEmitQuad(ic, ICOT_TableCreate, obj, 0, 0, UNUSED_LABEL, objDef->m_LineNumber);

		// Create one TABLESETELEM for each table element.
		unsigned int nodeID = 0;
		LinkedListNode* listNode = objDef->m_List->m_Head;
		while (listNode) {
			ASTNode* node = (ASTNode*)listNode->m_Item;
			assert(node->m_Type == AST_Expression);

			// Evaluate the expression for this table element.
			ICodeExpression* nodeExpr = compileExpression(ic, (ASTNodeExpression*)node, symScope);

			// Emit a TABLESETELEM adding the result of the expression to the table.
			icodeEmitQuad(ic, ICOT_TableSetElement, obj, icodeCreateExpression_NumConst(nodeID), nodeExpr, UNUSED_LABEL, node->m_LineNumber);

			++nodeID;
			listNode = listNode->m_Next;
		}
	} else if (type == ObjDef_IndexedElementList) {
		// Create the table and keep it in a temporary variable (this is the expression returned by this function.
		obj = icodeCreateExpression_TempVariable(symScope);
		icodeEmitQuad(ic, ICOT_TableCreate, obj, 0, 0, UNUSED_LABEL, objDef->m_LineNumber);

		// Create one TABLESETELEM for each table element.
		LinkedListNode* listNode = objDef->m_List->m_Head;
		while (listNode) {
			ASTNode* node = (ASTNode*)listNode->m_Item;
			assert(node->m_Type == AST_IndexedElement);

			ASTNodeIndexedElement* indexedElement = (ASTNodeIndexedElement*)node;

			// Evaluate the index expression into a temporary variable.
			ICodeExpression* indexExpr = compileExpression(ic, (ASTNodeExpression*)indexedElement->m_Index, symScope);

			// Evaluate the element expression into another temporary variable.
			ICodeExpression* elemExpr = compileExpression(ic, (ASTNodeExpression*)indexedElement->m_Elem, symScope);

			// Emit a TABLESETELEM quad adding the result of the element expression to the index specified by the index expression.
			icodeEmitQuad(ic, ICOT_TableSetElement, obj, indexExpr, elemExpr, UNUSED_LABEL, node->m_LineNumber);

			listNode = listNode->m_Next;
		}
	} else {
		assert(false); // Unknown object definition type.
	}

	return obj;
}

ICodeExpression* compileLValue(ICode* ic, ASTNodeLValue* lvalue, Scope* symScope)
{
	ICodeExpression* ice = 0;
	LValueType type = lvalue->m_LValueType;
	if (type == LValue_Identifier) {
		ASTNodeLValueIdentifier* node = (ASTNodeLValueIdentifier*)lvalue;
		ice = icodeCreateExpression_Variable(node->m_Symbol);
	} else if (type == LValue_ObjectMember) {
		ASTNodeLValueObjectMember* node = (ASTNodeLValueObjectMember*)lvalue;
		if (node->m_Object->m_Type == AST_LValue) {
			ICodeExpression* objMemberExpr = compileLValue(ic, (ASTNodeLValue*)node->m_Object, symScope);

			ICodeExpression* lvalueExpr = icodeEmitIfTableItem(ic, objMemberExpr, symScope);

			ice = icodeCreateExpression(ICET_TableItem);
			ice->m_Symbol = lvalueExpr->m_Symbol;
			ice->m_Index = icodeCreateExpression_StringConst(node->m_Member->m_Name);
		} else if (node->m_Object->m_Type == AST_Call) {
			compileCall(ic, (ASTNodeCall*)node->m_Object, symScope);

			ICodeExpression* retVal = icodeCreateExpression_TempVariable(symScope);
			icodeEmitQuad(ic, ICOT_GetRetValue, retVal, 0, 0, UNUSED_LABEL, lvalue->m_LineNumber);

			ice = icodeCreateExpression(ICET_TableItem);
			ice->m_Symbol = retVal->m_Symbol;
			ice->m_Index = icodeCreateExpression_StringConst(node->m_Member->m_Name);
		} else {
			assert(false); // Unknown case
		}
	} else if (type == LValue_ArrayElement) {
		ASTNodeLValueArrayElement* node = (ASTNodeLValueArrayElement*)lvalue;
		if (node->m_Array->m_Type == AST_LValue) {
			ICodeExpression* arrElemExpr = compileLValue(ic, (ASTNodeLValue*)node->m_Array, symScope);
			ICodeExpression* lvalueExpr = icodeEmitIfTableItem(ic, arrElemExpr, symScope);

			ICodeExpression* indexExpr = compileExpression(ic, (ASTNodeExpression*)node->m_Index, symScope);

			ice = icodeCreateExpression(ICET_TableItem);
			ice->m_Symbol = lvalueExpr->m_Symbol;
			ice->m_Index = indexExpr;
		} else if (node->m_Array->m_Type == AST_Call) {
			compileCall(ic, (ASTNodeCall*)node->m_Array, symScope);

			ICodeExpression* retVal = icodeCreateExpression_TempVariable(symScope);
			icodeEmitQuad(ic, ICOT_GetRetValue, retVal, 0, 0, UNUSED_LABEL, lvalue->m_LineNumber);

			ICodeExpression* indexExpr = compileExpression(ic, (ASTNodeExpression*)node->m_Index, symScope);

			ice = icodeCreateExpression(ICET_TableItem);
			ice->m_Symbol = retVal->m_Symbol;
			ice->m_Index = indexExpr;
		} else {
			assert(false); // Unknown case
		}
	} else {
		assert(false); // Unknown lvalue type.
	}

	return ice;
}

ICodeExpression* compileExpression_BinaryOp(ICode* ic, ASTNodeExpressionOp* expression, Scope* symScope)
{
	ICodeExpression* leftExpression = compileExpression(ic, (ASTNodeExpression*)expression->m_Left, symScope);
	ICodeExpression* rightExpression = compileExpression(ic, (ASTNodeExpression*)expression->m_Right, symScope);

	leftExpression = icodeEmitIfTableItem(ic, leftExpression, symScope);
	rightExpression = icodeEmitIfTableItem(ic, rightExpression, symScope);

	// Emit quad for binary op between the two expressions.
	ICodeExpression* res = icodeCreateExpression_TempVariable(symScope);
	switch (expression->m_Op) {
	case Op_Add:
		icodeEmitQuad(ic, ICOT_Add, res, leftExpression, rightExpression, UNUSED_LABEL, expression->m_LineNumber);
		break;
	case Op_Subtract:
		icodeEmitQuad(ic, ICOT_Sub, res, leftExpression, rightExpression, UNUSED_LABEL, expression->m_LineNumber);
		break;
	case Op_Multiply:
		icodeEmitQuad(ic, ICOT_Mul, res, leftExpression, rightExpression, UNUSED_LABEL, expression->m_LineNumber);
		break;
	case Op_Divide:
		icodeEmitQuad(ic, ICOT_Div, res, leftExpression, rightExpression, UNUSED_LABEL, expression->m_LineNumber);
		break;
	case Op_Modulo:
		icodeEmitQuad(ic, ICOT_Mod, res, leftExpression, rightExpression, UNUSED_LABEL, expression->m_LineNumber);
		break;
	}

	return res;
}

// Compile an AST expression node into icode quads.
ICodeExpression* compileExpression(ICode* ic, ASTNodeExpression* expression, Scope* symScope)
{
	ICodeExpression* ice = 0;

	ExpressionType type = expression->m_ExpressionType;
	if (type == ET_Assignment) {
		// lvalue = expression
		ASTNodeExpressionAssignment* node = (ASTNodeExpressionAssignment*)expression;
		
		// Compile the right hand side expression (recursively).
		ASTNodeExpression* rvalue = (ASTNodeExpression*)node->m_Expression;
		ICodeExpression* rvalueExpression = compileExpression(ic, rvalue, symScope);

		// If this is a table item, emit a TABLEGETELEM quad.
		rvalueExpression = icodeEmitIfTableItem(ic, rvalueExpression, symScope);

		// Compile the left hand side of the assignment.
		ASTNodeLValue* lvalue = (ASTNodeLValue*)node->m_LValue;
		ICodeExpression* lvalueExpression = compileLValue(ic, lvalue, symScope);

		// If this is a table item, emit a TABLESETELEM quad. Otherwise emit an ASSIGN quad.
		if (lvalueExpression->m_Type != ICET_TableItem) {
			icodeEmitQuad(ic, ICOT_Assign, lvalueExpression, rvalueExpression, 0, UNUSED_LABEL, expression->m_LineNumber);
		} else {
			icodeEmitQuad(ic, ICOT_TableSetElement, lvalueExpression, lvalueExpression->m_Index, rvalueExpression, UNUSED_LABEL, expression->m_LineNumber);
		}

		// Finally emit an optional TABLEGETELEM quad in case the caller needs the result of the assignment 
		// (which is the lvalue). Note that if the lvalue isn't a table element, icodeEmitIfTableItem() will not
		// emit anything (ice will be the same as lvalueExpression).
		ice = icodeEmitIfTableItem(ic, lvalueExpression, symScope);
	} else if (type == ET_BinaryOp) {
		// expr <op> expr
		// Compile an arithmetic operation between two expressions.
		ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)expression;
		assert(node->m_Op >= Op_Add && node->m_Op <= Op_Modulo);
		ice = compileExpression_BinaryOp(ic, node, symScope);
	} else if (type == ET_LogicOp) {
		// expr <logicop> expr
		// Compile a logic operation between two expressions.
		ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)expression;
		if (node->m_Op >= LogicOp_GreaterThan && node->m_Op <= LogicOp_NotEqual) {
			// expr <relop> expr
			//
			// IF_relop leftExpr, rightExpr, TRUE_LABEL
			// ASSIGN tmp, false,
			// JUMP END
			// TRUE_LABEL: ASSIGN tmp true
			// END: ...

			// Compile both expressions recursively.
			ICodeExpression* leftExpression = compileExpression(ic, (ASTNodeExpression*)node->m_Left, symScope);
			ICodeExpression* rightExpression = compileExpression(ic, (ASTNodeExpression*)node->m_Right, symScope);

			// Emit TABLEGETELEM quads if the results are TableItem expressions.
			leftExpression = icodeEmitIfTableItem(ic, leftExpression, symScope);
			rightExpression = icodeEmitIfTableItem(ic, rightExpression, symScope);

			// Create a temporary variable to hold the result of the comparison (tmp in the code snippet above).
			ice = icodeCreateExpression_TempVariable(symScope);

			const unsigned int firstQuad = ic->m_NumQuads;

			// Emit the quad comparing the two expressions. If the result is true, jump to the dummy TEMP_TRUE_LABEL label.
			icodeEmitQuad(ic, icodeLogicOpToICodeOp(node->m_Op), 0, leftExpression, rightExpression, TEMP_TRUE_LABEL, expression->m_LineNumber);
			// The result is false, so assign FALSE to the temporary variable.
			icodeEmitQuad(ic, ICOT_Assign, ice, icodeCreateExpression_BoolConst(false), 0, UNUSED_LABEL, expression->m_LineNumber);
			// Jump 1 quad (away of the TRUE case; END label in the code snippet above).
			icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, ic->m_NumQuads + 2, expression->m_LineNumber);
			// The result is true, so assign TRUE to the temprary variable.
			// NOTE: This is the true label for the IF_XXX quad above.
			icodeEmitQuad(ic, ICOT_Assign, ice, icodeCreateExpression_BoolConst(true), 0, UNUSED_LABEL, expression->m_LineNumber);

			// Backpatch the TEMP_TRUE_LABEL with the actual label.
			icodeBackpatchLabel(ic, firstQuad, TEMP_TRUE_LABEL, ic->m_NumQuads - 1);
		} else if (node->m_Op == LogicOp_And) {
			// expr and expr
			// Short-circuit evaluation (i.e. if left expression is false, 
			// don't evaluate the right expression because there's no way the 'and' will be true.
			//
			// ... ; evaluate left expression.
			// IF_NEQ leftExpr, TRUE, TEMP_EARLY_OUT_LABEL
			// ... ; evaluate right expression.
			// IF_NEQ rightExpr, TRUE, TEMP_EARLY_OUT_LABEL
			// ASSIGN tmp, TRUE
			// JUMP END
			// TEMP_EARLY_OUT_LABEL: ASSIGN tmp, FALSE
			// END: ...
			ice = icodeCreateExpression_TempVariable(symScope);

			const unsigned int firstQuad = ic->m_NumQuads;
			// ... ; evaluate left expression.
			ICodeExpression* leftExpression = compileExpression(ic, (ASTNodeExpression*)node->m_Left, symScope);
			leftExpression = icodeEmitIfTableItem(ic, leftExpression, symScope);

			// IF_NEQ leftExpr, TRUE, TEMP_EARLY_OUT_LABEL
			icodeEmitQuad(ic, ICOT_IfNotEqual, 0, leftExpression, icodeCreateExpression_BoolConst(true), TEMP_BOOLEAN_EXPR_EARLY_OUT_LABEL, expression->m_LineNumber);

			// ... ; evaluate right expression.
			ICodeExpression* rightExpression = compileExpression(ic, (ASTNodeExpression*)node->m_Right, symScope);
			rightExpression = icodeEmitIfTableItem(ic, rightExpression, symScope);

			// IF_NEQ rightExpr, TRUE, TEMP_EARLY_OUT_LABEL
			icodeEmitQuad(ic, ICOT_IfNotEqual, 0, rightExpression, icodeCreateExpression_BoolConst(true), TEMP_BOOLEAN_EXPR_EARLY_OUT_LABEL, expression->m_LineNumber);

			// ASSIGN tmp, TRUE
			icodeEmitQuad(ic, ICOT_Assign, ice, icodeCreateExpression_BoolConst(true), 0, UNUSED_LABEL, expression->m_LineNumber);

			// JUMP END
			icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, ic->m_NumQuads + 2, expression->m_LineNumber);

			// TEMP_EARLY_OUT_LABEL: ASSIGN tmp, FALSE
			icodeEmitQuad(ic, ICOT_Assign, ice, icodeCreateExpression_BoolConst(false), 0, UNUSED_LABEL, expression->m_LineNumber);

			// Backpatch TEMP_EARLY_OUT_LABEL with the actual label.
			icodeBackpatchLabel(ic, firstQuad, TEMP_BOOLEAN_EXPR_EARLY_OUT_LABEL, ic->m_NumQuads - 1);
		} else if (node->m_Op == LogicOp_Or) {
			// expr or expr
			// Short-circuit evaluation (i.e. if left expression is true, 
			// don't evaluate the right expression because we are sure the 'or' will be true.
			//
			// ... ; evaluate left expression.
			// IF_EQ leftExpr, TRUE, TEMP_EARLY_OUT_LABEL
			// ... ; evaluate right expression.
			// IF_EQ rightExpr, TRUE, TEMP_EARLY_OUT_LABEL
			// ASSIGN tmp, FALSE
			// JUMP END
			// TEMP_EARLY_OUT_LABEL: ASSIGN tmp, TRUE
			// END: ...
			ice = icodeCreateExpression_TempVariable(symScope);

			const unsigned int firstQuad = ic->m_NumQuads;

			// ... ; evaluate left expression.
			ICodeExpression* leftExpression = compileExpression(ic, (ASTNodeExpression*)node->m_Left, symScope);
			leftExpression = icodeEmitIfTableItem(ic, leftExpression, symScope);

			// IF_EQ leftExpr, TRUE, TEMP_EARLY_OUT_LABEL
			icodeEmitQuad(ic, ICOT_IfEqual, 0, leftExpression, icodeCreateExpression_BoolConst(true), TEMP_BOOLEAN_EXPR_EARLY_OUT_LABEL, expression->m_LineNumber);

			// ... ; evaluate right expression.
			ICodeExpression* rightExpression = compileExpression(ic, (ASTNodeExpression*)node->m_Right, symScope);
			rightExpression = icodeEmitIfTableItem(ic, rightExpression, symScope);

			// IF_EQ rightExpr, TRUE, TEMP_EARLY_OUT_LABEL
			icodeEmitQuad(ic, ICOT_IfEqual, 0, rightExpression, icodeCreateExpression_BoolConst(true), TEMP_BOOLEAN_EXPR_EARLY_OUT_LABEL, expression->m_LineNumber);
			
			// ASSIGN tmp, FALSE
			icodeEmitQuad(ic, ICOT_Assign, ice, icodeCreateExpression_BoolConst(false), 0, UNUSED_LABEL, expression->m_LineNumber);

			// JUMP END
			icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, ic->m_NumQuads + 2, expression->m_LineNumber);

			// TEMP_EARLY_OUT_LABEL: ASSIGN tmp, TRUE
			icodeEmitQuad(ic, ICOT_Assign, ice, icodeCreateExpression_BoolConst(true), 0, UNUSED_LABEL, expression->m_LineNumber);

			// Backpatch TEMP_EARLY_OUT_LABEL with the actual label.
			icodeBackpatchLabel(ic, firstQuad, TEMP_BOOLEAN_EXPR_EARLY_OUT_LABEL, ic->m_NumQuads - 1);
		} else {
			assert(false); // Unknown logic op.
		}
	} else if (type == ET_UnaryOp) {
		// <op> expr or <op> lvalue
		ASTNodeExpressionOp* node = (ASTNodeExpressionOp*)expression;
		if (node->m_Left->m_Type == AST_Expression) {
			// <op> expr, where <op> can be UMINUS or NOT

			// Create a temp variable
			ice = icodeCreateExpression_TempVariable(symScope);
			
			// Evaluate expression recursively.
			ICodeExpression* leftExpression = compileExpression(ic, (ASTNodeExpression*)node->m_Left, symScope);
			leftExpression = icodeEmitIfTableItem(ic, leftExpression, symScope);

			// Emit an unary op quad storing the result into the temp variable.
			if (node->m_Op == UnaryOp_Minus) {
				icodeEmitQuad(ic, ICOT_UnaryMinus, ice, leftExpression, 0, UNUSED_LABEL, expression->m_LineNumber);
			} else if (node->m_Op == UnaryOp_Not) {
				icodeEmitQuad(ic, ICOT_Not, ice, leftExpression, 0, UNUSED_LABEL, expression->m_LineNumber);
			} else {
				assert(false); // Unknown unary expression.
			}
		} else if (node->m_Left->m_Type == AST_LValue) {
			// <op> lvalue, where <op> can be pre or post ++/--
			// Evaluate lvalue.
			ICodeExpression* leftExpression = compileLValue(ic, (ASTNodeLValue*)node->m_Left, symScope);

			if (node->m_Op == UnaryOp_PreDec || node->m_Op == UnaryOp_PreInc) {
				// It's a pre-op. This means that the result of the expression is the lvalue AFTER the operator
				// is applied to it.

				// NOTE: icodeEmitIfTableItem(leftExpression) inlined to simplify logic.
				if (leftExpression->m_Type != ICET_TableItem) {
					// This is a variable. Emit a quad for the operation and return the result to the caller.
					// OP lvalue, lvalue, 1.
					if (node->m_Op == UnaryOp_PreDec) {
						icodeEmitQuad(ic, ICOT_Sub, leftExpression, leftExpression, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					} else {
						icodeEmitQuad(ic, ICOT_Add, leftExpression, leftExpression, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					}

					ice = leftExpression;
				} else {
					// This is a table item (i.e. t.x++).
					// Create a temp variable to hold the current value of the table item.
					ICodeExpression* tableItem = icodeCreateExpression_TempVariable(symScope);

					// Get the current value of the table item.
					icodeEmitQuad(ic, ICOT_TableGetElement, tableItem, leftExpression, leftExpression->m_Index, UNUSED_LABEL, expression->m_LineNumber);

					// Apply the operation to the temp value.
					if (node->m_Op == UnaryOp_PreDec) {
						icodeEmitQuad(ic, ICOT_Sub, tableItem, tableItem, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					} else {
						icodeEmitQuad(ic, ICOT_Add, tableItem, tableItem, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					}

					// Set the new value back to the table.
					icodeEmitQuad(ic, ICOT_TableSetElement, leftExpression, leftExpression->m_Index, tableItem, UNUSED_LABEL, expression->m_LineNumber);

					// Return the temp value as the result of this expression.
					ice = tableItem;
				}
			} else if (node->m_Op == UnaryOp_PostDec || node->m_Op == UnaryOp_PostInc) {
				// This is a post op expression.
				// This means that the result of this expression will be the lvalue BEFORE the operator
				// is applied to it.
				// Create a temp variable to hold the old value.
				ice = icodeCreateExpression_TempVariable(symScope);

				// NOTE: icodeEmitIfTableItem(leftExpression) inlined to simplify logic.
				if (leftExpression->m_Type != ICET_TableItem) {
					// This is a regular variable.
					// Get the current variable value into the temp.
					icodeEmitQuad(ic, ICOT_Assign, ice, leftExpression, 0, UNUSED_LABEL, expression->m_LineNumber);

					// Apply the operation on the variable
					if (node->m_Op == UnaryOp_PreDec) {
						icodeEmitQuad(ic, ICOT_Sub, leftExpression, leftExpression, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					} else {
						icodeEmitQuad(ic, ICOT_Add, leftExpression, leftExpression, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					}
				} else {
					// This is a table item.
					// Get the current table item value into another temp variable.
					ICodeExpression* tableItem = icodeCreateExpression_TempVariable(symScope);
					icodeEmitQuad(ic, ICOT_TableGetElement, tableItem, leftExpression, leftExpression->m_Index, UNUSED_LABEL, expression->m_LineNumber);

					// Assign the current table item value to the result of this expression
					icodeEmitQuad(ic, ICOT_Assign, ice, tableItem, 0, UNUSED_LABEL, expression->m_LineNumber);

					// Apply the operation on the new temp variable
					if (node->m_Op == UnaryOp_PreDec) {
						icodeEmitQuad(ic, ICOT_Sub, tableItem, tableItem, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					} else {
						icodeEmitQuad(ic, ICOT_Add, tableItem, tableItem, icodeCreateExpression_NumConst(1), UNUSED_LABEL, expression->m_LineNumber);
					}

					// Write the result of the operation back to the table.
					icodeEmitQuad(ic, ICOT_TableSetElement, leftExpression, leftExpression->m_Index, tableItem, UNUSED_LABEL, expression->m_LineNumber);
				}
			} else {
				assert(false); // Unknown unary expression
			}
		}
	} else if (type == ET_Nested) {
		// (expr)
		// Compile recursively.
		ASTNodeExpressionNested* node = (ASTNodeExpressionNested*)expression;
		ice = compileExpression(ic, (ASTNodeExpression*)node->m_Expr, symScope);
	} else if (type == ET_Const) {
		// Constant expression
		ASTNodeExpressionConst* node = (ASTNodeExpressionConst*)expression;
		ice = compileExpression_Const(node);
	} else if (type == ET_ObjectDefinition) {
		// Object definition
		ASTNodeExpressionObjDef* node = (ASTNodeExpressionObjDef*)expression;
		ice = compileExpression_ObjectDefinition(ic, node, symScope);
	} else if (type == ET_FunctionDefinition) {
		// Function definition
		ASTNodeExpressionFuncDef* node = (ASTNodeExpressionFuncDef*)expression;
		assert(node->m_FuncDef->m_Type == AST_FunctionDefinition);

		ASTNodeFuncDef* funcDef = (ASTNodeFuncDef*)node->m_FuncDef;
		compileFunctionDefinition(ic, funcDef);

		// Get the function address/symbol into a variable as the result of this expression.
		ice = icodeCreateExpression_Variable(funcDef->m_Symbol);
	} else if (type == ET_LValue) {
		// lvalue
		ASTNodeExpressionLValue* node = (ASTNodeExpressionLValue*)expression;
		ice = compileLValue(ic, (ASTNodeLValue*)node->m_LValue, symScope);

		// If this is a TableItem expression, get the value of the table item into the temp variable.
		ice = icodeEmitIfTableItem(ic, ice, symScope);
	} else if (type == ET_Call) {
		// call
		ASTNodeExpressionCall* node = (ASTNodeExpressionCall*)expression;
		assert(node->m_Call->m_Type == AST_Call);
		
		// Compile the call (NOTE: compileCall doesn't return an expression; the result of the call
		// is retrieved using GETRETVAL icode op.
		ASTNodeCall* call = (ASTNodeCall*)node->m_Call;
		compileCall(ic, call, symScope);

		// Get the result of the call into a temp variable using GETRETVAL.
		ice = icodeCreateExpression_TempVariable(symScope);
		icodeEmitQuad(ic, ICOT_GetRetValue, ice, 0, 0, UNUSED_LABEL, expression->m_LineNumber);
	} else {
		assert(false); // Unknown expression type.
	}

	return ice;
}

void compileFunctionDefinition(ICode* ic, ASTNodeFuncDef* funcDef)
{
	// Keep function address in the symbol (symbol's offset will always be ~0u for functions at this point).
	assert(funcDef->m_Symbol->m_Offset == ~0u);
	funcDef->m_Symbol->m_Offset = ic->m_NumQuads; // TODO: Should the address be the FUNCSTART quad index or the first function quad index?

	icodeEmitQuad(ic, ICOT_FunctionStart, 0, icodeCreateExpression_ProgramFunction(funcDef), 0, UNUSED_LABEL, funcDef->m_LineNumber);

	LinkedListNode* bodyListNode = funcDef->m_Body->m_Head;
	while (bodyListNode) {
		ASTNode* bodyNode = (ASTNode*)bodyListNode->m_Item;
		assert(bodyNode->m_Type == AST_Statement);

		compileStatement(ic, (ASTNodeStatement*)bodyNode);

		bodyListNode = bodyListNode->m_Next;
	}

	// Implicit return at the end of the function in case the programmer forgot to add a return. Otherwise it's dead code
	// and will be stripped at optimization phase.
	icodeEmitQuad(ic, ICOT_Return, 0, 0, 0, UNUSED_LABEL, funcDef->m_LineNumber);
	icodeEmitQuad(ic, ICOT_FunctionEnd, 0, icodeCreateExpression_ProgramFunction(funcDef), 0, UNUSED_LABEL, funcDef->m_LineNumber);
}

// Compile an AST call node into quads.
void compileCall(ICode* ic, ASTNodeCall* call, Scope* symScope)
{
	// Step 1: Get the function to be called into a temporary variable.
	ICodeExpression* funcExpr = 0;
	ICodeExpression* thisExpr = 0;
	CallType type = call->m_CallType;
	if (type == Call_Normal) {
		// f() or table.f()
		// Compile the lvalue
		funcExpr = compileLValue(ic, (ASTNodeLValue*)call->m_Func, symScope);

		// If this is a table item expression, emit a TABLEGETELEM
		funcExpr = icodeEmitIfTableItem(ic, funcExpr, symScope);
	} else if (type == Call_Method) {
		// table..f()
		ASTNodeCallMethod* node = (ASTNodeCallMethod*)call;

		// Compile the table lvalue.
		ICodeExpression* objExpr = compileLValue(ic, (ASTNodeLValue*)node->m_Func, symScope);
		ICodeExpression* lvalueExpr = icodeEmitIfTableItem(ic, objExpr, symScope);

		// Emit a TABLEGETELEM to get the function into a temporary variable. 
		funcExpr = icodeCreateExpression_TempVariable(symScope);
		icodeEmitQuad(ic, ICOT_TableGetElement, funcExpr, lvalueExpr, icodeCreateExpression_StringConst(node->m_Identifier->m_Name), UNUSED_LABEL, call->m_LineNumber);

		// NOTE: This is the only case which generates a 'this' "pointer"
		thisExpr = lvalueExpr;
	} else if (type == Call_ReturnValue) {
		// f()()
		// Recursively compile the call.
		compileCall(ic, (ASTNodeCall*)call->m_Func, symScope);

		// The return value of the call into a temp variable (this will be the function to call below in Step 2).
		funcExpr = icodeCreateExpression_TempVariable(symScope);
		icodeEmitQuad(ic, ICOT_GetRetValue, funcExpr, 0, 0, UNUSED_LABEL, call->m_LineNumber);
	} else if (type == Call_Lambda) {
		// (function () { ... })()
		// Compile the function definition (will most probably have an auto-generated name).
		ASTNodeFuncDef* funcDef = (ASTNodeFuncDef*)call->m_Func;
		compileFunctionDefinition(ic, funcDef);

		// The function to be called below is the symbol generated by the function definition.
		funcExpr = icodeCreateExpression_Variable(funcDef->m_Symbol);
	}

	// Step 2: Evaluate all argument expressions and add them to the linked list.
	LinkedList* argExprList = llCreate();
	LinkedListNode* argListNode = call->m_Args->m_Head;
	while (argListNode) {
		ASTNode* argNode = (ASTNode*)argListNode->m_Item;
		assert(argNode->m_Type == AST_Expression);

		ICodeExpression* argExpr = compileExpression(ic, (ASTNodeExpression*)argNode, symScope);
		llPushBack(argExprList, argExpr);

		argListNode = argListNode->m_Next;
	}

	// Emit a PARAM quad for each argument in reverse order.
	argListNode = argExprList->m_Tail;
	while (argListNode) {
		ICodeExpression* argExpr = (ICodeExpression*)argListNode->m_Item;
		icodeEmitQuad(ic, ICOT_Param, 0, argExpr, 0, UNUSED_LABEL, call->m_LineNumber);

		argListNode = argListNode->m_Prev;
	}

	// Emit "this" last if needed.
	if (thisExpr) {
		icodeEmitQuad(ic, ICOT_Param, 0, thisExpr, 0, UNUSED_LABEL, call->m_LineNumber);
	}
	
	// Emit CALL
	icodeEmitQuad(ic, ICOT_Call, 0, funcExpr, 0, UNUSED_LABEL, call->m_LineNumber);

	llDestroy(argExprList);
}

// Compile a single statement into quads.
// NOTE: This is the entry point to the compilation. A Program is composed of Statements
// and each statement is compiled using this function (see icodeCompileProgram)
void compileStatement(ICode* ic, ASTNodeStatement* statement)
{
	// Reset the temporary variable counter for the current scope (each statement stores
	// the scope it appears.
	stResetTempVariableCounter(statement->m_Scope);

	StatementType type = statement->m_StatementType;
	if (type == ST_Expression) {
		// statement: expression 
		ASTNodeStatementExpression* node = (ASTNodeStatementExpression*)statement;
		assert(node->m_Expr->m_Type == AST_Expression);
		compileExpression(ic, (ASTNodeExpression*)node->m_Expr, statement->m_Scope);
	} else if (type == ST_If) {
		// ... ; conditional expression quads
		// IF_EQ cond, TRUE, IF_BODY
		// JUMP ELSE_BLOCK
		// IF_BODY: 
		// ... ; body statements
		// JUMP IF_END
		// ELSE_BLOCK:
		// ... ; else body statements
		// IF_END: ...
		ASTNodeStatementIf* node = (ASTNodeStatementIf*)statement;
		assert(node->m_Cond->m_Type == AST_Expression);
		assert(node->m_Then->m_Type == AST_Statement);

		const unsigned int firstQuad = ic->m_NumQuads;

		// Compile the condition expression.
		ICodeExpression* cond = compileExpression(ic, (ASTNodeExpression*)node->m_Cond, statement->m_Scope);

		// IF_EQ condition, TRUE, IF_BODY
		icodeEmitQuad(ic, ICOT_IfEqual, 0, cond, icodeCreateExpression_BoolConst(true), ic->m_NumQuads + 2, statement->m_LineNumber);
		
		// JUMP FALSE_LABEL
		icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, TEMP_FALSE_LABEL, statement->m_LineNumber);

		// IF_BODY: ...
		// Compile the if body statement (will most probably be a block statement.
		compileStatement(ic, (ASTNodeStatement*)node->m_Then);
		
		// If there's an else block, compile it.
		if (node->m_Else) {
			// Emit a jump at the end of the else block (this is the last instruction executed for the then block above).
			icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, TEMP_TRUE_LABEL, statement->m_LineNumber);

			// Backpatch the FALSE_LABEL up to this point.
			icodeBackpatchLabel(ic, firstQuad, TEMP_FALSE_LABEL, ic->m_NumQuads);

			// FALSE_LABEL: ...
			assert(node->m_Else->m_Type == AST_Statement);
			compileStatement(ic, (ASTNodeStatement*)node->m_Else);

			// TRUE_LABEL: ...
			icodeBackpatchLabel(ic, firstQuad, TEMP_TRUE_LABEL, ic->m_NumQuads);
		} else {
			// FALSE_LABEL: ...
			icodeBackpatchLabel(ic, firstQuad, TEMP_FALSE_LABEL, ic->m_NumQuads);
		}
	} else if (type == ST_While) {
		// LOOP_CONTINUE:
		// ... ; conditional expression quads
		// IF_NEQ cond, TRUE, LOOP_END
		// ... ; body statements 
		// JUMP LOOP_CONTINUE
		// LOO_END: ... 
		ASTNodeStatementWhile* node = (ASTNodeStatementWhile*)statement;
		assert(node->m_Cond->m_Type == AST_Expression);
		assert(node->m_Body->m_Type == AST_Statement);

		const unsigned int firstQuad = ic->m_NumQuads;

		// Compile the conditional expression.
		ICodeExpression* cond = compileExpression(ic, (ASTNodeExpression*)node->m_Cond, statement->m_Scope);

		// If the condition isn't TRUE, jump to the end of the loop.
		icodeEmitQuad(ic, ICOT_IfNotEqual, 0, cond, icodeCreateExpression_BoolConst(true), TEMP_LOOP_END_LABEL, statement->m_LineNumber);

		// Compile the loop body.
		compileStatement(ic, (ASTNodeStatement*)node->m_Body);

		// Restart the loop.
		icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, firstQuad, statement->m_LineNumber); // NOTE: This could have been TEMP_LOOP_CONTINUE_LABEL but at this point we know the actual label number.

		// Backpatch labels. The LOOP_END label is the current quad number, and the CONTINUE_LABEL is the first quad number.
		icodeBackpatchLabel(ic, firstQuad, TEMP_LOOP_END_LABEL, ic->m_NumQuads); // This is needed for both the comparison above and the "break" statements.
		icodeBackpatchLabel(ic, firstQuad, TEMP_LOOP_CONTINUE_LABEL, firstQuad); // This is needed for the "continue" statements.
	} else if (type == ST_For) {
		// ... ; initilization expresssion quads
		// LOOP_START: 
		// ... ; conditional expression quads
		// IF_NEQ cond, TRUE, LOOP_END
		// ... ; loop body statements
		// CONTINUE_LOOP: ; this is where the loop will end normally OR when a "continue" statement is encountered.
		// ... ; iterator expressions
		// JUMP LOOP_START
		// LOOP_END: ...
		ASTNodeStatementFor* node = (ASTNodeStatementFor*)statement;
		assert(node->m_Cond->m_Type == AST_Expression);
		assert(node->m_Body->m_Type == AST_Statement);

		// Compile loop initialization expressions...
		LinkedListNode* initListNode = node->m_Init->m_Head;
		while (initListNode) {
			ASTNode* initNode = (ASTNode*)initListNode->m_Item;
			assert(initNode->m_Type == AST_Expression);

			compileExpression(ic, (ASTNodeExpression*)initNode, statement->m_Scope);

			initListNode = initListNode->m_Next;
		}

		const unsigned int firstQuad = ic->m_NumQuads;

		// Compile conditional expression
		ICodeExpression* cond = compileExpression(ic, (ASTNodeExpression*)node->m_Cond, statement->m_Scope);

		// If the condition isn't true jump to the end of the loop.
		icodeEmitQuad(ic, ICOT_IfNotEqual, 0, cond, icodeCreateExpression_BoolConst(true), TEMP_LOOP_END_LABEL, statement->m_LineNumber);
		
		// Compile body.
		compileStatement(ic, (ASTNodeStatement*)node->m_Body);

		const unsigned int iterQuad = ic->m_NumQuads;

		// Compile iterator expressions...
		LinkedListNode* iterListNode = node->m_Iter->m_Head;
		while (iterListNode) {
			ASTNode* iterNode = (ASTNode*)iterListNode->m_Item;
			assert(iterNode->m_Type == AST_Expression);

			compileExpression(ic, (ASTNodeExpression*)iterNode, statement->m_Scope);

			iterListNode = iterListNode->m_Next;
		}

		// Jump to start of loop
		icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, firstQuad, statement->m_LineNumber);

		// Backpatch temp labels.
		icodeBackpatchLabel(ic, firstQuad, TEMP_LOOP_END_LABEL, ic->m_NumQuads);
		icodeBackpatchLabel(ic, firstQuad, TEMP_LOOP_CONTINUE_LABEL, iterQuad);
	} else if (type == ST_Return) {
		ASTNodeStatementReturn* node = (ASTNodeStatementReturn*)statement;
		if (node->m_Expr) {
			assert(node->m_Expr->m_Type == AST_Expression);
			ICodeExpression* ret = compileExpression(ic, (ASTNodeExpression*)node->m_Expr, statement->m_Scope);
			icodeEmitQuad(ic, ICOT_Return, ret, 0, 0, UNUSED_LABEL, statement->m_LineNumber);
		} else {
			icodeEmitQuad(ic, ICOT_Return, 0, 0, 0, UNUSED_LABEL, statement->m_LineNumber);
		}
	} else if (type == ST_Block) {
		ASTNodeStatementBlock* node = (ASTNodeStatementBlock*)statement;
		LinkedListNode* statementNode = node->m_Body->m_Head;
		while (statementNode) {
			ASTNodeStatement* statement = (ASTNodeStatement*)statementNode->m_Item;

			compileStatement(ic, statement);

			statementNode = statementNode->m_Next;
		}
	} else if (type == ST_FuncDef) {
		// Compile the function definition.
		ASTNodeStatementFuncDef* node = (ASTNodeStatementFuncDef*)statement;
		assert(node->m_FuncDef->m_Type == AST_FunctionDefinition);
		compileFunctionDefinition(ic, (ASTNodeFuncDef*)node->m_FuncDef);
	} else if (type == ST_Break) {
		// Emit a JUMP LOOP_END (will be backpatched by the loop).
		icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, TEMP_LOOP_END_LABEL, statement->m_LineNumber);
	} else if (type == ST_Continue) {
		// Emit a JUMP LOOP_CONTINUE (will be backpatched by the loop).
		icodeEmitQuad(ic, ICOT_Jump, 0, 0, 0, TEMP_LOOP_CONTINUE_LABEL, statement->m_LineNumber);
	} else if (type == ST_Empty) {
		// Nothing to do here.
	} else {
		assert(false); // Unknown statement type.
	}
}

//////////////////////////////////////////////////////////////////////////
// Public interface...
//
ICode* icodeCompileProgram(Program* prog)
{
	ICode* ic = (ICode*)malloc(sizeof(ICode));
	ic->m_Quads = 0;
	ic->m_NumQuads = 0;
	ic->m_QuadCapacity = 0;

	LinkedListNode* statementNode = prog->m_StatementList->m_Head;
	while (statementNode) {
		ASTNodeStatement* statement = (ASTNodeStatement*)statementNode->m_Item;

		compileStatement(ic, statement);

		statementNode = statementNode->m_Next;
	}

	return ic;
}

void icodeDestroy(ICode* ic)
{
	// TODO: 
}

void icodePrintQuads(ICode* ic)
{
	const unsigned int numQuads = ic->m_NumQuads;
	for (unsigned int i = 0; i < numQuads; ++i) {
		ICodeQuad* q = &ic->m_Quads[i];

		fprintf(stdout, "%d: ", i);

		switch (q->m_Op) {
		case ICOT_Assign:
		{
			fprintf(stdout, "ASSIGN ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Result);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_Add:
		case ICOT_Sub:
		case ICOT_Mul:
		case ICOT_Div:
		case ICOT_Mod:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Arg2);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Result);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_Not:
		case ICOT_UnaryMinus:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Result);
			fprintf(stdout, "\n");
		}
		break;
//		case ICOT_And:
//		case ICOT_Or:
		case ICOT_IfEqual:
		case ICOT_IfNotEqual:
		case ICOT_IfLessEqual:
		case ICOT_IfGreaterEqual:
		case ICOT_IfLess:
		case ICOT_IfGreater:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Arg2);
			fprintf(stdout, ", #%d\n", q->m_Label);
		}
		break;
		case ICOT_Jump:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " #%d\n", q->m_Label);
		}
		break;
		case ICOT_Call:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_Param:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_GetRetValue:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Result);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_Return:
		{
			icodePrintOp(q->m_Op);
			if (q->m_Result) {
				fprintf(stdout, " ");
				icodePrintExpression(q->m_Result);
			}
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_FunctionStart:
		{
			assert(q->m_Arg1->m_FuncScope);
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, " (args: %d, locals: %d)\n", stGetNumScopeArguments(q->m_Arg1->m_FuncScope), stGetNumScopeLocals(q->m_Arg1->m_FuncScope));
		}
		break;
		case ICOT_FunctionEnd:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_TableCreate:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Result);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_TableGetElement:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Arg2);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Result);
			fprintf(stdout, "\n");
		}
		break;
		case ICOT_TableSetElement:
		{
			icodePrintOp(q->m_Op);
			fprintf(stdout, " ");
			icodePrintExpression(q->m_Result);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Arg1);
			fprintf(stdout, ", ");
			icodePrintExpression(q->m_Arg2);
			fprintf(stdout, "\n");
		}
		break;
		default:
			assert(false);
			break;
		}
	}
}
