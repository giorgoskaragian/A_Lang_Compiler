#include "avm_table.h"
#include "avm.h"
#include "avm_memory.h"
#include "avm_helpers.h"
#include "handlealloc32.h"
#include <malloc.h> // realloc()/free()
#include <memory.h> // memset()
#include <assert.h>
#include <map>
#include <string>

struct CompareMemoryCells
{
	bool operator () (const AVMMemoryCell& a, const AVMMemoryCell& b)
	{
		if (a.m_Type != b.m_Type) {
			return a.m_Type < b.m_Type;
		} else {
			// Cell types are identical. Compare values.
			switch (a.m_Type) {
			case AVMMemCell_Number:
				return a.m_Data.m_Number < b.m_Data.m_Number;
			case AVMMemCell_String:
				return strcmp(a.m_Data.m_String, b.m_Data.m_String) == -1;
			case AVMMemCell_Bool:
				return !a.m_Data.m_Bool && b.m_Data.m_Bool;
			case AVMMemCell_Table:
				return a.m_Data.m_TableHandle < b.m_Data.m_TableHandle;
			case AVMMemCell_Nil:
				assert(false); // Cannot have nil keys
				break;
			case AVMMemCell_UserFunction:
				return a.m_Data.m_UserFunc->m_Address < b.m_Data.m_UserFunc->m_Address;
			case AVMMemCell_LibraryFunction:
				return a.m_Data.m_LibraryFunc < b.m_Data.m_LibraryFunc;
			case AVMMemCell_Undefined:
				assert(false); // Cannot have undefined keys
				break;
			}

			return false;
		}
	}
};

typedef std::map<AVMMemoryCell, AVMMemoryCell, CompareMemoryCells> Dictionary;

struct AVMTable
{
	Dictionary m_Dict;
	unsigned int m_RefCount;

	AVMTable() : m_RefCount(0)
	{}
};

void avmInitTables(AVM* avm)
{
	avm->m_Tables = NULL;
	avm->m_TableCapacity = 0;
	avm->m_TableHandleAllocator = new HandleAlloc32();
}

void avmDestroyTables(AVM* avm)
{
	delete avm->m_TableHandleAllocator;
	avm->m_TableHandleAllocator = NULL;

	for (unsigned int i = 0; i < avm->m_TableCapacity; ++i) {
		if (avm->m_Tables[i]) {
			delete avm->m_Tables[i];
		}
	}

	free(avm->m_Tables);
	avm->m_Tables = NULL;

	avm->m_TableCapacity = 0;
}

unsigned int avmTableCreate(AVM* avm)
{
	unsigned int tableHandle = avm->m_TableHandleAllocator->allocHandle();
	if (tableHandle >= avm->m_TableCapacity) {
		unsigned int oldCapacity = avm->m_TableCapacity;

		avm->m_TableCapacity = avm->m_TableHandleAllocator->getCapacity();
		avm->m_Tables = (AVMTable**)realloc(avm->m_Tables, sizeof(AVMTable*) * avm->m_TableCapacity);
		
		memset(&avm->m_Tables[oldCapacity], 0, sizeof(AVMTable*) * (avm->m_TableCapacity - oldCapacity));
	}

	// Get the table.
	AVMTable* table = avm->m_Tables[tableHandle];
	if (!table) {
		// This is the first time we touch this handle. Allocate a new table.
		// NOTE: This is the only place we are using C++ new operator because the table structure 
		// has a non-POD type (std::map).
		table = new AVMTable();

		// Put it back in the array.
		avm->m_Tables[tableHandle] = table;
	} else {
		// Table already exists.
		// Make sure this is an empty unreferenced table.
		assert(table->m_RefCount == ~0);
		assert(table->m_Dict.empty());
		table->m_RefCount = 0;
	}

	return tableHandle;
}

AVMMemoryCell* avmTableGetElement(AVM* avm, unsigned int tableHandle, const AVMMemoryCell* index)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount > 0);

	if (index->m_Type != AVMMemCell_Number &&
		index->m_Type != AVMMemCell_String &&
		index->m_Type != AVMMemCell_Bool &&
		index->m_Type != AVMMemCell_Table &&
		index->m_Type != AVMMemCell_UserFunction &&
		index->m_Type != AVMMemCell_LibraryFunction) 
	{
		avmError(avm, "Cannot use %s as table index", avmMemCellTypeToString(index->m_Type));
		return NULL;
	}
	
	Dictionary::iterator prevInstance = table->m_Dict.find(*index);
	if (prevInstance == table->m_Dict.end()) {
		// Key doesn't exist.
		return NULL;
	}

	return &prevInstance->second;
}

void avmTableSetElement(AVM* avm, unsigned int tableHandle, const AVMMemoryCell* index, const AVMMemoryCell* content)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount > 0);

	if (index->m_Type != AVMMemCell_Number &&
		index->m_Type != AVMMemCell_String &&
		index->m_Type != AVMMemCell_Bool &&
		index->m_Type != AVMMemCell_Table &&
		index->m_Type != AVMMemCell_UserFunction &&
		index->m_Type != AVMMemCell_LibraryFunction) {
		avmError(avm, "Cannot use %s as table index", avmMemCellTypeToString(index->m_Type));
		return;
	}

	Dictionary::iterator prevInstance = table->m_Dict.find(*index);
	if (prevInstance != table->m_Dict.end()) {
		if (content->m_Type == AVMMemCell_Nil) {
			// Remove key from the map.
			table->m_Dict.erase(prevInstance);
		} else {
			// Change the key.
			avmMemCellAssign(avm, &prevInstance->second, content);
		}
	} else {
		if (content->m_Type == AVMMemCell_Nil) {
			avmWarning(avm, "Trying to set undefined table key to nil");
		} else {
			AVMMemoryCell val;
			avmMemCellInit(&val);
			avmMemCellAssign(avm, &val, content);

			AVMMemoryCell key;
			avmMemCellInit(&key);
			avmMemCellAssign(avm, &key, index);

			table->m_Dict.insert(Dictionary::value_type(key, val));
		}
	}
}

void avmTableIncRefCount(AVM* avm, unsigned int tableHandle)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount != ~0u);

	++table->m_RefCount;
}

void avmTableDecRefCount(AVM* avm, unsigned int tableHandle)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount > 0);

	--table->m_RefCount;
	if (table->m_RefCount == 0) {
		// Clear all memory cells.
		Dictionary::iterator it, itLast = table->m_Dict.end();
		for (it = table->m_Dict.begin(); it != itLast; ++it) {
			avmMemCellClear(avm, &it->second);
		}

		// Clear the dictionary
		table->m_Dict.clear();

		// Mark the table as unused.
		table->m_RefCount = ~0u;

		// Free the handle
		avm->m_TableHandleAllocator->freeHandle(tableHandle);
	}
}

unsigned int avmTableExtractIndices(AVM* avm, unsigned int tableHandle)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount > 0);

	unsigned int indexTableHandle = avmTableCreate(avm);
	avmTableIncRefCount(avm, indexTableHandle); // In order to shut asserts in avmTableSetElement.

	unsigned int i = 0;
	Dictionary::iterator it, itLast = table->m_Dict.end();
	for (it = table->m_Dict.begin(); it != itLast; ++it, ++i) {
		AVMMemoryCell id;
		avmMemCellInit(&id);

		id.m_Type = AVMMemCell_Number;
		id.m_Data.m_Number = i;

		avmTableSetElement(avm, indexTableHandle, &id, &it->first);
	}

	return indexTableHandle;
}

unsigned int avmTableCountMembers(AVM* avm, unsigned int tableHandle)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount > 0);

	return (unsigned int)table->m_Dict.size();
}

unsigned int avmTableCopy(AVM* avm, unsigned int tableHandle)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount > 0);

	unsigned int copyTableHandle = avmTableCreate(avm);
	avmTableIncRefCount(avm, copyTableHandle); // In order to shut asserts in avmTableSetElement.

	unsigned int i = 0;
	Dictionary::iterator it, itLast = table->m_Dict.end();
	for (it = table->m_Dict.begin(); it != itLast; ++it, ++i) {
		avmTableSetElement(avm, copyTableHandle, &it->first, &it->second);
	}

	return copyTableHandle;
}

char* avmTableToString(AVM* avm, unsigned int tableHandle)
{
	assert(tableHandle < avm->m_TableCapacity);
	AVMTable* table = avm->m_Tables[tableHandle];
	assert(table);
	assert(table->m_RefCount > 0);

	std::string buf;
	buf.append("[");

	bool appendComma = false;
	Dictionary::iterator it, itLast = table->m_Dict.end();
	for (it = table->m_Dict.begin(); it != itLast; ++it) {
		const AVMMemoryCell& key = it->first;
		const AVMMemoryCell& val = it->second;

		if (appendComma) {
			buf.append(", ");
		}

		char* keyStr = avmMemCellToString(&key);
		buf.append(keyStr);
		buf.append(" : ");
		free(keyStr);

		if (val.m_Type == AVMMemCell_Table) {
			char* tableStr = avmTableToString(avm, val.m_Data.m_TableHandle);
			buf.append(tableStr);
			free(tableStr);
		} else {
			char* valStr = avmMemCellToString(&val);
			buf.append(valStr);
			free(valStr);
		}

		appendComma = true;
	}

	buf.append("]");

	char* str = strdup(buf.c_str());
	return str;
}
