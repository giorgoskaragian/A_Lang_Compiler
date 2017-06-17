#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct LinkedListNode
{
	LinkedListNode* m_Next;
	LinkedListNode* m_Prev;
	void* m_Item;
} ;

struct LinkedList
{
	LinkedListNode* m_Head;
	LinkedListNode* m_Tail;
};

LinkedList* llCreate();
void llDestroy(LinkedList* ll);

void llPushBack(LinkedList* list, void* item);

#endif
