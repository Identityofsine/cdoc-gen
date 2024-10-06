#pragma once
#include <stdlib.h>

typedef struct Node Node; // Forward declaration of Node

typedef struct Node {
  Node *next;
  Node *prev;
  void *data;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  size_t size;
} List;

List *list_new();
List *list_merge(List *list1, List *list2);
void list_push(List *list, void *data);
void list_pop(List *list);
Node* list_get(List *list, size_t index);
void list_set(List *list, size_t index, void *data);
Node *list_remove(List *list, size_t index);

void **list_to_array(List *list, unsigned long size);
