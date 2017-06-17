#include "tcode.h"
#include "icode.h"
#include "symbol_table.h"
#include <malloc.h> // malloc()/free()
#include <memory.h> // memset()
#include <string.h> // strcmp()
#include <stdio.h> // fprintf()
#include <assert.h> // assert()

#define LABEL_IS_ICODE       0x80000000
#define TEMP_FUNCEND_LABEL   0x40000000

unsigned int findFuncEnd(const ICode* ic, unsigned int firstQuad)
{
	unsigned int depth = 0;

	const unsigned int n = ic->m_NumQuads;
	for (unsigned int i = firstQuad; i < n; ++i) {
		const ICodeQuad* q = &ic->m_Quads[i];
		if (q->m_Op == ICOT_FunctionEnd) {
			if (depth == 0) {
				return i;
			} else {
				--depth;
			}
		} else if (q->m_Op == ICOT_FunctionStart) {
			++depth;
		}
	}

	return ~0u;
}

void patchClosestTCodeLabel(TCode* tc, unsigned int tempLabel, unsigned int trueLabel)
{
	assert(tc->m_NumInstructions > 0);
	bool instructionFound = false; // Sanity check;
	int i = (int)tc->m_NumInstructions - 1;
	while (i-- > 0) {
		TCodeInstruction* instr = &tc->m_Instructions[i];

		// Is this a jump instruction?
		if (instr->m_Op >= TCOT_Jump && instr->m_Op <= TCOT_JumpGreater) {
			// All jump instructions store the target label in the result operand.
			assert(instr->m_Result.m_Type == TCOT_Label);
			if (instr->m_Result.m_Value == tempLabel) {
				instr->m_Result.m_Value = trueLabel;
				instructionFound = true;
				break;
			}
		}
	}

	assert(instructionFound == true);
}

char* escapeString(const char* str)
{
	unsigned int len = (unsigned int)strlen(str);
	char* escapedString = (char*)malloc(len + 1);

	char* dst = escapedString;
	const char* src = str;
	while (*src) {
		if (*src == '\\') {
			if (*(src + 1) == 'n') {
				*dst++ = '\n';
				src++;
			} else if (*(src + 1) == 't') {
				*dst++ = '\t';
				src++;
			} else if (*(src + 1) == 'r') {
				*dst++ = '\r';
				src++;
			}
		} else {
			*dst++ = *src;
		}
		src++;
	}

	*dst = 0;

	return escapedString;
}

unsigned int tcodeInsertConst_Num(TCode* tc, double num)
{
	// Check if the number is already in the table.
	const unsigned int n = tc->m_NumConstNumbers;
	for (unsigned int i = 0; i < n; ++i) {
		if (tc->m_ConstNumTable[i] == num) {
			// Found it.
			return i;
		}
	}

	// The number isn't in the table. Grow the table if necessary.
	if (tc->m_NumConstNumbers + 1 > tc->m_ConstNumTableCapacity) {
		tc->m_ConstNumTableCapacity += 256;
		tc->m_ConstNumTable = (double*)realloc(tc->m_ConstNumTable, sizeof(double) * tc->m_ConstNumTableCapacity);
	}

	// Insert the new constant.
	tc->m_ConstNumTable[tc->m_NumConstNumbers++] = num;

	return tc->m_NumConstNumbers - 1;
}

unsigned int tcodeInsertConst_String(TCode* tc, const char* str)
{
	// Check if the string is already in the table.
	const unsigned int n = tc->m_NumConstStrings;
	for (unsigned int i = 0; i < n; ++i) {
		if (!strcmp(tc->m_ConstStringTable[i], str)) {
			// Found it.
			return i;
		}
	}

	// The string isn't in the table. Grow the table if necessary.
	if (tc->m_NumConstStrings + 1 > tc->m_ConstStringTableCapacity) {
		tc->m_ConstStringTableCapacity += 256;
		tc->m_ConstStringTable = (char**)realloc(tc->m_ConstStringTable, sizeof(char*) * tc->m_ConstStringTableCapacity);
	}

	// Insert the new constant.
	tc->m_ConstStringTable[tc->m_NumConstStrings++] = strdup(str);

	return tc->m_NumConstStrings - 1;
}

unsigned int tcodeInsertUserFunction(TCode* tc, Symbol* sym)
{
	// Check if the function is already in the table.
	const unsigned int n = tc->m_NumUserFunctions;
	for (unsigned int i = 0; i < n; ++i) {
		TCodeUserFunction* userFunc = &tc->m_UserFunctionTable[i];
		if (userFunc->m_ICodeQuadID == sym->m_Offset) {
			assert(userFunc->m_Symbol != 0);
			assert(!strcmp(userFunc->m_Symbol->m_Identifier, sym->m_Identifier));

			// Found it.
			return i;
		}
	}

	// The function isn't in the table. Grow the table if necessary.
	if (tc->m_NumUserFunctions + 1 > tc->m_UserFunctionTableCapacity) {
		tc->m_UserFunctionTableCapacity += 32;
		tc->m_UserFunctionTable = (TCodeUserFunction*)realloc(tc->m_UserFunctionTable, sizeof(TCodeUserFunction) * tc->m_UserFunctionTableCapacity);
	}

	// Insert the new constant.
	TCodeUserFunction* func = &tc->m_UserFunctionTable[tc->m_NumUserFunctions++];
	func->m_ICodeQuadID = sym->m_Offset;
	func->m_Symbol = sym;
	func->m_TCodeOffset = ~0u;
	func->m_NumLocals = ~0u;

	return tc->m_NumUserFunctions - 1;
}

unsigned int tcodeInsertLibraryFunction(TCode* tc, const char* name)
{
	// Check if the function is already in the table.
	const unsigned int n = tc->m_NumLibFunctions;
	for (unsigned int i = 0; i < n; ++i) {
		if (!strcmp(tc->m_LibFunctionTable[i], name)) {
			// Found it.
			return i;
		}
	}

	// The function isn't in the table. Grow the table if necessary.
	if (tc->m_NumLibFunctions + 1 > tc->m_LibFunctionTableCapacity) {
		tc->m_LibFunctionTableCapacity += 32;
		tc->m_LibFunctionTable = (char**)realloc(tc->m_LibFunctionTable, sizeof(char*) * tc->m_LibFunctionTableCapacity);
	}

	// Insert the new constant.
	tc->m_LibFunctionTable[tc->m_NumLibFunctions++] = strdup(name);

	return tc->m_NumLibFunctions - 1;
}

void tcodeOperandLabel(TCodeOperand* op, unsigned int label, bool isICodeLabel)
{
	op->m_Type = TCOT_Label;
	op->m_Value = label | (isICodeLabel ? LABEL_IS_ICODE : 0); // Mark the label as an i-code label in order to patch it later
}

void tcodeOperandConstNum(TCode* tc, TCodeOperand* op, double num)
{
	op->m_Type = TCOT_ConstNum;
	op->m_Value = tcodeInsertConst_Num(tc, num);
}

void tcodeOperandRetVal(TCodeOperand* op)
{
	op->m_Type = TCOT_RetVal;
	op->m_Value = ~0u; // Not used.
}

void tcodeOperandFromExpression(TCode* tc, TCodeOperand* op, const ICodeExpression* expr)
{
	ICodeExpressionType type = expr->m_Type;

	if (type == ICET_Variable || 
		type == ICET_TableItem) 
	{
		const Symbol* sym = expr->m_Symbol;
		assert(sym != 0);

		op->m_Value = sym->m_Offset;

		if (sym->m_Type == SymType_FunctionArgument) {
			op->m_Type = TCOT_FunctionArgument;
		} else {
			assert(sym->m_Type == SymType_Variable || sym->m_Type == SymType_TempVariable);
			assert(sym->m_ParentScope);
			if (sym->m_ParentScope->m_ID == 0) {
				op->m_Type = TCOT_GlobalVariable;
			} else {
				op->m_Type = TCOT_LocalVariable;
			}
		}
	} else if (type == ICET_ProgramFunction) {
		assert(expr->m_Symbol != 0);

		op->m_Type = TCOT_UserFunction;
		op->m_Value = tcodeInsertUserFunction(tc, expr->m_Symbol);
	} else if (type == ICET_LibraryFunction) {
		assert(expr->m_Symbol != 0);

		op->m_Type = TCOT_LibraryFunction;
		op->m_Value = tcodeInsertLibraryFunction(tc, expr->m_Symbol->m_Identifier);
	} else if (type == ICET_ConstNum) {
		tcodeOperandConstNum(tc, op, expr->m_NumConst);
	} else if (type == ICET_ConstBool) {
		op->m_Type = TCOT_ConstBool;
		op->m_Value = expr->m_BoolConst;
	} else if (type == ICET_ConstString) {
		op->m_Type = TCOT_ConstString;
		op->m_Value = tcodeInsertConst_String(tc, expr->m_StrConst);
	} else if (type == ICET_Nil) {
		op->m_Type = TCOT_Nil;
		op->m_Value = 0;
	} else {
		assert(false); // Unknown expression type.
	}
}

void tcodeEmitInstruction(TCode* tc, const TCodeInstruction* instr)
{
	if (tc->m_NumInstructions + 1 > tc->m_InstructionCapacity) {
		tc->m_InstructionCapacity += 64;
		tc->m_Instructions = (TCodeInstruction*)realloc(tc->m_Instructions, sizeof(TCodeInstruction) * tc->m_InstructionCapacity);
	}

	memcpy(&tc->m_Instructions[tc->m_NumInstructions], instr, sizeof(TCodeInstruction));
	++tc->m_NumInstructions;
}

void tcodeGenerate_Assign(TCode* tc, const ICodeExpression* res, const ICodeExpression* arg, unsigned int srcLine)
{
	// ASSIGN v/i, v
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_Assign;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Arg1, arg);
	tcodeOperandFromExpression(tc, &instr.m_Result, res);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_UnaryMinus(TCode* tc, const ICodeExpression* res, const ICodeExpression* arg, unsigned int srcLine)
{
	// UMINUS v/i, v
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_Mul;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Arg1, arg);
	tcodeOperandConstNum(tc, &instr.m_Arg2, -1.0);
	tcodeOperandFromExpression(tc, &instr.m_Result, res);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_BinaryArithOp(TCode* tc, TCodeOperationType op, const ICodeExpression* res, const ICodeExpression* arg1, const ICodeExpression* arg2, unsigned int srcLine)
{
	// ADD/SUB/MUL/DIV/MOD v/i, v/i, v
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = op;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Arg1, arg1);
	tcodeOperandFromExpression(tc, &instr.m_Arg2, arg2);
	tcodeOperandFromExpression(tc, &instr.m_Result, res);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_Jmp(TCode* tc, unsigned int label, bool isICodeLabel, unsigned int srcLine)
{
	// JUMP i
	// If the label is an i-code label, check if we already have the corresponding t-code label (i.e. backward jump)
	if (isICodeLabel && tc->m_FirstQuadInstrID[label] != ~0u) {
		label = tc->m_FirstQuadInstrID[label];
		isICodeLabel = false;
	}

	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_Jump;
	instr.m_SrcLine = srcLine;
	tcodeOperandLabel(&instr.m_Result, label, isICodeLabel); // Store the jump target into the instruction result.
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_Jcc(TCode* tc, TCodeOperationType op, const ICodeExpression* arg1, const ICodeExpression* arg2, unsigned int label, unsigned int srcLine)
{
	// IF_cc v/i, v/i, label

	// The label is always an i-code label at this point. Check if the corresponding t-code label is known.
	bool isICodeLabel = true;
	if (tc->m_FirstQuadInstrID[label] != ~0u) {
		label = tc->m_FirstQuadInstrID[label];
		isICodeLabel = false;
	}

	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = op;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Arg1, arg1);
	tcodeOperandFromExpression(tc, &instr.m_Arg2, arg2);
	tcodeOperandLabel(&instr.m_Result, label, isICodeLabel); // Store the jump target into the instruction result.
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_Call(TCode* tc, const ICodeExpression* funcExpr, unsigned int srcLine)
{
	// CALL v/f
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_Call;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Result, funcExpr);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_Param(TCode* tc, const ICodeExpression* arg1, unsigned int srcLine)
{
	// PARAM v/i
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_PushArg;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Arg1, arg1);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_SetRetVal(TCode* tc, const ICodeExpression* arg1, unsigned int srcLine)
{
	// RETURN v/i
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_Assign;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Arg1, arg1);
	tcodeOperandRetVal(&instr.m_Result);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_GetRetVal(TCode* tc, const ICodeExpression* res, unsigned int srcLine)
{
	// GETRETVAL v
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_Assign;
	instr.m_SrcLine = srcLine;
	tcodeOperandRetVal(&instr.m_Arg1);
	tcodeOperandFromExpression(tc, &instr.m_Result, res);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_FuncEnter(TCode* tc, const ICodeExpression* funcExpr, unsigned int srcLine)
{
	// FUNCSTART f
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_FuncEnter;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Result, funcExpr);
	tcodeEmitInstruction(tc, &instr);

	// Update function's t-code address
	assert(instr.m_Result.m_Type == TCOT_UserFunction);
	TCodeUserFunction* userFunc = &tc->m_UserFunctionTable[instr.m_Result.m_Value];
	assert(userFunc->m_TCodeOffset == ~0u);
	userFunc->m_TCodeOffset = tc->m_NumInstructions - 1;

	// Update functions's number of locals
	assert(funcExpr->m_FuncScope);
	userFunc->m_NumLocals = stGetNumScopeLocals(funcExpr->m_FuncScope);
}

void tcodeGenerate_FuncEnd(TCode* tc, const ICodeExpression* funcExpr, unsigned int srcLine)
{
	// FUNCEND f
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_FuncExit;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Result, funcExpr);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_TableCreate(TCode* tc, const ICodeExpression* expr, unsigned int srcLine)
{
	// TABLECREATE table
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_TableCreate;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Result, expr);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_TableGetElem(TCode* tc, const ICodeExpression* res, const ICodeExpression* table, const ICodeExpression* index, unsigned int srcLine)
{
	// TABLEGETELEM table, index, res
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_TableGetElement;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Result, res);
	tcodeOperandFromExpression(tc, &instr.m_Arg1, table);
	tcodeOperandFromExpression(tc, &instr.m_Arg2, index);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeGenerate_TableSetElem(TCode* tc, const ICodeExpression* table, const ICodeExpression* index, const ICodeExpression* arg, unsigned int srcLine)
{
	// TABLEGETELEM table, index, res
	TCodeInstruction instr;
	memset(&instr, 0, sizeof(TCodeInstruction));

	instr.m_Op = TCOT_TableSetElement;
	instr.m_SrcLine = srcLine;
	tcodeOperandFromExpression(tc, &instr.m_Result, table);
	tcodeOperandFromExpression(tc, &instr.m_Arg1, index);
	tcodeOperandFromExpression(tc, &instr.m_Arg2, arg);
	tcodeEmitInstruction(tc, &instr);
}

void tcodeCompileQuad(TCode* tc, const ICode* ic, unsigned int quadID)
{
	const ICodeQuad* quad = &ic->m_Quads[quadID];
	ICodeOperationType op = quad->m_Op;
	unsigned int srcLine = quad->m_Line;

	if (op == ICOT_Assign) {
		tcodeGenerate_Assign(tc, quad->m_Result, quad->m_Arg1, srcLine);
	} else if (op == ICOT_Add) {
		tcodeGenerate_BinaryArithOp(tc, TCOT_Add, quad->m_Result, quad->m_Arg1, quad->m_Arg2, srcLine);
	} else if (op == ICOT_Sub) {
		tcodeGenerate_BinaryArithOp(tc, TCOT_Sub, quad->m_Result, quad->m_Arg1, quad->m_Arg2, srcLine);
	} else if (op == ICOT_Mul) {
		tcodeGenerate_BinaryArithOp(tc, TCOT_Mul, quad->m_Result, quad->m_Arg1, quad->m_Arg2, srcLine);
	} else if (op == ICOT_Div) {
		tcodeGenerate_BinaryArithOp(tc, TCOT_Div, quad->m_Result, quad->m_Arg1, quad->m_Arg2, srcLine);
	} else if (op == ICOT_Mod) {
		tcodeGenerate_BinaryArithOp(tc, TCOT_Mod, quad->m_Result, quad->m_Arg1, quad->m_Arg2, srcLine);
	} else if (op == ICOT_UnaryMinus) {
		tcodeGenerate_UnaryMinus(tc, quad->m_Result, quad->m_Arg1, srcLine);
	} else if (op == ICOT_Jump) {
		tcodeGenerate_Jmp(tc, quad->m_Label, true, srcLine);
	} else if (op == ICOT_IfEqual) {
		tcodeGenerate_Jcc(tc, TCOT_JumpEqual, quad->m_Arg1, quad->m_Arg2, quad->m_Label, srcLine);
	} else if (op == ICOT_IfNotEqual) {
		tcodeGenerate_Jcc(tc, TCOT_JumpNotEqual, quad->m_Arg1, quad->m_Arg2, quad->m_Label, srcLine);
	} else if (op == ICOT_IfLessEqual) {
		tcodeGenerate_Jcc(tc, TCOT_JumpLessEqual, quad->m_Arg1, quad->m_Arg2, quad->m_Label, srcLine);
	} else if (op == ICOT_IfGreaterEqual) {
		tcodeGenerate_Jcc(tc, TCOT_JumpGreaterEqual, quad->m_Arg1, quad->m_Arg2, quad->m_Label, srcLine);
	} else if (op == ICOT_IfLess) {
		tcodeGenerate_Jcc(tc, TCOT_JumpLess, quad->m_Arg1, quad->m_Arg2, quad->m_Label, srcLine);
	} else if (op == ICOT_IfGreater) {
		tcodeGenerate_Jcc(tc, TCOT_JumpGreater, quad->m_Arg1, quad->m_Arg2, quad->m_Label, srcLine);
	} else if (op == ICOT_Call) {
		tcodeGenerate_Call(tc, quad->m_Arg1, srcLine);
	} else if (op == ICOT_Param) {
		tcodeGenerate_Param(tc, quad->m_Arg1, srcLine);
	} else if (op == ICOT_Return) {
		if (quad->m_Result) {
			tcodeGenerate_SetRetVal(tc, quad->m_Result, srcLine);
		}

		// Find the FUNCEND label matching the current FUNCSTART.
		unsigned int label = findFuncEnd(ic, quadID + 1);
		assert(label != ~0u);

		// Jump to that quad.
		tcodeGenerate_Jmp(tc, label, true, srcLine);
	} else if (op == ICOT_GetRetValue) {
		tcodeGenerate_GetRetVal(tc, quad->m_Result, srcLine);
	} else if (op == ICOT_FunctionStart) {
		// Jump over the function.
		tcodeGenerate_Jmp(tc, TEMP_FUNCEND_LABEL, false, srcLine);

		// Generate FUNCENTER
		tcodeGenerate_FuncEnter(tc, quad->m_Arg1, srcLine);
	} else if (op == ICOT_FunctionEnd) {
		tcodeGenerate_FuncEnd(tc, quad->m_Arg1, srcLine);

		// Patch the closest TEMP_FUNCEND_LABEL with the address of the current instruction.
		const unsigned int tcodeAddress = tc->m_NumInstructions;
		patchClosestTCodeLabel(tc, TEMP_FUNCEND_LABEL, tcodeAddress);
	} else if (op == ICOT_TableCreate) {
		tcodeGenerate_TableCreate(tc, quad->m_Result, srcLine);
	} else if (op == ICOT_TableGetElement) {
		tcodeGenerate_TableGetElem(tc, quad->m_Result, quad->m_Arg1, quad->m_Arg2, srcLine);
	} else if (op == ICOT_TableSetElement) {
		tcodeGenerate_TableSetElem(tc, quad->m_Result, quad->m_Arg1, quad->m_Arg2, srcLine);
	} else {
		assert(false); // Unknown icode operation
	}
}

TCode* tcodeCompileICode(const ICode* ic, const SymbolTable* symTable)
{
	TCode* tc = (TCode*)malloc(sizeof(TCode));
	memset(tc, 0, sizeof(TCode));

	// Initialize quad -> instruction map
	tc->m_FirstQuadInstrID = (unsigned int*)malloc(sizeof(unsigned int) * (ic->m_NumQuads + 1));

	const unsigned int numQuads = ic->m_NumQuads;
	for (unsigned int i = 0; i < numQuads; ++i) {
		tc->m_FirstQuadInstrID[i] = ~0u;
	}
	tc->m_FirstQuadInstrID[numQuads] = ~0u;

	tc->m_NumGlobals = stGetNumScopeLocals(symTable->m_GlobalScope);

	// Assemble quads into tcode instructions...
	for (unsigned int i = 0; i < numQuads; ++i) {
		tc->m_FirstQuadInstrID[i] = tc->m_NumInstructions;

		tcodeCompileQuad(tc, ic, i);
	}

	// Emit NOP as last instruction
	{
		TCodeInstruction nop;
		memset(&nop, 0, sizeof(TCodeInstruction));
		nop.m_Op = TCOT_Nop;
		tcodeEmitInstruction(tc, &nop);

		tc->m_FirstQuadInstrID[numQuads] = tc->m_NumInstructions - 1;
	}

	// Patch icode labels
	const unsigned int numInstructions = tc->m_NumInstructions;
	for (unsigned int i = 0; i < numInstructions; ++i) {
		TCodeInstruction* instr = &tc->m_Instructions[i];
		if (instr->m_Op >= TCOT_Jump && instr->m_Op <= TCOT_JumpGreater) {
			assert(instr->m_Result.m_Type == TCOT_Label);

			unsigned int lbl = instr->m_Result.m_Value;
			if ((lbl & LABEL_IS_ICODE) == LABEL_IS_ICODE) {
				unsigned int icodeLabel = lbl & (~LABEL_IS_ICODE);
				assert(icodeLabel <= numQuads);
				unsigned int tcodeLabel = tc->m_FirstQuadInstrID[icodeLabel];
				assert(tcodeLabel != ~0u);
				instr->m_Result.m_Value = tcodeLabel;
			}
		}
	}

	return tc;
}

void tcodeDestroy(TCode* tc)
{
	for (unsigned int i = 0; i < tc->m_NumConstStrings; ++i) {
		free(tc->m_ConstStringTable[i]);
	}
	free(tc->m_ConstStringTable);
	tc->m_ConstStringTable = 0;
	tc->m_ConstStringTableCapacity = 0;
	tc->m_NumConstStrings = 0;

	for (unsigned int i = 0; i < tc->m_NumLibFunctions; ++i) {
		free(tc->m_LibFunctionTable[i]);
	}
	free(tc->m_LibFunctionTable);
	tc->m_LibFunctionTable = 0;
	tc->m_LibFunctionTableCapacity = 0;
	tc->m_NumLibFunctions = 0;

	free(tc->m_Instructions);
	tc->m_Instructions = 0;
	tc->m_NumInstructions = 0;
	tc->m_InstructionCapacity = 0;

	free(tc->m_ConstNumTable);
	tc->m_ConstNumTable = 0;
	tc->m_ConstNumTableCapacity = 0;
	tc->m_NumConstNumbers = 0;

	// TODO: Free identifiers
	free(tc->m_UserFunctionTable);
	tc->m_UserFunctionTable = 0;
	tc->m_UserFunctionTableCapacity = 0;
	tc->m_NumUserFunctions = 0;

	free(tc->m_FirstQuadInstrID);
	tc->m_FirstQuadInstrID = 0;

	free(tc);
}

void tcodePrintOpcode(TCodeOperationType op)
{
	switch (op) {
	case TCOT_Assign:
		fprintf(stdout, "ASSIGN      ");
		break;
	case TCOT_Add:
		fprintf(stdout, "ADD         ");
		break;
	case TCOT_Sub:
		fprintf(stdout, "SUB         ");
		break;
	case TCOT_Mul:
		fprintf(stdout, "MUL         ");
		break;
	case TCOT_Div:
		fprintf(stdout, "DIV         ");
		break;
	case TCOT_Mod:
		fprintf(stdout, "MOD         ");
		break;
	case TCOT_Jump:
		fprintf(stdout, "JMP         ");
		break;
	case TCOT_JumpEqual:
		fprintf(stdout, "JEQ         ");
		break;
	case TCOT_JumpNotEqual:
		fprintf(stdout, "JNE         ");
		break;
	case TCOT_JumpLessEqual:
		fprintf(stdout, "JLE         ");
		break;
	case TCOT_JumpGreaterEqual:
		fprintf(stdout, "JGE         ");
		break;
	case TCOT_JumpLess:
		fprintf(stdout, "JLT         ");
		break;
	case TCOT_JumpGreater:
		fprintf(stdout, "JGT         ");
		break;
	case TCOT_Call:
		fprintf(stdout, "CALL        ");
		break;
	case TCOT_PushArg:
		fprintf(stdout, "PUSHARG     ");
		break;
	case TCOT_FuncEnter:
		fprintf(stdout, "ENTERFUNC   ");
		break;
	case TCOT_FuncExit:
		fprintf(stdout, "EXITFUNC    ");
		break;
	case TCOT_TableCreate:
		fprintf(stdout, "TABLECREATE ");
		break;
	case TCOT_TableGetElement:
		fprintf(stdout, "TABLEGETELEM");
		break;
	case TCOT_TableSetElement:
		fprintf(stdout, "TABLESETELEM");
		break;
	case TCOT_Nop:
		fprintf(stdout, "NOP");
		break;
	default:
		assert(false);
		break;
	}
}

void tcodePrintOperand(const TCodeOperand* op)
{
	switch (op->m_Type) {
	case TCOT_Label:
		fprintf(stdout, "LBL(%u)", op->m_Value);
		break;
	case TCOT_GlobalVariable:
		fprintf(stdout, "GBL[%u]", op->m_Value);
		break;
	case TCOT_FunctionArgument:
		fprintf(stdout, "ARG[%u]", op->m_Value);
		break;
	case TCOT_LocalVariable:
		fprintf(stdout, "LOC[%u]", op->m_Value);
		break;
	case TCOT_ConstNum:
		fprintf(stdout, "NUM[%u]", op->m_Value);
		break;
	case TCOT_ConstString:
		fprintf(stdout, "STR[%u]", op->m_Value);
		break;
	case TCOT_ConstBool:
		fprintf(stdout, "%s", op->m_Value == 0 ? "FALSE" : "TRUE");
		break;
	case TCOT_Nil:
		fprintf(stdout, "NIL");
		break;
	case TCOT_UserFunction:
		fprintf(stdout, "FUN[%u]", op->m_Value);
		break;
	case TCOT_LibraryFunction:
		fprintf(stdout, "LIB[%u]", op->m_Value);
		break;
	case TCOT_RetVal:
		fprintf(stdout, "RETVAL", op->m_Value);
		break;
	default:
		assert(false); // Unknown t-code operand type.
	}
}

void tcodePrint(const TCode* tc)
{
	// Print number of globals
	fprintf(stdout, "Num globals: %d\n", tc->m_NumGlobals);
	fprintf(stdout, "\n");

	// Print constants
	fprintf(stdout, "Constant numbers table\n");
	fprintf(stdout, "----------------------\n");
	for (unsigned int i = 0; i < tc->m_NumConstNumbers; ++i) {
		fprintf(stdout, "%d: %g\n", i, tc->m_ConstNumTable[i]);
	}
	fprintf(stdout, "\n");

	fprintf(stdout, "Constant string table\n");
	fprintf(stdout, "---------------------\n");
	for (unsigned int i = 0; i < tc->m_NumConstStrings; ++i) {
		fprintf(stdout, "%d: \"%s\"\n", i, tc->m_ConstStringTable[i]);
	}
	fprintf(stdout, "\n");

	// Print program functions
	fprintf(stdout, "Program functions\n");
	fprintf(stdout, "-----------------\n");
	for (unsigned int i = 0; i < tc->m_NumUserFunctions; ++i) {
		const TCodeUserFunction* func = &tc->m_UserFunctionTable[i];
		fprintf(stdout, "%d: addr = %u, nlocals = %d\n", i, func->m_TCodeOffset, func->m_NumLocals);
	}
	fprintf(stdout, "\n");

	// Print library functions
	fprintf(stdout, "Library functions\n");
	fprintf(stdout, "-----------------\n");
	for (unsigned int i = 0; i < tc->m_NumLibFunctions; ++i) {
		fprintf(stdout, "%d: %s\n", i, tc->m_LibFunctionTable[i]);
	}
	fprintf(stdout, "\n");

	const unsigned int numInstructions = tc->m_NumInstructions;
	for (unsigned int i = 0; i < numInstructions; ++i) {
		const TCodeInstruction* instr = &tc->m_Instructions[i];

		fprintf(stdout, "%04d: ", i);
		tcodePrintOpcode(instr->m_Op);
		fprintf(stdout, " ");

		switch (instr->m_Op) {
		case TCOT_Assign:
			tcodePrintOperand(&instr->m_Result);
			fprintf(stdout, ", ");
			tcodePrintOperand(&instr->m_Arg1);
			break;
		case TCOT_Add:
		case TCOT_Sub:
		case TCOT_Mul:
		case TCOT_Div:
		case TCOT_Mod:
			tcodePrintOperand(&instr->m_Result);
			fprintf(stdout, ", ");
			tcodePrintOperand(&instr->m_Arg1);
			fprintf(stdout, ", ");
			tcodePrintOperand(&instr->m_Arg2);
			break;
		case TCOT_Jump:
			tcodePrintOperand(&instr->m_Result);
			break;
		case TCOT_JumpEqual:
		case TCOT_JumpNotEqual:
		case TCOT_JumpLessEqual:
		case TCOT_JumpGreaterEqual:
		case TCOT_JumpLess:
		case TCOT_JumpGreater:
			tcodePrintOperand(&instr->m_Arg1);
			fprintf(stdout, ", ");
			tcodePrintOperand(&instr->m_Arg2);
			fprintf(stdout, ", ");
			tcodePrintOperand(&instr->m_Result);
			break;
		case TCOT_Call:
			tcodePrintOperand(&instr->m_Result);
			break;
		case TCOT_PushArg:
			tcodePrintOperand(&instr->m_Arg1);
			break;
		case TCOT_FuncEnter:
		case TCOT_FuncExit:
			tcodePrintOperand(&instr->m_Result);
			break;
		case TCOT_TableCreate:
			tcodePrintOperand(&instr->m_Result);
			break;
		case TCOT_TableGetElement:
		case TCOT_TableSetElement:
			tcodePrintOperand(&instr->m_Result);
			fprintf(stdout, ", ");
			tcodePrintOperand(&instr->m_Arg1);
			fprintf(stdout, ", ");
			tcodePrintOperand(&instr->m_Arg2);
			break;
		case TCOT_Nop:
			break;
		default:
			assert(false); // Unknown instruction opcode
			break;
		}
		fprintf(stdout, "\n");
	}
}

bool tcodeSaveBinary(const TCode* tc, const char* filename)
{
	FILE* f = fopen(filename, "wb");
	if (!f) {
		return false;
	}

	unsigned int magic = 340200501; // ???
	fwrite(&magic, sizeof(unsigned int), 1, f);

	// Save arrays
	{
		// Strings
		{
			const unsigned int n = tc->m_NumConstStrings;
			fwrite(&n, sizeof(unsigned int), 1, f);
			for (unsigned int i = 0; i < n; ++i) {
				const char* str = tc->m_ConstStringTable[i];
				char* escapedString = escapeString(str);
				unsigned int len = (unsigned int)strlen(escapedString);
				fwrite(&len, sizeof(unsigned int), 1, f);
				fwrite(escapedString, sizeof(char), len, f);
				free(escapedString);
			}
		}

		// Numbers
		{
			const unsigned int n = tc->m_NumConstNumbers;
			fwrite(&n, sizeof(unsigned int), 1, f);
			fwrite(tc->m_ConstNumTable, sizeof(double), n, f);
		}

		// User functions...
		{
			const unsigned int n = tc->m_NumUserFunctions;
			fwrite(&n, sizeof(unsigned int), 1, f);
			for (unsigned int i = 0; i < n; ++i) {
				TCodeUserFunction* func = &tc->m_UserFunctionTable[i];
				fwrite(&func->m_TCodeOffset, sizeof(unsigned int), 1, f);
				fwrite(&func->m_NumLocals, sizeof(unsigned int), 1, f);
				
				unsigned int len = (unsigned int)strlen(func->m_Symbol->m_Identifier);
				fwrite(&len, sizeof(unsigned int), 1, f);
				fwrite(func->m_Symbol->m_Identifier, sizeof(char), len, f);
			}
		}

		// Library functions...
		{
			const unsigned int n = tc->m_NumLibFunctions;
			fwrite(&n, sizeof(unsigned int), 1, f);
			for (unsigned int i = 0; i < n; ++i) {
				const char* str = tc->m_LibFunctionTable[i];
				unsigned int len = (unsigned int)strlen(str);
				fwrite(&len, sizeof(unsigned int), 1, f);
				fwrite(str, sizeof(char), len, f);
			}
		}
	}

	// Save number of globals.
	fwrite(&tc->m_NumGlobals, sizeof(unsigned int), 1, f);

	// Code
	{
		const unsigned int n = tc->m_NumInstructions;
		fwrite(&n, sizeof(unsigned int), 1, f);
		for (unsigned int i = 0; i < n; ++i) {
			const TCodeInstruction* instr = &tc->m_Instructions[i];

			unsigned char opcode = (unsigned char)instr->m_Op;
			fwrite(&opcode, sizeof(unsigned char), 1, f);

			// Result
			{
				const TCodeOperand* op = &instr->m_Result;
				unsigned char type = (unsigned char)op->m_Type;
				fwrite(&type, sizeof(unsigned char), 1, f);
				fwrite(&op->m_Value, sizeof(unsigned int), 1, f);
			}
			// Arg1
			{
				const TCodeOperand* op = &instr->m_Arg1;
				unsigned char type = (unsigned char)op->m_Type;
				fwrite(&type, sizeof(unsigned char), 1, f);
				fwrite(&op->m_Value, sizeof(unsigned int), 1, f);
			}
			// Arg2
			{
				const TCodeOperand* op = &instr->m_Arg2;
				unsigned char type = (unsigned char)op->m_Type;
				fwrite(&type, sizeof(unsigned char), 1, f);
				fwrite(&op->m_Value, sizeof(unsigned int), 1, f);
			}
		}
	}

	// Debug info
	{
		const unsigned int n = tc->m_NumInstructions;
		for (unsigned int i = 0; i < n; ++i) {
			const TCodeInstruction* instr = &tc->m_Instructions[i];
			fwrite(&instr->m_SrcLine, sizeof(unsigned int), 1, f);
		}
	}

	fclose(f);

	return true;
}