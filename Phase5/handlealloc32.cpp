#include "handlealloc32.h"
#include <malloc.h>

#define HANDLE_CAPACITY_DELTA 1024

HandleAlloc32::HandleAlloc32() : 
	m_FreeList(0),
	m_Capacity(0)
{
}

HandleAlloc32::~HandleAlloc32()
{
	FreeListNode* node = m_FreeList;
	while (node) {
		FreeListNode* next = node->m_Next;
		free(node);
		node = next;
	}
	m_FreeList = 0;
}

unsigned int HandleAlloc32::allocHandle()
{
	// Find first free list slot which can hold n handles.
	FreeListNode* node = m_FreeList;
	while (node) {
		if (node->m_NumFreeHandles >= 1) {
			// Found. Allocate n handles from this node.
			const unsigned int firstHandleID = node->m_FirstHandleID;

			node->m_NumFreeHandles -= 1;
			if (node->m_NumFreeHandles == 0) {
				// No more free handles in the node. Remove it from the free list.
				removeFreeListNode(node);
				free(node);
			} else {
				// There are more handles in this free node. Just increase the first handle index.
				node->m_FirstHandleID += 1;
			}

			return firstHandleID;
		}

		node = node->m_Next;
	}

	// No free slot found to hold n handles.
	const unsigned int firstHandleID = m_Capacity;

	FreeListNode* newNode = (FreeListNode*)malloc(sizeof(FreeListNode));
	newNode->m_FirstHandleID = m_Capacity + 1;
	newNode->m_NumFreeHandles = HANDLE_CAPACITY_DELTA - 1;
	newNode->m_Next = 0;
	newNode->m_Prev = 0;
	m_Capacity += HANDLE_CAPACITY_DELTA;

	insertFreeListNode(newNode);

	return firstHandleID;
}

void HandleAlloc32::freeHandle(unsigned int handle)
{
	// Check if we can merge these handles with an existing free list node.
	const unsigned int endHandleID = handle + 1;

	FreeListNode* node = m_FreeList;
	while (node) {
		if (node->m_FirstHandleID == endHandleID) {
			// Merge after.
			node->m_FirstHandleID = handle;
			node->m_NumFreeHandles += 1;
			return;
		} else if (node->m_FirstHandleID + node->m_NumFreeHandles == handle) {
			// Merge before.
			node->m_NumFreeHandles += 1;
			return;
		}

		node = node->m_Next;
	}

	// Cannot merge these handles with an existing free list node. Create a new node
	// and insert it to the list.
	FreeListNode* newNode = (FreeListNode*)malloc(sizeof(FreeListNode));
	newNode->m_FirstHandleID = handle;
	newNode->m_NumFreeHandles = 1;
	newNode->m_Next = 0;
	newNode->m_Prev = 0;

	insertFreeListNode(newNode);
}
