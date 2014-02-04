#ifndef __DLIST_H__

#define __DLIST_H__

struct _DList;
typedef struct _DList DList;

enum _DListRet;
typedef enum _DListRet DListRet;

typedef void (*DListVisitFunc)(void *ctx, void *data);

DList *dlist_create(void);
void dlist_destroy(DList *thiz);

size_t dlist_size(DList *thiz);
void dlist_foreach(DList *thiz, DListVisitFunc data_func, void *ctx);
void dlist_insert(DList *thiz, size_t idx, void *data);
void dlist_delete(DList *thiz, size_t idx);

#endif // __DLIST_H__
