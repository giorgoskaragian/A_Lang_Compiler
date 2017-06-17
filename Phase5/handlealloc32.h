#ifndef HANDLEALLOC32_H
#define HANDLEALLOC32_H

class HandleAlloc32
{
public:
	HandleAlloc32();
	~HandleAlloc32();

	unsigned int allocHandle();
	void freeHandle(unsigned int handle);

	unsigned int getCapacity() const;

private:
	struct FreeListNode
	{
		FreeListNode* m_Next;
		FreeListNode* m_Prev;
		unsigned int m_FirstHandleID;
		unsigned int m_NumFreeHandles;
	};

	FreeListNode* m_FreeList;
	unsigned int m_Capacity;

	void removeFreeListNode(FreeListNode* node)
	{
		if (node->m_Prev) {
			node->m_Prev->m_Next = node->m_Next;
		}

		if (node->m_Next) {
			node->m_Next->m_Prev = node->m_Prev;
		}

		if (node == m_FreeList) {
			m_FreeList = node->m_Next;
		}
	}

	void insertFreeListNode(FreeListNode* newNode)
	{
		// Find the correct position in the free list to insert the given node.
		const unsigned int newFirstHandleID = newNode->m_FirstHandleID;
		FreeListNode* node = m_FreeList;
		FreeListNode* prevNode = 0;
		while (node) {
			if (node->m_FirstHandleID > newFirstHandleID) {
				break;
			}

			prevNode = node;
			node = node->m_Next;
		}

		if (prevNode) {
			newNode->m_Next = prevNode->m_Next;
			newNode->m_Prev = prevNode;
			prevNode->m_Next = newNode;
		} else {
			newNode->m_Next = m_FreeList;
			newNode->m_Prev = 0;
			m_FreeList = newNode;
		}
	}
};

inline unsigned int HandleAlloc32::getCapacity() const
{
	return m_Capacity;
}

#endif
