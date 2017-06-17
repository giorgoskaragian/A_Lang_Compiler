#ifndef AVM_MEMORY_H
#define AVM_MEMORY_H

struct AVM;
struct AVMUserFunc;
struct AVMLibraryFunc;

enum AVMMemCellType
{
	AVMMemCell_Number = 0,
	AVMMemCell_String,
	AVMMemCell_Bool,
	AVMMemCell_Table,
	AVMMemCell_Nil,
	AVMMemCell_UserFunction,
	AVMMemCell_LibraryFunction,
	AVMMemCell_Undefined
};

struct AVMMemoryCell
{
	AVMMemCellType m_Type;
	union _Data
	{
		double m_Number;
		char* m_String;
		bool m_Bool;
		unsigned int m_TableHandle;
		AVMUserFunc* m_UserFunc;
		AVMLibraryFunc* m_LibraryFunc;
	} m_Data;
};

// Memory functions
void avmMemCellInit(AVMMemoryCell* cell);
void avmMemCellClear(AVM* avm, AVMMemoryCell* cell);
void avmMemCellAssign(AVM* avm, AVMMemoryCell* lv, const AVMMemoryCell* rv);
char* avmMemCellToString(const AVMMemoryCell* cell);
bool avmMemCellToBool(const AVMMemoryCell* cell);
const char* avmMemCellTypeToString(AVMMemCellType type);

bool avmMemCellIsStack(AVM* avm, AVMMemoryCell* mc);
bool avmMemCellIsRetVal(AVM* avm, AVMMemoryCell* mc);
bool avmMemCellIsRegister(AVM* avm, AVMMemoryCell* mc);

#endif
