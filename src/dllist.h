#ifndef dllist_h
#define dllist_h

typedef int (*dl_comparator)(void *, void *);
typedef int (*dl_print)(void *);
typedef int (*dl_freefunc)(void *);

typedef struct dlnode
{
    struct dlnode *next;
    struct dlnode *prev;
    void *data;
}dlnode;

typedef struct dllist
{
    dlnode *start;
    dlnode *end;
    dl_comparator comp;
    dl_print print;
    dl_freefunc free_func;
}dllist;

dllist  *dllist_new(dl_comparator, dl_print, dl_freefunc);
int     dllist_addtonow(dllist *list, dlnode *node);
int     dllist_isempty(dllist *);
void    dllist_print(dllist *list, dlnode *n);
int     dllist_free(dllist *list);
int     dllist_removefromlist(dllist *list, dlnode *node);
int     dllist_insertafter(dllist *list, dlnode *node, dlnode *after);

#endif
