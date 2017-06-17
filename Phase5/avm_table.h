#ifndef AVM_TABLE_H
#define AVM_TABLE_H

struct AVM;
struct AVMMemoryCell;

#define INVALID_TABLE_HANDLE (~0u)

void avmInitTables(AVM* avm);
void avmDestroyTables(AVM* avm);

unsigned int avmTableCreate(AVM* avm);

char* avmTableToString(AVM* avm, unsigned int tableHandle);

AVMMemoryCell* avmTableGetElement(AVM* avm, unsigned int tableHandle, const AVMMemoryCell* index);
void avmTableSetElement(AVM* avm, unsigned int tableHandle, const AVMMemoryCell* index, const AVMMemoryCell* content);

void avmTableIncRefCount(AVM* avm, unsigned int tableHandle);
void avmTableDecRefCount(AVM* avm, unsigned int tableHandle);

unsigned int avmTableCountMembers(AVM* avm, unsigned int tableHandle);
unsigned int avmTableExtractIndices(AVM* avm, unsigned int tableHandle);
unsigned int avmTableCopy(AVM* avm, unsigned int tableHandle);

#endif
