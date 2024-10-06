# list.c
#### list_merge 
Merges two lists into one. The function should return a new list that contains all the elements of the two input lists.
```c
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
```