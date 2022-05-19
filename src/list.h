#ifndef __LIST_PEG__
#define __LIST_PEG__

#include "utils.h"

typedef struct listnode {
    node_t* current;
    struct listnode* next;
} node_l;

node_l* insertNode(node_l *head,node_t *current);
void freeList(node_l *head);

#endif