#include "avm_lib.h"
#include "avm.h"
#include "avm_stack.h"
#include "avm_table.h"
#include "avm_memory.h"
#include "avm_helpers.h"
#include "avm_op.h" // avmOpExecute_funcexit()
#include <string.h> // strcmp()
#include <math.h>   // atof(), sin, cos, sqrt
#include <stdio.h>  // fprintf(), stdout, stdin
#include <malloc.h> // free()
#include <assert.h> // assert()

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

// totalarguments()
void avmlibTotalArguments(AVM* avm)
{
	AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
	avmMemCellClear(avm, retVal);

	// After AVM initialization TopSP is 0 because there's no call frame on the stack.
	// We are currently inside the totalarguments()'s call frame, so we have to check
	// the saved TopSP to see if we are inside a function.
	unsigned int prevTopSP = avmStackGetEnvValue(avm, AVM_SAVED_TOPSP_OFFSET);
	if (prevTopSP == 0) {
		avmWarning(avm, "\"totalarguments\" called outside of a function");
		retVal->m_Type = AVMMemCell_Nil;
	} else {
		// Temporarily switch current TopSP with previous TopSP because avmStackGetEnvValue()
		// gets an index relative to the current TopSP.
		unsigned int curTopSP = avm->m_TopSP;
		avm->m_TopSP = prevTopSP;
		{
			// Get the number of arguments of the caller.
			retVal->m_Type = AVMMemCell_Number;
			retVal->m_Data.m_Number = avmStackGetEnvValue(avm, AVM_NUM_FUNCTION_ARGS_OFFSET);
		}
		// Switch TopSP to its correct value before returning.
		avm->m_TopSP = curTopSP;
	}
}

// typeof(any)
void avmlibTypeOf(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "\"typeof\" expects only 1 argument.");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);

		AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
		avmMemCellClear(avm, retVal);

		retVal->m_Type = AVMMemCell_String;
		retVal->m_Data.m_String = strdup(avmMemCellTypeToString(arg->m_Type));
	}
}

// argument(num)
void avmlibArgument(AVM* avm)
{
	AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
	avmMemCellClear(avm, retVal);

	unsigned int prevTopSP = avmStackGetEnvValue(avm, AVM_SAVED_TOPSP_OFFSET);
	if (prevTopSP == 0) {
		avmWarning(avm, "\"argument\" called outside of a function");
		retVal->m_Type = AVMMemCell_Nil;
	} else {
		unsigned int numArgs = avmGetTotalFunctionArguments(avm);
		if (numArgs != 1) {
			avmError(avm, "\"argument\" expects only 1 argument.");
		} else {
			AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);

			if (arg->m_Type != AVMMemCell_Number || !isInteger(arg->m_Data.m_Number)) {
				avmError(avm, "Argument #0 of \"argument\" should be an integer");
			} else {
				const unsigned int argID = (unsigned int)arg->m_Data.m_Number;

				// Temporarily switch current TopSP with previous TopSP because avmGetFunctionArgument()
				// expects TopSP to point to the frame we are interested in (and at this point we are 
				// interested in the frame of the caller and not the frame of argument()).
				unsigned int curTopSP = avm->m_TopSP;
				avm->m_TopSP = prevTopSP;
				{
					// Check if the requested argument is valid because otherwise avmGetFunctionArgument() will 
					// assert.
					unsigned int numCallerArgs = avmGetTotalFunctionArguments(avm);
					if (argID >= numCallerArgs) {
						// NOTE: Assume this isn't an error and return UNDEFINED.
						avmWarning(avm, "Tried to access invalid function argument #%d", argID);

						retVal->m_Type = AVMMemCell_Undefined;
					} else {
						AVMMemoryCell* callerArg = avmGetFunctionArgument(avm, argID);
						avmMemCellAssign(avm, retVal, callerArg);
					}
				}
				// Switch TopSP to its correct value before returning.
				avm->m_TopSP = curTopSP;
			}
		}
	}
}

// print(...)
void avmlibPrint(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	for (unsigned int i = 0; i < numArgs; ++i) {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, i);

		char* str = NULL;
		if (arg->m_Type == AVMMemCell_Table) {
			str = avmTableToString(avm, arg->m_Data.m_TableHandle);
		} else {
			str = avmMemCellToString(arg);
		}

		assert(str);
		fprintf(stdout, "%s", str);
		free(str);
	}
}

// input()
void avmlibInput(AVM* avm)
{
	char buf[512];
	fgets(buf, 512, stdin);

	// Remove \n from the buffer.
	unsigned int len = strlen(buf);
	if (buf[len - 1] == '\n') {
		buf[len - 1] = 0;
		--len;
	}

	AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
	avmMemCellClear(avm, retVal);

	if (len == 0) {
		retVal->m_Type = AVMMemCell_Undefined;
	} else if (!strcmp(buf, "true")) {
		retVal->m_Type = AVMMemCell_Bool;
		retVal->m_Data.m_Bool = true;
	} else if (!strcmp(buf, "false")) {
		retVal->m_Type = AVMMemCell_Bool;
		retVal->m_Data.m_Bool = false;
	} else if (!strcmp(buf, "nil")) {
		retVal->m_Type = AVMMemCell_Nil;
	} else if (isNumber(buf)) {
		retVal->m_Type = AVMMemCell_Number;
		retVal->m_Data.m_Number = atof(buf);
	} else {
		retVal->m_Type = AVMMemCell_String;
		retVal->m_Data.m_String = strdup(buf);
	}
}

// strtonum(str)
void avmlibStrToNum(AVM* avm)
{
	AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
	avmMemCellClear(avm, retVal);

	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "Only 1 argument expected in \"strtonum\"");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);
		if (arg->m_Type != AVMMemCell_String) {
			avmWarning(avm, "Argument #0 of \"strtonum\" should be a string");
			retVal->m_Type = AVMMemCell_Nil;
		} else {
			if (!isNumber(arg->m_Data.m_String)) {
				avmWarning(avm, "\"strtonum\" cannot convert \"%s\" to number", arg->m_Data.m_String);
				retVal->m_Type = AVMMemCell_Nil;
			} else {
				retVal->m_Type = AVMMemCell_Number;
				retVal->m_Data.m_Number = atof(arg->m_Data.m_String);
			}
		}
	}
}

// sqrt(num)
void avmlibSqrt(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "Only 1 argument expected in \"sqrt\"");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);
		if (arg->m_Type != AVMMemCell_Number) {
			avmError(avm, "Argument #0 of \"sqrt\" should be a number");
		} else {
			AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
			avmMemCellClear(avm, retVal);

			if (arg->m_Data.m_Number < 0) {
				avmWarning(avm, "sqrt(%g) is undefined", arg->m_Data.m_Number);
				retVal->m_Type = AVMMemCell_Nil;
			} else {
				retVal->m_Type = AVMMemCell_Number;
				retVal->m_Data.m_Number = sqrt(arg->m_Data.m_Number);
			}
		}
	}
}

// cos(num)
void avmlibCos(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "Only 1 argument expected in \"cos\"");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);
		if (arg->m_Type != AVMMemCell_Number) {
			avmError(avm, "Argument #0 of \"cos\" should be a number");
		} else {
			AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
			avmMemCellClear(avm, retVal);

			retVal->m_Type = AVMMemCell_Number;
			retVal->m_Data.m_Number = cos(arg->m_Data.m_Number * PI / 180.0);
		}
	}
}

// sin(num)
void avmlibSin(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "Only 1 argument expected in \"sin\"");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);
		if (arg->m_Type != AVMMemCell_Number) {
			avmError(avm, "Argument #0 of \"sin\" should be a number");
		} else {
			AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
			avmMemCellClear(avm, retVal);

			retVal->m_Type = AVMMemCell_Number;
			retVal->m_Data.m_Number = sin(arg->m_Data.m_Number * PI / 180.0);
		}
	}
}

// objecttotalmembers(table)
void avmlibObjectTotalMembers(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "Only 1 argument expected in \"objecttotalmembers\"");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);
		if (arg->m_Type != AVMMemCell_Table) {
			avmError(avm, "Argument #0 of \"objecttotalmembers\" should be a table");
		} else {
			AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
			avmMemCellClear(avm, retVal);

			retVal->m_Type = AVMMemCell_Number;
			retVal->m_Data.m_Number = avmTableCountMembers(avm, arg->m_Data.m_TableHandle);
		}
	}
}

// objectmemberkeys(table)
void avmlibObjectMemberKeys(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "Only 1 argument expected in \"objecttotalmembers\"");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);
		if (arg->m_Type != AVMMemCell_Table) {
			avmError(avm, "Argument #0 of \"objecttotalmembers\" should be a table");
		} else {
			AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
			avmMemCellClear(avm, retVal);

			unsigned int indexTableHandle = avmTableExtractIndices(avm, arg->m_Data.m_TableHandle);

			retVal->m_Type = AVMMemCell_Table;
			retVal->m_Data.m_TableHandle = indexTableHandle;
		}
	}
}

// objectcopy(table)
void avmlibObjectCopy(AVM* avm)
{
	unsigned int numArgs = avmGetTotalFunctionArguments(avm);
	if (numArgs != 1) {
		avmError(avm, "Only 1 argument expected in \"objecttotalmembers\"");
	} else {
		AVMMemoryCell* arg = avmGetFunctionArgument(avm, 0);
		if (arg->m_Type != AVMMemCell_Table) {
			avmError(avm, "Argument #0 of \"objecttotalmembers\" should be a table");
		} else {
			AVMMemoryCell* retVal = &avm->m_Registers[Reg_RetVal];
			avmMemCellClear(avm, retVal);

			unsigned int copyHandle = avmTableCopy(avm, arg->m_Data.m_TableHandle);

			retVal->m_Type = AVMMemCell_Table;
			retVal->m_Data.m_TableHandle = copyHandle;
		}
	}
}

AVMLibraryFuncCallback avmGetLibraryFunctionPointer(const char* name)
{
	if (!strcmp(name, "print")) {
		return avmlibPrint;
	} else if (!strcmp(name, "input")) {
		return avmlibInput;
	} else if (!strcmp(name, "objectmemberkeys")) {
		return avmlibObjectMemberKeys;
	} else if (!strcmp(name, "objecttotalmembers")) {
		return avmlibObjectTotalMembers;
	} else if (!strcmp(name, "objectcopy")) {
		return avmlibObjectCopy;
	} else if (!strcmp(name, "totalarguments")) {
		return avmlibTotalArguments;
	} else if (!strcmp(name, "argument")) {
		return avmlibArgument;
	} else if (!strcmp(name, "typeof")) {
		return avmlibTypeOf;
	} else if (!strcmp(name, "strtonum")) {
		return avmlibStrToNum;
	} else if (!strcmp(name, "sqrt")) {
		return avmlibSqrt;
	} else if (!strcmp(name, "cos")) {
		return avmlibCos;
	} else if (!strcmp(name, "sin")) {
		return avmlibSin;
	}

	return NULL;
}

void avmCallLibraryFunc(AVM* avm, AVMLibraryFuncCallback f)
{
	if (!f) {
		avmError(avm, "Cannot invoke unknown library function!");
	} else {
		// FuncEnter
		avm->m_TopSP = avm->m_Top;
		avm->m_TotalFunctionArguments = 0;

		f(avm);

		// FuncExit
		if (!avm->m_ExecutionFinished) {
			avmOpExecute_funcexit(avm, NULL);
		}
	}
}
