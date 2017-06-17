#include "avm.h"
#include "avm_op.h"
#include "avm_stack.h"
#include "avm_table.h"
#include "avm_memory.h"
#include "avm_helpers.h"
#include <stdio.h>
#include <stdarg.h> // va_list, va_start, va_arg, va_end
#include <malloc.h> // malloc()/free()
#include <string.h> // strcmp()
#include <assert.h> // assert()

#define MAGIC_HEADER 340200501

typedef void(*AVMExecutOpFunc)(AVM*, AVMInstruction*);

static const AVMExecutOpFunc avmExecuteOp[AVMOp_NumOpCodes] = {
	avmOpExecute_assign,
	avmOpExecute_BinaryArithmetic,
	avmOpExecute_BinaryArithmetic,
	avmOpExecute_BinaryArithmetic,
	avmOpExecute_BinaryArithmetic,
	avmOpExecute_BinaryArithmetic,
	avmOpExecute_jmp,
	avmOpExecute_jeq,
	avmOpExecute_jne,
	avmOpExecute_jle,
	avmOpExecute_jge,
	avmOpExecute_jlt,
	avmOpExecute_jgt,
	avmOpExecute_call,
	avmOpExecute_pusharg,
	avmOpExecute_funcenter,
	avmOpExecute_funcexit,
	avmOpExecute_tablecreate,
	avmOpExecute_tablegetelem,
	avmOpExecute_tablesetelem,
	avmOpExecute_nop
};

char* loadString(FILE* f);

//////////////////////////////////////////////////////////////////////////
// Public AVM interface...
//
AVM* avmCreate(unsigned int stackSize)
{
	AVM* avm = (AVM*)malloc(sizeof(AVM));

	// Initialize tables
	avm->m_ConstStringTable = NULL;
	avm->m_NumConstStrings = 0;
	avm->m_ConstNumTable = NULL;
	avm->m_NumConstNumbers = 0;
	avm->m_UserFunctionTable = NULL;
	avm->m_NumUserFunctions = 0;
	avm->m_LibraryFunctionTable = NULL;
	avm->m_NumLibraryFunctions = 0;
	avm->m_NumGlobals = 0;
	avm->m_Instructions = NULL;
	avm->m_SrcLineNumber = NULL;
	avm->m_NumInstructions = 0;

	avmInitTables(avm);

	// Initialize state
	// Registers
	avm->m_Registers = (AVMMemoryCell*)malloc(sizeof(AVMMemoryCell) * Reg_NumRegisters);
	for (unsigned int i = 0; i < Reg_NumRegisters; ++i) {
		avmMemCellInit(&avm->m_Registers[i]);
	}

	// Stack
	avm->m_Stack = (AVMMemoryCell*)malloc(sizeof(AVMMemoryCell) * stackSize);
	if (!avm->m_Stack) {
		free(avm);
		return NULL;
	}

	avm->m_StackSize = stackSize;
	for (unsigned int i = 0; i < stackSize; ++i) {
		avmMemCellInit(&avm->m_Stack[i]);
	}

	// The rest
	avm->m_TopSP = 0; // Indicates that we are currently outside of a function
	avm->m_Top = avm->m_StackSize - 1;
	avm->m_TotalFunctionArguments = 0;
	avm->m_PC = 0;
	avm->m_ExecutionFinished = false;
	avm->m_CurSrcLineNumber = 0;

	return avm;
}

void avmDestroy(AVM* avm)
{
	// Clear all stack memory cells.
	for (unsigned int i = 0; i < avm->m_StackSize; ++i) {
		avmMemCellClear(avm, &avm->m_Stack[i]);
	}
	free(avm->m_Stack);
	avm->m_Stack = NULL;
	avm->m_StackSize = 0;

	// Destroy tables
	avmDestroyTables(avm);

	for (unsigned int i = 0; i < avm->m_NumConstStrings; ++i) {
		free(avm->m_ConstStringTable[i]);
	}
	free(avm->m_ConstStringTable);
	avm->m_ConstStringTable = NULL;
	avm->m_NumConstStrings = 0;

	free(avm->m_LibraryFunctionTable);
	avm->m_LibraryFunctionTable = NULL;
	avm->m_NumLibraryFunctions = 0;

	free(avm->m_Instructions);
	avm->m_Instructions = NULL;
	free(avm->m_SrcLineNumber);
	avm->m_SrcLineNumber = NULL;
	avm->m_NumInstructions = 0;

	free(avm->m_ConstNumTable);
	avm->m_ConstNumTable = NULL;
	avm->m_NumConstNumbers = 0;

	for (unsigned int i = 0; i < avm->m_NumUserFunctions; ++i) {
		free(avm->m_UserFunctionTable[i].m_Identifier);
	}
	free(avm->m_UserFunctionTable);
	avm->m_UserFunctionTable = NULL;
	avm->m_NumUserFunctions = 0;

	avm->m_NumGlobals = 0;

	free(avm);
}

bool avmLoadProgram(AVM* avm, const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f) {
		return false;
	}

	unsigned int magic = 0;
	fread(&magic, sizeof(unsigned int), 1, f);
	if (magic != MAGIC_HEADER) {
		avmError(avm, "Invalid program header");
		fclose(f);
		return false;
	}

	// Load arrays
	{
		// Strings
		{
			unsigned int n;
			fread(&n, sizeof(unsigned int), 1, f);

			if (n > 0) {
				avm->m_ConstStringTable = (char**)malloc(sizeof(char*) * n);
				if (!avm->m_ConstStringTable) {
					avmError(avm, "Failed to allocate constant string table (%u entries)", n);
					fclose(f);
					return false;
				}
			}

			avm->m_NumConstStrings = n;
			for (unsigned int i = 0; i < n; ++i) {
				avm->m_ConstStringTable[i] = loadString(f);
			}
		}

		// Numbers
		{
			unsigned int n;
			fread(&n, sizeof(unsigned int), 1, f);
			if (n > 0) {
				avm->m_ConstNumTable = (double*)malloc(sizeof(double) * n);
				if (!avm->m_ConstNumTable) {
					avmError(avm, "Failed to allocate constant number table (%u entries)", n);
					fclose(f);
					return false;
				}

				fread(avm->m_ConstNumTable, sizeof(double), n, f);
			}

			avm->m_NumConstNumbers = n;
		}

		// User functions
		{
			unsigned int n;
			fread(&n, sizeof(unsigned int), 1, f);
			if (n > 0) {
				avm->m_UserFunctionTable = (AVMUserFunc*)malloc(sizeof(AVMUserFunc) * n);
				if (!avm->m_UserFunctionTable) {
					avmError(avm, "Failed to allocate user function table (%u entries)", n);
					fclose(f);
					return false;
				}
			}
			
			avm->m_NumUserFunctions = n;
			for (unsigned int i = 0; i < n; ++i) {
				AVMUserFunc* func = &avm->m_UserFunctionTable[i];
				fread(&func->m_Address, sizeof(unsigned int), 1, f);
				fread(&func->m_NumLocals, sizeof(unsigned int), 1, f);
				func->m_Identifier = loadString(f);
			}
		}

		// Library functions...
		{
			unsigned int n;
			fread(&n, sizeof(unsigned int), 1, f);
			if (n > 0) {
				avm->m_LibraryFunctionTable = (AVMLibraryFunc*)malloc(sizeof(AVMLibraryFunc) * n);
				if (!avm->m_LibraryFunctionTable) {
					avmError(avm, "Failed to allocate library function table (%u entries)", n);
					fclose(f);
					return false;
				}
			}

			avm->m_NumLibraryFunctions = n;
			for (unsigned int i = 0; i < n; ++i) {
				avm->m_LibraryFunctionTable[i].m_Name = loadString(f);
				avm->m_LibraryFunctionTable[i].m_Callback = avmGetLibraryFunctionPointer(avm->m_LibraryFunctionTable[i].m_Name);
				if (!avm->m_LibraryFunctionTable[i].m_Callback) {
					avmError(avm, "Unknown library function %s used by program.", avm->m_LibraryFunctionTable[i].m_Name);
					free(avm->m_LibraryFunctionTable[i].m_Name);
					fclose(f);
					return false;
				}
			}
		}
	}

	// Load the number of globals.
	{
		unsigned int n;
		fread(&n, sizeof(unsigned int), 1, f);

		avm->m_NumGlobals = n;
	}

	// Load the instructions...
	{
		unsigned int n;
		fread(&n, sizeof(unsigned int), 1, f);
		if (n > 0) {
			avm->m_Instructions = (AVMInstruction*)malloc(sizeof(AVMInstruction) * n);
			if (!avm->m_Instructions) {
				avmError(avm, "Failed to allocate instruction table (%u entries)", n);
				fclose(f);
				return false;
			}
		}
		
		avm->m_NumInstructions = n;
		for (unsigned int i = 0; i < n; ++i) {
			AVMInstruction* instr = &avm->m_Instructions[i];
			
			fread(&instr->m_OpCode, sizeof(unsigned char), 1, f);

			fread(&instr->m_Res.m_Type, sizeof(unsigned char), 1, f);
			fread(&instr->m_Res.m_Value, sizeof(unsigned int), 1, f);

			fread(&instr->m_Arg1.m_Type, sizeof(unsigned char), 1, f);
			fread(&instr->m_Arg1.m_Value, sizeof(unsigned int), 1, f);

			fread(&instr->m_Arg2.m_Type, sizeof(unsigned char), 1, f);
			fread(&instr->m_Arg2.m_Value, sizeof(unsigned int), 1, f);
		}
	}

	// Load debug info
	if (avm->m_NumInstructions) {
		avm->m_SrcLineNumber = (unsigned int*)malloc(sizeof(unsigned int) * avm->m_NumInstructions);
		if (!avm->m_SrcLineNumber) {
			avmError(avm, "Failed to allocate debug info table (%u entries)", avm->m_NumInstructions);
			fclose(f);
			return false;
		}

		fread(avm->m_SrcLineNumber, sizeof(unsigned int), avm->m_NumInstructions, f);
	}

	fclose(f);

	return true;
}

void avmRun(AVM* avm)
{
	avm->m_Top = avm->m_StackSize - 1 - avm->m_NumGlobals;
	avm->m_TopSP = 0;

	while (!avm->m_ExecutionFinished) {
		if (avm->m_PC == avm->m_NumInstructions) {
			avm->m_ExecutionFinished = true;
		} else {
			assert(avm->m_PC < avm->m_NumInstructions);
			AVMInstruction* instr = &avm->m_Instructions[avm->m_PC];

			unsigned int newSrcLine = avm->m_SrcLineNumber[avm->m_PC];
			if (newSrcLine != ~0u) {
				avm->m_CurSrcLineNumber = newSrcLine;
			}

			assert(instr->m_OpCode < AVMOp_NumOpCodes);
			unsigned int oldPC = avm->m_PC;
			avmExecuteOp[instr->m_OpCode](avm, instr);
			if (avm->m_PC == oldPC) {
				++avm->m_PC;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Private AVM interface...
//
double avmGetConstNumber(AVM* avm, unsigned int index)
{
	if (index >= avm->m_NumConstNumbers) {
		avmError(avm, "Tried to read invalid constant number %u\n", index);
		return 0.0;
	}

	return avm->m_ConstNumTable[index];
}

const char* avmGetConstString(AVM* avm, unsigned int index)
{
	if (index >= avm->m_NumConstStrings) {
		avmError(avm, "Tried to read invalid constant string %u\n", index);
		return "";
	}

	return avm->m_ConstStringTable[index];
}

AVMUserFunc* avmGetUserFunction(AVM* avm, unsigned int index)
{
	if (index >= avm->m_NumUserFunctions) {
		avmError(avm, "Tried to read invalid user function %u\n", index);
		return NULL;
	}

	return &avm->m_UserFunctionTable[index];
}

AVMLibraryFunc* avmGetLibraryFunction(AVM* avm, unsigned int index)
{
	if (index >= avm->m_NumLibraryFunctions) {
		avmError(avm, "Tried to read invalid library function %u\n", index);
		return NULL;
	}

	return &avm->m_LibraryFunctionTable[index];
}

AVMMemoryCell* avmTranslateOperand(AVM* avm, const AVMInstructionOperand* op, AVMMemoryCell* reg)
{
	switch (op->m_Type) {
	case AVMOperand_Global:
		assert(op->m_Value < avm->m_NumGlobals);
		assert(op->m_Value < avm->m_StackSize);
		return &avm->m_Stack[avm->m_StackSize - 1 - op->m_Value];
	case AVMOperand_Local:
		assert(avm->m_TopSP != 0); // Are we inside a function?
		assert(avm->m_TopSP > op->m_Value);
		return &avm->m_Stack[avm->m_TopSP - op->m_Value];
	case AVMOperand_FunctionArgument:
		assert(avm->m_TopSP != 0); // Are we inside a function?
		assert(avm->m_TopSP + AVM_STACK_ENV_SIZE + 1 + op->m_Value < avm->m_StackSize);
		return &avm->m_Stack[avm->m_TopSP + AVM_STACK_ENV_SIZE + 1 + op->m_Value];
	case AVMOperand_RetVal:
		return &avm->m_Registers[Reg_RetVal];
	case AVMOperand_ConstNumber:
		assert(reg != NULL);
		reg->m_Type = AVMMemCell_Number;
		reg->m_Data.m_Number = avmGetConstNumber(avm, op->m_Value);
		break;
	case AVMOperand_ConstString:
		assert(reg != NULL);
		reg->m_Type = AVMMemCell_String;
		reg->m_Data.m_String = strdup(avmGetConstString(avm, op->m_Value));
		break;
	case AVMOperand_ConstBool:
		assert(reg != NULL);
		reg->m_Type = AVMMemCell_Bool;
		reg->m_Data.m_Bool = op->m_Value != 0 ? true : false;
		break;
	case AVMOperand_Nil:
		assert(reg != NULL);
		reg->m_Type = AVMMemCell_Nil;
		break;
	case AVMOperand_UserFunction:
		assert(reg != NULL);
		reg->m_Type = AVMMemCell_UserFunction;
		reg->m_Data.m_UserFunc = avmGetUserFunction(avm, op->m_Value);
		break;
	case AVMOperand_LibraryFunction:
		assert(reg != NULL);
		reg->m_Type = AVMMemCell_LibraryFunction;
		reg->m_Data.m_LibraryFunc = avmGetLibraryFunction(avm, op->m_Value);
		break;
	default:
		assert(false); // Unknown operand type.
		break;
	}

	return reg;
}

void avmError(AVM* avm, const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	fprintf(stderr, "(x) AVM Error (%u): ", avm->m_CurSrcLineNumber);
	vfprintf(stderr, fmt, argptr);
	va_end(argptr);
	fprintf(stderr, "\n");

	avm->m_ExecutionFinished = true;
}

void avmWarning(AVM* avm, const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	fprintf(stderr, "(!) AVM Warning (%u): ", avm->m_CurSrcLineNumber);
	vfprintf(stderr, fmt, argptr);
	va_end(argptr);
	fprintf(stderr, "\n");
}

//////////////////////////////////////////////////////////////////////////
// Helpers
//
char* loadString(FILE* f)
{
	unsigned int len;
	fread(&len, sizeof(unsigned int), 1, f);

	char* str = (char*)malloc(sizeof(char) * (len + 1));
	fread(str, sizeof(char), len, f);
	str[len] = 0;

	return str;
}
