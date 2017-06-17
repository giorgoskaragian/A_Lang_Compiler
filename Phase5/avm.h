#ifndef AVM_H
#define AVM_H

#include "avm_lib.h" // AVMLibraryFuncCallback

struct AVM;
struct AVMUserFunc;
struct AVMLibraryFunc;
struct AVMMemoryCell;
struct AVMInstruction;
struct AVMTable;
class HandleAlloc32;

enum AVMOpCode
{
	// NOTE: WARNING: The order should be the same as the order in TCodeOperationType from phase4
	AVMOp_Assign = 0,
	AVMOp_Add = 1,
	AVMOp_Sub = 2,
	AVMOp_Mul = 3,
	AVMOp_Div = 4,
	AVMOp_Mod = 5,
	AVMOp_Jump = 6,
	AVMOp_JumpEqual = 7,
	AVMOp_JumpNotEqual = 8,
	AVMOp_JumpLessEqual = 9,
	AVMOp_JumpGreaterEqual = 10,
	AVMOp_JumpLess = 11,
	AVMOp_JumpGreater = 12,
	AVMOp_Call = 13,
	AVMOp_PushArg = 14,
	AVMOp_FuncEnter = 15,
	AVMOp_FuncExit = 16,
	AVMOp_TableCreate = 17,
	AVMOp_TableGetElement = 18,
	AVMOp_TableSetElement = 19,
	AVMOp_Nop = 20,

	AVMOp_NumOpCodes
};

enum AVMOperandType
{
	// NOTE: WARNING: The order should be the same as the order in TCodeOperandType from phase4
	AVMOperand_Label = 0,
	AVMOperand_Global = 1,
	AVMOperand_FunctionArgument = 2,
	AVMOperand_Local = 3,
	AVMOperand_ConstNumber = 4,
	AVMOperand_ConstString = 5,
	AVMOperand_ConstBool = 6,
	AVMOperand_Nil = 7,
	AVMOperand_UserFunction = 8,
	AVMOperand_LibraryFunction = 9,
	AVMOperand_RetVal = 10,
};

enum AVMReg
{
	Reg_AX = 0,
	Reg_BX = 1,
	Reg_CX = 2,
	Reg_RetVal = 3,

	Reg_NumRegisters
};

struct AVM
{
	char** m_ConstStringTable;
	unsigned int m_NumConstStrings;

	double* m_ConstNumTable;
	unsigned int m_NumConstNumbers;

	AVMUserFunc* m_UserFunctionTable;
	unsigned int m_NumUserFunctions;

	AVMLibraryFunc* m_LibraryFunctionTable;
	unsigned int m_NumLibraryFunctions;

	unsigned int m_NumGlobals;

	AVMInstruction* m_Instructions;
	unsigned int* m_SrcLineNumber;
	unsigned int m_NumInstructions;

	// Tables
	AVMTable** m_Tables;
	HandleAlloc32* m_TableHandleAllocator;
	unsigned int m_TableCapacity;

	// State
	AVMMemoryCell* m_Stack;
	AVMMemoryCell* m_Registers; // AX, BX, CX, RetVal
	unsigned int m_TopSP;
	unsigned int m_Top;
	unsigned int m_TotalFunctionArguments;
	unsigned int m_StackSize;
	unsigned int m_PC;
	unsigned int m_CurSrcLineNumber;
	bool m_ExecutionFinished;
};

struct AVMInstructionOperand
{
	unsigned char m_Type;
	unsigned int m_Value;
};

struct AVMInstruction
{
	unsigned char m_OpCode;
	AVMInstructionOperand m_Res;
	AVMInstructionOperand m_Arg1;
	AVMInstructionOperand m_Arg2;
};

struct AVMUserFunc
{
	char* m_Identifier;
	unsigned int m_Address;
	unsigned int m_NumLocals;
};

struct AVMLibraryFunc
{
	char* m_Name;
	AVMLibraryFuncCallback m_Callback;
};

// Public AVM interface...
AVM* avmCreate(unsigned int stackSize);
void avmDestroy(AVM* avm);
bool avmLoadProgram(AVM* avm, const char* filename);
void avmRun(AVM* avm);

// Private AVM interface
AVMMemoryCell* avmTranslateOperand(AVM* avm, const AVMInstructionOperand* op, AVMMemoryCell* memcell);
double avmGetConstNumber(AVM* avm, unsigned int index);
const char* avmGetConstString(AVM* avm, unsigned int index);
AVMUserFunc* avmGetUserFunction(AVM* avm, unsigned int index);
AVMLibraryFunc* avmGetLibraryFunction(AVM* avm, unsigned int index);
void avmError(AVM* avm, const char* fmt, ...);
void avmWarning(AVM* avm, const char* fmt, ...);

#endif
