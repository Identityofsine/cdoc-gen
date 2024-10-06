#include "list.h"
#include "../util/util.h"

List *list_new() {
  List *list = fxMemAlloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  return list;
}

void list_push(List *list, void *data) {
  Node *node = fxMemAlloc(sizeof(Node));
  node->data = data;
  node->next = NULL;
  // Ignore warning
  node->prev = list->tail;
  if (list->head == NULL) {
    list->head = node;
  } else if (list->tail != NULL) {
    list->tail->next = node;
    list->tail = node;
  } else {
    list->tail = node;
    (list->head)->next = list->tail;
  }
  list->size++;
}

void **list_to_array(List *list, unsigned long size) {
  size_t t_size = size * (list->size + 1);
  void **array = fxMemAlloc(t_size);
  Node *node = list->head;
  for (size_t i = 0; i < list->size; i++) {
    if (node == NULL) {
      printf("Warning: Node(%d) is NULL\n", i);
      continue;
    }
    array[i] = node->data;
    node = node->next;
  }
  array[list->size] = NULL;
  return array;
}

//@BLOCK
//@TITLE list_merge 
//@DESC Merges two lists into one. The function should return a new list that contains all the elements of the two input lists.
List* list_merge(List* list_1, List* list_2) {
	List* merged_list = list_new();
	Node* node = list_1->head;	
	while(node != NULL) {
		list_push(merged_list, node->data);
		node = node->next;
	}
	node = list_2->head;
	while(node != NULL) {
		list_push(merged_list, node->data);
		node = node->next;
	}
	return merged_list;
}
//@END

Node* list_get(List *list, size_t index) {
	if (index >= list->size) {
		return NULL;
	}
	Node *node = list->head;
	for (size_t i = 0; i < index; i++) {
		node = node->next;
	}
	return node;
}
