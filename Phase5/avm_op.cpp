#include "avm_op.h"
#include "avm.h"
#include "avm_lib.h"
#include "avm_table.h"
#include "avm_stack.h"
#include "avm_memory.h"
#include "avm_helpers.h"
#include <string.h> // strcmp()
#include <malloc.h> // free()
#include <assert.h> // assert()

void avmCallSaveEnv(AVM* avm)
{
	avmStackPushEnvValue(avm, avm->m_TotalFunctionArguments);
	avmStackPushEnvValue(avm, avm->m_PC + 1);
	avmStackPushEnvValue(avm, avm->m_Top + avm->m_TotalFunctionArguments + 2);
	avmStackPushEnvValue(avm, avm->m_TopSP);
}

unsigned int avmGetTotalFunctionArguments(AVM* avm)
{
	return avmStackGetEnvValue(avm, AVM_NUM_FUNCTION_ARGS_OFFSET);
}

AVMMemoryCell* avmGetFunctionArgument(AVM* avm, unsigned int index)
{
	assert(index < avmGetTotalFunctionArguments(avm));
	return &avm->m_Stack[avm->m_TopSP + AVM_STACK_ENV_SIZE + 1 + index];
}

void avmOpExecute_assign(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* lvalue = avmTranslateOperand(avm, &instr->m_Res, NULL);
	AVMMemoryCell* rvalue = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);

	assert(lvalue);
	assert(rvalue);
	assert(avmMemCellIsStack(avm, lvalue) || avmMemCellIsRetVal(avm, lvalue));
	assert(avmMemCellIsStack(avm, rvalue) || avmMemCellIsRegister(avm, rvalue));

	avmMemCellAssign(avm, lvalue, rvalue);
}

void avmOpExecute_BinaryArithmetic(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* lv = avmTranslateOperand(avm, &instr->m_Res, NULL);
	AVMMemoryCell* arg1 = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* arg2 = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(lv);
	assert(arg1);
	assert(arg2);
	assert(avmMemCellIsStack(avm, lv)); // The result will always be written to a stack memcell.
	assert(avmMemCellIsStack(avm, arg1) || avmMemCellIsRegister(avm, arg1));
	assert(avmMemCellIsStack(avm, arg2) || avmMemCellIsRegister(avm, arg2));

	if (arg1->m_Type != AVMMemCell_Number || arg2->m_Type != AVMMemCell_Number) {
		avmError(avm, "Both arguments should be numbers in arithmetic operations.");
	} else {
		avmMemCellClear(avm, lv);
		lv->m_Type = AVMMemCell_Number;

		switch (instr->m_OpCode) {
		case AVMOp_Add:	lv->m_Data.m_Number = arg1->m_Data.m_Number + arg2->m_Data.m_Number; break;
		case AVMOp_Sub:	lv->m_Data.m_Number = arg1->m_Data.m_Number - arg2->m_Data.m_Number; break;
		case AVMOp_Mul:	lv->m_Data.m_Number = arg1->m_Data.m_Number * arg2->m_Data.m_Number; break;
		case AVMOp_Div:
		{
			if (arg2->m_Data.m_Number == 0) {
				avmError(avm, "Division by zero");
			} else {
				lv->m_Data.m_Number = arg1->m_Data.m_Number / arg2->m_Data.m_Number;
			}
			break;
		}
		case AVMOp_Mod:
		{
			int i1 = (int)arg1->m_Data.m_Number;
			int i2 = (int)arg2->m_Data.m_Number;
			if (i2 == 0) {
				avmError(avm, "Division by zero");
			} else {
				lv->m_Data.m_Number = (double)(i1 % i2);
			}
			break;
		}
		}
	}
}

void avmOpExecute_jmp(AVM* avm, AVMInstruction* instr)
{
	// No operand translation for label operands.
	assert(instr->m_Res.m_Type == AVMOperand_Label);
	if (!avm->m_ExecutionFinished) {
		avm->m_PC = instr->m_Res.m_Value;
	}
}

void avmOpExecute_jeq(AVM* avm, AVMInstruction* instr)
{
	// No operand translation for label operands.
	assert(instr->m_Res.m_Type == AVMOperand_Label);

	AVMMemoryCell* arg1 = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* arg2 = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(arg1 && (avmMemCellIsStack(avm, arg1) || avmMemCellIsRegister(avm, arg1)));
	assert(arg2 && (avmMemCellIsStack(avm, arg2) || avmMemCellIsRegister(avm, arg2)));

	bool result = false;
	if (arg1->m_Type == AVMMemCell_Undefined || arg2->m_Type == AVMMemCell_Undefined) {
		avmError(avm, "Undefined value involved in conditional");
	} else if (arg1->m_Type == AVMMemCell_Nil || arg2->m_Type == AVMMemCell_Nil) {
		// If one of them are nil both of them should be nil for the condition to be true.
		result = arg1->m_Type == AVMMemCell_Nil && arg2->m_Type == AVMMemCell_Nil;
	} else if (arg1->m_Type == AVMMemCell_Bool || arg2->m_Type == AVMMemCell_Bool) {
		// If one of them is a boolean, convert both to boolean and compare.
		result = avmMemCellToBool(arg1) == avmMemCellToBool(arg2);
	} else if (arg1->m_Type != arg2->m_Type) {
		// Otherwise both should be the same type.
		avmError(avm, "%s == %s is illegal", avmMemCellTypeToString(arg1->m_Type), avmMemCellTypeToString(arg2->m_Type));
	} else {
		// Both args are of the same type and not Undefined/Nil/Bool. Do the check depending on the type.
		if (arg1->m_Type == AVMMemCell_Number) {
			result = arg1->m_Data.m_Number == arg2->m_Data.m_Number;
		} else if (arg1->m_Type == AVMMemCell_String) {
			result = !strcmp(arg1->m_Data.m_String, arg2->m_Data.m_String);
		} else if (arg1->m_Type == AVMMemCell_Table) {
			// NOTE: Only if the handles are the same, the tables are considered the same. No deep comparison.
			result = arg1->m_Data.m_TableHandle == arg2->m_Data.m_TableHandle;
		} else if (arg1->m_Type == AVMMemCell_UserFunction) {
			result = arg1->m_Data.m_UserFunc->m_Address == arg2->m_Data.m_UserFunc->m_Address;
		} else if (arg1->m_Type == AVMMemCell_LibraryFunction) {
			result = arg1->m_Data.m_LibraryFunc == arg2->m_Data.m_LibraryFunc;
		} else {
			assert(false); // Unknown memory cell type.
		}
	}

	if (!avm->m_ExecutionFinished && result) {
		avm->m_PC = instr->m_Res.m_Value;
	}
}

void avmOpExecute_jne(AVM* avm, AVMInstruction* instr)
{
	// No operand translation for label operands.
	assert(instr->m_Res.m_Type == AVMOperand_Label);

	AVMMemoryCell* arg1 = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* arg2 = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(arg1 && (avmMemCellIsStack(avm, arg1) || avmMemCellIsRegister(avm, arg1)));
	assert(arg2 && (avmMemCellIsStack(avm, arg2) || avmMemCellIsRegister(avm, arg2)));

	bool result = false;
	if (arg1->m_Type == AVMMemCell_Undefined || arg2->m_Type == AVMMemCell_Undefined) {
		avmError(avm, "Undefined value involved in conditional");
	} else if (arg1->m_Type == AVMMemCell_Nil || arg2->m_Type == AVMMemCell_Nil) {
		// If one of them are nil, the other should not be nil for the condition to be true
		result = (arg1->m_Type == AVMMemCell_Nil && arg2->m_Type != AVMMemCell_Nil) ||
			(arg1->m_Type != AVMMemCell_Nil && arg2->m_Type != AVMMemCell_Nil);
	} else if (arg1->m_Type == AVMMemCell_Bool || arg2->m_Type == AVMMemCell_Bool) {
		// If one of them is a boolean, convert both to boolean and compare.
		result = avmMemCellToBool(arg1) != avmMemCellToBool(arg2);
	} else if (arg1->m_Type != arg2->m_Type) {
		// Otherwise both should be the same type.
		avmError(avm, "%s != %s is illegal", avmMemCellTypeToString(arg1->m_Type), avmMemCellTypeToString(arg2->m_Type));
	} else {
		// Both args are of the same type and not Undefined/Nil/Bool. Do the check depending on the type.
		if (arg1->m_Type == AVMMemCell_Number) {
			result = arg1->m_Data.m_Number != arg2->m_Data.m_Number;
		} else if (arg1->m_Type == AVMMemCell_String) {
			result = strcmp(arg1->m_Data.m_String, arg2->m_Data.m_String) != 0;
		} else if (arg1->m_Type == AVMMemCell_Table) {
			result = arg1->m_Data.m_TableHandle != arg2->m_Data.m_TableHandle;
		} else if (arg1->m_Type == AVMMemCell_UserFunction) {
			result = arg1->m_Data.m_UserFunc->m_Address != arg2->m_Data.m_UserFunc->m_Address;
		} else if (arg1->m_Type == AVMMemCell_LibraryFunction) {
			result = arg1->m_Data.m_LibraryFunc != arg2->m_Data.m_LibraryFunc;
		} else {
			assert(false); // Unknown memory cell type.
		}
	}

	if (!avm->m_ExecutionFinished && result) {
		avm->m_PC = instr->m_Res.m_Value;
	}
}

void avmOpExecute_jle(AVM* avm, AVMInstruction* instr)
{
	// No operand translation for label operands.
	assert(instr->m_Res.m_Type == AVMOperand_Label);

	AVMMemoryCell* arg1 = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* arg2 = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(arg1 && (avmMemCellIsStack(avm, arg1) || avmMemCellIsRegister(avm, arg1)));
	assert(arg2 && (avmMemCellIsStack(avm, arg2) || avmMemCellIsRegister(avm, arg2)));

	bool result = false;
	if (arg1->m_Type == AVMMemCell_Undefined || arg2->m_Type == AVMMemCell_Undefined) {
		avmError(avm, "Undefined value involved in conditional");
	} else if (arg1->m_Type == AVMMemCell_Nil || arg2->m_Type == AVMMemCell_Nil) {
		// If one of them is nil both of them should be nil for the condition to be true. Ignore the LESS part of the condition
		result = arg1->m_Type == AVMMemCell_Nil && arg2->m_Type == AVMMemCell_Nil;
	} else if (arg1->m_Type == AVMMemCell_Bool || arg2->m_Type == AVMMemCell_Bool) {
		// If one of them is a boolean, convert both to boolean and compare. Ignore the LESS part of the condition
		result = avmMemCellToBool(arg1) == avmMemCellToBool(arg2);
	} else if (arg1->m_Type != arg2->m_Type) {
		// Otherwise both should be the same type.
		avmError(avm, "%s <= %s is illegal", avmMemCellTypeToString(arg1->m_Type), avmMemCellTypeToString(arg2->m_Type));
	} else {
		// Both args are of the same type and not Undefined/Nil/Bool. Do the check depending on the type.
		if (arg1->m_Type == AVMMemCell_Number) {
			result = arg1->m_Data.m_Number <= arg2->m_Data.m_Number;
		} else if (arg1->m_Type == AVMMemCell_String) {
			result = strcmp(arg1->m_Data.m_String, arg2->m_Data.m_String) != 1;
		} else if (arg1->m_Type == AVMMemCell_Table) {
			// NOTE: Only if the handles are the same, the tables are considered the same. No deep comparison. Ignore the LESS part of the condition
			result = arg1->m_Data.m_TableHandle == arg2->m_Data.m_TableHandle;
		} else if (arg1->m_Type == AVMMemCell_UserFunction) {
			// Ignore the LESS part of the condition
			result = arg1->m_Data.m_UserFunc->m_Address == arg2->m_Data.m_UserFunc->m_Address;
		} else if (arg1->m_Type == AVMMemCell_LibraryFunction) {
			// Ignore the LESS part of the condition
			result = arg1->m_Data.m_LibraryFunc == arg2->m_Data.m_LibraryFunc;
		} else {
			assert(false); // Unknown memory cell type.
		}
	}

	if (!avm->m_ExecutionFinished && result) {
		avm->m_PC = instr->m_Res.m_Value;
	}
}

void avmOpExecute_jge(AVM* avm, AVMInstruction* instr)
{
	// No operand translation for label operands.
	assert(instr->m_Res.m_Type == AVMOperand_Label);

	AVMMemoryCell* arg1 = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* arg2 = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(arg1 && (avmMemCellIsStack(avm, arg1) || avmMemCellIsRegister(avm, arg1)));
	assert(arg2 && (avmMemCellIsStack(avm, arg2) || avmMemCellIsRegister(avm, arg2)));

	bool result = false;
	if (arg1->m_Type == AVMMemCell_Undefined || arg2->m_Type == AVMMemCell_Undefined) {
		avmError(avm, "Undefined value involved in conditional");
	} else if (arg1->m_Type == AVMMemCell_Nil || arg2->m_Type == AVMMemCell_Nil) {
		// If one of them is nil both of them should be nil for the condition to be true. Ignore the GREATER part of the condition
		result = arg1->m_Type == AVMMemCell_Nil && arg2->m_Type == AVMMemCell_Nil;
	} else if (arg1->m_Type == AVMMemCell_Bool || arg2->m_Type == AVMMemCell_Bool) {
		// If one of them is a boolean, convert both to boolean and compare. Ignore the GREATER part of the condition
		result = avmMemCellToBool(arg1) == avmMemCellToBool(arg2);
	} else if (arg1->m_Type != arg2->m_Type) {
		// Otherwise both should be the same type.
		avmError(avm, "%s == %s is illegal", avmMemCellTypeToString(arg1->m_Type), avmMemCellTypeToString(arg2->m_Type));
	} else {
		// Both args are of the same type and not Undefined/Nil/Bool. Do the check depending on the type.
		if (arg1->m_Type == AVMMemCell_Number) {
			result = arg1->m_Data.m_Number >= arg2->m_Data.m_Number;
		} else if (arg1->m_Type == AVMMemCell_String) {
			result = strcmp(arg1->m_Data.m_String, arg2->m_Data.m_String) != -1;
		} else if (arg1->m_Type == AVMMemCell_Table) {
			// NOTE: Only if the handles are the same, the tables are considered the same. No deep comparison. Ignore the GREATER part of the condition
			result = arg1->m_Data.m_TableHandle == arg2->m_Data.m_TableHandle;
		} else if (arg1->m_Type == AVMMemCell_UserFunction) {
			// Ignore the GREATER part of the condition
			result = arg1->m_Data.m_UserFunc->m_Address == arg2->m_Data.m_UserFunc->m_Address;
		} else if (arg1->m_Type == AVMMemCell_LibraryFunction) {
			// Ignore the GREATER part of the condition
			result = arg1->m_Data.m_LibraryFunc == arg2->m_Data.m_LibraryFunc;
		} else {
			assert(false); // Unknown memory cell type.
		}
	}

	if (!avm->m_ExecutionFinished && result) {
		avm->m_PC = instr->m_Res.m_Value;
	}
}

void avmOpExecute_jlt(AVM* avm, AVMInstruction* instr)
{
	// No operand translation for label operands.
	assert(instr->m_Res.m_Type == AVMOperand_Label);

	AVMMemoryCell* arg1 = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* arg2 = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(arg1 && (avmMemCellIsStack(avm, arg1) || avmMemCellIsRegister(avm, arg1)));
	assert(arg2 && (avmMemCellIsStack(avm, arg2) || avmMemCellIsRegister(avm, arg2)));

	bool result = false;
	if (arg1->m_Type == AVMMemCell_Undefined || arg2->m_Type == AVMMemCell_Undefined) {
		avmError(avm, "Undefined value involved in conditional");
	} else if (arg1->m_Type == AVMMemCell_Nil || arg2->m_Type == AVMMemCell_Nil) {
		result = false;
	} else if (arg1->m_Type == AVMMemCell_Bool || arg2->m_Type == AVMMemCell_Bool) {
		result = false;
	} else if (arg1->m_Type != arg2->m_Type) {
		// Otherwise both should be the same type.
		avmError(avm, "%s < %s is illegal", avmMemCellTypeToString(arg1->m_Type), avmMemCellTypeToString(arg2->m_Type));
	} else {
		// Both args are of the same type and not Undefined/Nil/Bool. Do the check depending on the type.
		if (arg1->m_Type == AVMMemCell_Number) {
			result = arg1->m_Data.m_Number < arg2->m_Data.m_Number;
		} else if (arg1->m_Type == AVMMemCell_String) {
			result = strcmp(arg1->m_Data.m_String, arg2->m_Data.m_String) == -1;
		} else if (arg1->m_Type == AVMMemCell_Table) {
			result = false;
		} else if (arg1->m_Type == AVMMemCell_UserFunction) {
			result = false;
		} else if (arg1->m_Type == AVMMemCell_LibraryFunction) {
			result = false;
		} else {
			assert(false); // Unknown memory cell type.
		}
	}

	if (!avm->m_ExecutionFinished && result) {
		avm->m_PC = instr->m_Res.m_Value;
	}
}

void avmOpExecute_jgt(AVM* avm, AVMInstruction* instr)
{
	// No operand translation for label operands.
	assert(instr->m_Res.m_Type == AVMOperand_Label);

	AVMMemoryCell* arg1 = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* arg2 = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(arg1 && (avmMemCellIsStack(avm, arg1) || avmMemCellIsRegister(avm, arg1)));
	assert(arg2 && (avmMemCellIsStack(avm, arg2) || avmMemCellIsRegister(avm, arg2)));

	bool result = false;
	if (arg1->m_Type == AVMMemCell_Undefined || arg2->m_Type == AVMMemCell_Undefined) {
		avmError(avm, "Undefined value involved in conditional");
	} else if (arg1->m_Type == AVMMemCell_Nil || arg2->m_Type == AVMMemCell_Nil) {
		result = false;
	} else if (arg1->m_Type == AVMMemCell_Bool || arg2->m_Type == AVMMemCell_Bool) {
		result = false;
	} else if (arg1->m_Type != arg2->m_Type) {
		// Otherwise both should be the same type.
		avmError(avm, "%s > %s is illegal", avmMemCellTypeToString(arg1->m_Type), avmMemCellTypeToString(arg2->m_Type));
	} else {
		// Both args are of the same type and not Undefined/Nil/Bool. Do the check depending on the type.
		if (arg1->m_Type == AVMMemCell_Number) {
			result = arg1->m_Data.m_Number > arg2->m_Data.m_Number;
		} else if (arg1->m_Type == AVMMemCell_String) {
			result = strcmp(arg1->m_Data.m_String, arg2->m_Data.m_String) == 1;
		} else if (arg1->m_Type == AVMMemCell_Table) {
			result = false;
		} else if (arg1->m_Type == AVMMemCell_UserFunction) {
			result = false;
		} else if (arg1->m_Type == AVMMemCell_LibraryFunction) {
			result = false;
		} else {
			assert(false); // Unknown memory cell type.
		}
	}

	if (!avm->m_ExecutionFinished && result) {
		avm->m_PC = instr->m_Res.m_Value;
	}
}

void avmOpExecute_call(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* func = avmTranslateOperand(avm, &instr->m_Res, &avm->m_Registers[Reg_AX]);
	assert(func && (avmMemCellIsStack(avm, func) || avmMemCellIsRegister(avm, func)));

	// NOTE: Dont call avmCallSaveEnv() at this point because if the operand is a table we have to 
	// check if it has the "()" key and push the table on the stack.

	switch (func->m_Type) {
	case AVMMemCell_UserFunction:
	{
		avmCallSaveEnv(avm);

		AVMUserFunc* userFunc = func->m_Data.m_UserFunc;
		assert(userFunc->m_Address < avm->m_NumInstructions);
		assert(avm->m_Instructions[userFunc->m_Address].m_OpCode == AVMOp_FuncEnter);
		avm->m_PC = userFunc->m_Address;
		break;
	}
	case AVMMemCell_LibraryFunction:
		avmCallSaveEnv(avm);

		assert(func->m_Data.m_LibraryFunc != NULL);
		avmCallLibraryFunc(avm, func->m_Data.m_LibraryFunc->m_Callback);
		break;
	case AVMMemCell_String:
	{
		AVMLibraryFuncCallback libFunc = avmGetLibraryFunctionPointer(func->m_Data.m_String);
		if (!libFunc) {
			avmError(avm, "Library function \"%s\" not found.", func->m_Data.m_String);
		} else {
			avmCallSaveEnv(avm);
			avmCallLibraryFunc(avm, libFunc);
		}
		break;
	}
	case AVMMemCell_Table:
	{
		// Check if the table has an element for "()" and if it's a function. If it is call it.
		AVMMemoryCell funcOperator;
		funcOperator.m_Data.m_String = "()";
		funcOperator.m_Type = AVMMemCell_String;
		AVMMemoryCell* element = avmTableGetElement(avm, func->m_Data.m_TableHandle, &funcOperator);
		if (!element) {
			avmError(avm, "Key \"()\" not found in table");
		} else {
			if (element->m_Type != AVMMemCell_UserFunction) {
				avmError(avm, "Key \"()\" should be a user function");
			} else {
				// Implicit PARAM table
				avmStackPush(avm, func);
				++avm->m_TotalFunctionArguments;

				avmCallSaveEnv(avm);

				AVMUserFunc* userFunc = element->m_Data.m_UserFunc;
				assert(userFunc->m_Address < avm->m_NumInstructions);
				assert(avm->m_Instructions[userFunc->m_Address].m_OpCode == AVMOp_FuncEnter);
				avm->m_PC = userFunc->m_Address;
			}
		}
		break;
	}
	default:
	{
		char* str = avmMemCellToString(func);
		avmError(avm, "Cannot bind \"%s\" to function.", str);
		free(str);
		break;
	}
	}
}

void avmOpExecute_pusharg(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* arg = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	assert(arg && (avmMemCellIsStack(avm, arg) || avmMemCellIsRegister(avm, arg)));

	avmStackPush(avm, arg);
	++avm->m_TotalFunctionArguments;
}

void avmOpExecute_funcenter(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* func = avmTranslateOperand(avm, &instr->m_Res, &avm->m_Registers[Reg_AX]);
	assert(func && (avmMemCellIsStack(avm, func) || avmMemCellIsRegister(avm, func)));
	assert(func->m_Data.m_UserFunc->m_Address == avm->m_PC);

	avm->m_TotalFunctionArguments = 0;
	avm->m_TopSP = avm->m_Top;
	avm->m_Top -= func->m_Data.m_UserFunc->m_NumLocals;
}

void avmOpExecute_funcexit(AVM* avm, AVMInstruction* /*instr*/)
{
	unsigned int oldTop = avm->m_Top;
	avm->m_Top = avmStackGetEnvValue(avm, AVM_SAVED_TOP_OFFSET);
	avm->m_PC = avmStackGetEnvValue(avm, AVM_SAVED_PC_OFFSET);
	avm->m_TopSP = avmStackGetEnvValue(avm, AVM_SAVED_TOPSP_OFFSET);

	// Gargage collection
	while (++oldTop <= avm->m_Top) {
		avmMemCellClear(avm, &avm->m_Stack[oldTop]);
	}
}

void avmOpExecute_tablecreate(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* lv = avmTranslateOperand(avm, &instr->m_Res, NULL);

	assert(lv && (avmMemCellIsStack(avm, lv) || avmMemCellIsRetVal(avm, lv)));

	avmMemCellClear(avm, lv);

	lv->m_Type = AVMMemCell_Table;
	lv->m_Data.m_TableHandle = avmTableCreate(avm);
	avmTableIncRefCount(avm, lv->m_Data.m_TableHandle);
}

void avmOpExecute_tablegetelem(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* lv = avmTranslateOperand(avm, &instr->m_Res, NULL);
	AVMMemoryCell* table = avmTranslateOperand(avm, &instr->m_Arg1, NULL);
	AVMMemoryCell* index = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_AX]);

	assert(lv && (avmMemCellIsStack(avm, lv) || avmMemCellIsRetVal(avm, lv)));
	assert(table && avmMemCellIsStack(avm, table));
	assert(index && (avmMemCellIsStack(avm, index) || avmMemCellIsRegister(avm, index)));

	avmMemCellClear(avm, lv);

	if (table->m_Type != AVMMemCell_Table) {
		avmError(avm, "Illegal use of type %s as table.", avmMemCellTypeToString(table->m_Type));
	} else {
		AVMMemoryCell* content = avmTableGetElement(avm, table->m_Data.m_TableHandle, index);
		if (content) {
			avmMemCellAssign(avm, lv, content);
		} else {
			char* ts = avmMemCellToString(table);
			char* is = avmMemCellToString(index);
			avmWarning(avm, "%s[%s] not found", ts, is);
			free(ts);
			free(is);
		}
	}
}

void avmOpExecute_tablesetelem(AVM* avm, AVMInstruction* instr)
{
	AVMMemoryCell* table = avmTranslateOperand(avm, &instr->m_Res, NULL);
	AVMMemoryCell* index = avmTranslateOperand(avm, &instr->m_Arg1, &avm->m_Registers[Reg_AX]);
	AVMMemoryCell* content = avmTranslateOperand(avm, &instr->m_Arg2, &avm->m_Registers[Reg_BX]);

	assert(table && avmMemCellIsStack(avm, table));
	assert(index && (avmMemCellIsStack(avm, index) || avmMemCellIsRegister(avm, index)));
	assert(content && (avmMemCellIsStack(avm, content) || avmMemCellIsRegister(avm, content)));

	if (table->m_Type != AVMMemCell_Table) {
		avmError(avm, "Illegal use of type %s as table.", avmMemCellTypeToString(table->m_Type));
	} else {
		avmTableSetElement(avm, table->m_Data.m_TableHandle, index, content);
	}
}

void avmOpExecute_nop(AVM* avm, AVMInstruction* instr)
{
	// Nothing to do here!
}
