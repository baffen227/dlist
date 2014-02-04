#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

struct _DListNode
{
    void *pdata;
    struct _DListNode *prev;
    struct _DListNode *next;
};

struct _DList
{
    struct _DListNode *head;
    size_t size;
};
typedef struct _DList DList;

enum _DListRet
{
    DLIST_RET_OK,
    DLIST_RET_FAIL
};
typedef enum _DListRet DListRet;

DList *dlist_create(void)
{
    DList *thiz = (DList *)malloc(sizeof(DList));
    if (thiz == NULL)
    {
        perror("dlist_create malloc() failed");
        exit(EXIT_FAILURE);
    }

    thiz->head = NULL;
    thiz->size = 0;

    return thiz;
}

void dlist_destroy(DList *thiz)
{
    while (thiz->size != 0)
    {
        dlist_delete(thiz, 0);
    }
    free(thiz);
}

size_t dlist_size(DList *thiz)
{
    return thiz->size; 
}

void dlist_foreach(DList *thiz, DListVisitFunc data_func, void *ctx)
{
    struct _DListNode *iter;
    unsigned int idx;

    iter = thiz->head;
    idx = 0;
    while (idx < thiz->size)
    {
        data_func(ctx, iter->pdata);

        iter = iter->next;
        idx++;
    }
}


void dlist_insert(DList *thiz, size_t idx, void *pdata)
{
    size_t i;
    struct _DListNode *new_node_p = NULL;
    struct _DListNode *node_of_idx_p = NULL;
    struct _DListNode *prev_node_of_idx_p = NULL;

    // verify idx(arg1) in [0..dlist_size]
    if (idx > thiz->size)
    {
        printf("idx %zu is out of range[0..%zu]\n", idx, thiz->size);
        return;
    }

    // make new node with data(arg2) and new_node_p
    new_node_p = (struct _DListNode *)malloc(sizeof(struct _DListNode));
    if (new_node_p == NULL)
    {
        perror("new_node_p malloc");
        exit(1);
    }

    new_node_p->pdata = pdata;
    new_node_p->next = NULL;
    new_node_p->prev = NULL;

    // get node_of_idx_p
    // get prev_node_of_idx_p
    if (thiz->size == 0)
    {
        node_of_idx_p = prev_node_of_idx_p = new_node_p;
    }
    else
    {
        node_of_idx_p = thiz->head;
        for (i = 0; i < idx; i++)
            node_of_idx_p = node_of_idx_p->next;
        prev_node_of_idx_p = node_of_idx_p->prev;
    }

    // link new node into dlist
    new_node_p->next = node_of_idx_p;
    new_node_p->prev = prev_node_of_idx_p;

    node_of_idx_p->prev = new_node_p;
    prev_node_of_idx_p->next = new_node_p;

    if (idx == 0)
    {
        thiz->head = new_node_p;
    }

    thiz->size++;
    
}

void dlist_delete(DList *thiz, size_t idx)
{
    size_t i;
    struct _DListNode *node_of_idx_p = NULL;
    struct _DListNode *next_node_of_idx_p = NULL;
    struct _DListNode *prev_node_of_idx_p = NULL;

    // verify dlist_size is not 0 
    if (thiz->size == 0)
    {
        printf("dlist is empty.\n");
        return;
    }

    // verify idx(arg1) in [0..dlist_size-1]
    if (idx >= thiz->size)
    {
        printf("idx %zu is out of range[0..%zu]\n", idx, (thiz->size - 1));
        return;
    }

    // get node_of_idx_p
    // get next_node_of_idx_p
    // get prev_node_of_idx_p
    node_of_idx_p = thiz->head;
    for (i = 0; i < idx; i++)
    {
        node_of_idx_p = node_of_idx_p->next;
    }
    next_node_of_idx_p = node_of_idx_p->next;
    prev_node_of_idx_p = node_of_idx_p->prev;

    if (idx == 0)
    {
        thiz->head = next_node_of_idx_p;
    }

    next_node_of_idx_p->prev = prev_node_of_idx_p;
    prev_node_of_idx_p->next = next_node_of_idx_p;

    thiz->size--;
    if (thiz->size == 0)
    {
        thiz->head = NULL;
    }

    free(node_of_idx_p->pdata);
    free(node_of_idx_p);
}
