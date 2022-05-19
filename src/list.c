#include "list.h"

node_l* insertNode(node_l *head,node_t *current) {
	node_l *newNode = (node_l*)malloc(sizeof(node_l));
	newNode->current = current;
	newNode->next = NULL;
	if(head==NULL) {
		head = newNode;
	} else {
		newNode->next = head;
		head = newNode;
	}
	return head;
}
void freeList(node_l *head) {
	node_l *tmp = head;
	while(tmp!=NULL) {
		free(tmp->current);
		node_l* n_next = tmp->next;
		free(tmp);
		tmp = n_next;
	}
}
