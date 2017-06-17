#include "avm_stack.h"
#include "avm_memory.h"
#include "avm.h"
#include "avm_helpers.h"
#include <assert.h>

void avmStackPush(AVM* avm, const AVMMemoryCell* cell)
{
	if (avm->m_Top == 0) {
		avmError(avm, "Stack overflow");
		return;
	}

	avmMemCellAssign(avm, &avm->m_Stack[avm->m_Top], cell);
	--avm->m_Top;
}

void avmStackPushEnvValue(AVM* avm, unsigned int val)
{
	AVMMemoryCell tmp;
	tmp.m_Type = AVMMemCell_Number;
	tmp.m_Data.m_Number = val;
	avmStackPush(avm, &tmp);
}

unsigned int avmStackGetEnvValue(AVM* avm, unsigned int topSPRelativeIndex)
{
	assert(avm->m_TopSP + topSPRelativeIndex < avm->m_StackSize);
	AVMMemoryCell* cell = &avm->m_Stack[avm->m_TopSP + topSPRelativeIndex];

	assert(cell->m_Type == AVMMemCell_Number);
	assert(isInteger(cell->m_Data.m_Number));
	return (unsigned int)cell->m_Data.m_Number;
}
