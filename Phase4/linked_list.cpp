#include "linked_list.h"
#include <malloc.h> // malloc(), free()
#include <memory.h> // memset()

LinkedList* llCreate()
{
	LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList));
	memset(ll, 0, sizeof(LinkedList));

	return ll;
}

void llDestroy(LinkedList* ll)
{
	LinkedListNode* node = ll->m_Head;
	while (node) {
		LinkedListNode* next = node->m_Next;
		free(node);
		node = next;
	}

	free(ll);
}

void llPushBack(LinkedList* list, void* item)
{
	LinkedListNode* node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	node->m_Item = item;
	node->m_Next = 0;
	node->m_Prev = list->m_Tail;
	if (list->m_Tail) {
		list->m_Tail->m_Next = node;
	}
	list->m_Tail = node;

	if (!list->m_Head) {
		list->m_Head = node;
	}
}

