#ifndef TCODE_H
#define TCODE_H

struct ICode;
struct SymbolTable;
struct Symbol;

// 5 bits per opcode
enum TCodeOperationType
{
	TCOT_Assign = 0,
	TCOT_Add = 1,
	TCOT_Sub = 2,
	TCOT_Mul = 3,
	TCOT_Div = 4,
	TCOT_Mod = 5,
	TCOT_Jump = 6,
	TCOT_JumpEqual = 7,
	TCOT_JumpNotEqual = 8,
	TCOT_JumpLessEqual = 9,
	TCOT_JumpGreaterEqual = 10,
	TCOT_JumpLess = 11,
	TCOT_JumpGreater = 12,
	TCOT_Call = 13,
	TCOT_PushArg = 14,
	TCOT_FuncEnter = 15,
	TCOT_FuncExit = 16,
	TCOT_TableCreate = 17,
	TCOT_TableGetElement = 18,
	TCOT_TableSetElement = 19,
	TCOT_Nop = 20
};

// 4 bits per operand type
enum TCodeOperandType
{
	TCOT_Label = 0,
	TCOT_GlobalVariable = 1,
	TCOT_FunctionArgument = 2,
	TCOT_LocalVariable = 3,
	TCOT_ConstNum = 4,
	TCOT_ConstString = 5,
	TCOT_ConstBool = 6,
	TCOT_Nil = 7,
	TCOT_UserFunction = 8,
	TCOT_LibraryFunction = 9,
	TCOT_RetVal = 10
};

struct TCodeOperand
{
	TCodeOperandType m_Type;
	unsigned int m_Value;
};

struct TCodeInstruction
{
	TCodeOperationType m_Op;
	TCodeOperand m_Result;
	TCodeOperand m_Arg1;
	TCodeOperand m_Arg2;
	unsigned int m_SrcLine;
};

struct TCodeUserFunction
{
	unsigned int m_ICodeQuadID;
	unsigned int m_TCodeOffset;
	unsigned int m_NumLocals;
	Symbol* m_Symbol;
};

struct TCode
{
	TCodeInstruction* m_Instructions;
	unsigned int m_NumInstructions;
	unsigned int m_InstructionCapacity;

	char** m_ConstStringTable;
	unsigned int m_NumConstStrings;
	unsigned int m_ConstStringTableCapacity;

	double* m_ConstNumTable;
	unsigned int m_NumConstNumbers;
	unsigned int m_ConstNumTableCapacity;

	TCodeUserFunction* m_UserFunctionTable;
	unsigned int m_NumUserFunctions;
	unsigned int m_UserFunctionTableCapacity;

	char** m_LibFunctionTable;
	unsigned int m_NumLibFunctions;
	unsigned int m_LibFunctionTableCapacity;

	unsigned int* m_FirstQuadInstrID; // icode quad ID -> tcode instruction ID map

	unsigned int m_NumGlobals;
};

TCode* tcodeCompileICode(const ICode* ic, const SymbolTable* symTable);
void tcodeDestroy(TCode* tc);

void tcodePrint(const TCode* tc);

bool tcodeSaveBinary(const TCode* tc, const char* filename);

#endif
