#include "avm_memory.h"
#include "avm_helpers.h"
#include "avm_table.h"
#include "avm.h"
#include <malloc.h> // free()
#include <memory.h> // memcpy()
#include <string.h> // strdup()
#include <stdio.h>  // sprintf()
#include <assert.h> // assert()

void avmMemCellInit(AVMMemoryCell* cell)
{
	cell->m_Type = AVMMemCell_Undefined;
	cell->m_Data.m_Number = 0;
}

void avmMemCellClear(AVM* avm, AVMMemoryCell* cell)
{
	if (cell->m_Type == AVMMemCell_String) {
		assert(cell->m_Data.m_String);
		free(cell->m_Data.m_String);
		cell->m_Data.m_String = NULL;
	} else if (cell->m_Type == AVMMemCell_Table) {
		assert(cell->m_Data.m_TableHandle != INVALID_TABLE_HANDLE);
		avmTableDecRefCount(avm, cell->m_Data.m_TableHandle);
		cell->m_Data.m_TableHandle = INVALID_TABLE_HANDLE;
	}

	cell->m_Type = AVMMemCell_Undefined;
}

void avmMemCellAssign(AVM* avm, AVMMemoryCell* dst, const AVMMemoryCell* src)
{
	if (dst == src) {
		// Same memory cell.
		return;
	}

	if (dst->m_Type == AVMMemCell_Table && src->m_Type == AVMMemCell_Table &&
		dst->m_Data.m_TableHandle == src->m_Data.m_TableHandle) {
		// Same table.
		return;
	}

	if (src->m_Type == AVMMemCell_Undefined) {
		avmWarning(avm, "Reading from an undefined memory cell.");
	}

	avmMemCellClear(avm, dst);

	memcpy(dst, src, sizeof(AVMMemoryCell));

	if (dst->m_Type == AVMMemCell_String) {
		dst->m_Data.m_String = strdup(src->m_Data.m_String);
	} else if (dst->m_Type == AVMMemCell_Table) {
		avmTableIncRefCount(avm, src->m_Data.m_TableHandle);
	}
}

char* avmMemCellToString(const AVMMemoryCell* cell)
{
	char* str = NULL;
	const AVMMemCellType type = cell->m_Type;
	if (type == AVMMemCell_Number) {
		str = (char*)malloc(256);
		sprintf(str, "%g", cell->m_Data.m_Number);
	} else if (type == AVMMemCell_String) {
		str = strdup(cell->m_Data.m_String);
	} else if (type == AVMMemCell_Bool) {
		str = (char*)malloc(8);
		sprintf(str, "%s", cell->m_Data.m_Bool ? "TRUE" : "FALSE");
	} else if (type == AVMMemCell_Table) {
		str = (char*)malloc(32);
		sprintf(str, "TABLE[0x%08X]", cell->m_Data.m_TableHandle);
	} else if (type == AVMMemCell_Nil) {
		str = (char*)malloc(4);
		sprintf(str, "NIL");
	} else if (type == AVMMemCell_UserFunction) {
		str = (char*)malloc(256);
		sprintf(str, "USERFUNC[0x%08X]", cell->m_Data.m_UserFunc->m_Address);
	} else if (type == AVMMemCell_LibraryFunction) {
		str = (char*)malloc(256);
		sprintf(str, "LIBFUNC(%s)", cell->m_Data.m_LibraryFunc->m_Name);
	} else if (type == AVMMemCell_Undefined) {
		str = (char*)malloc(16);
		sprintf(str, "UNDEFINED");
	} else {
		assert(false); // Unknown memory cell type.
	}

	return str;
}

bool avmMemCellToBool(const AVMMemoryCell* cell)
{
	const AVMMemCellType type = cell->m_Type;
	if (type == AVMMemCell_Number) {
		return cell->m_Data.m_Number != 0;
	} else if (type == AVMMemCell_String) {
		return cell->m_Data.m_String[0] != 0;
	} else if (type == AVMMemCell_Bool) {
		return cell->m_Data.m_Bool;
	} else if (type == AVMMemCell_Table) {
		return true;
	} else if (type == AVMMemCell_Nil) {
		return false;
	} else if (type == AVMMemCell_UserFunction) {
		return true;
	} else if (type == AVMMemCell_LibraryFunction) {
		return true;
	} else if (type == AVMMemCell_Undefined) {
		assert(false);
	} else {
		assert(false); // Unknown memory cell type.
	}

	return false;
}

const char* avmMemCellTypeToString(AVMMemCellType type)
{
	static const char* typeStrings[] = {
		"number",
		"string",
		"bool",
		"table",
		"nil",
		"userfunc",
		"libfunc",
		"undefined"
	};

	assert(type >= AVMMemCell_Number && type <= AVMMemCell_Undefined);
	return typeStrings[type];
}

bool avmMemCellIsStack(AVM* avm, AVMMemoryCell* mc)
{
	return avm->m_Stack <= mc && &avm->m_Stack[avm->m_StackSize - 1] >= mc;
}

bool avmMemCellIsRetVal(AVM* avm, AVMMemoryCell* mc)
{
	return mc == &avm->m_Registers[Reg_RetVal];
}

bool avmMemCellIsRegister(AVM* avm, AVMMemoryCell* mc)
{
	return mc >= avm->m_Registers && mc <= &avm->m_Registers[Reg_NumRegisters - 1];
}
