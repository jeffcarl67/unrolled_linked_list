#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "list.h"

#define CAPICITY 16

struct listitem
{
    struct list_head list;
    int i[CAPICITY];
    int stored;
    int cap;
};

int insert_unrolled(struct list_head *list, int item);
int delete_unrolled(struct list_head *list, int index);
void print_unrolled(struct list_head *list);

int main()
{
    struct list_head list1;
    struct listitem *entry;
    struct timeval start;
    struct timeval end;
    struct timeval result = {0 ,0};
    int i;
    int ret;

    INIT_LIST_HEAD(&list1);
    for(i = 0; i < 100000000; i++) {
        ret = insert_unrolled(&list1, i);
    }
    //delete_unrolled(&list1, 50);
    //print_unrolled(&list1);
    gettimeofday(&start, NULL);
    list_for_each_entry(entry, &list1, list) {
        i = 0;
        while(i < entry->stored) {
            //printf("%d ", entry->i[i]);
            i++;
        }
        //printf("\n");
    }
    gettimeofday(&end, NULL);
    timersub(&end, &start, &result);
    printf("%lu %lu\n", result.tv_sec, result.tv_usec);
    return 0;
}

void init_listitem(struct listitem *item)
{
    if (! item)
        return;
    item->cap = CAPICITY;
    item->stored = 0;
}

int insert_unrolled(struct list_head *list, int item)
{
    struct listitem *tmp = NULL;
    struct listitem *last = NULL;
    if (! list)
        return 0;
    last = list_entry(list->prev, struct listitem, list);
    if (list_empty(list) || last->stored >= (last->cap / 2 + 1)) {
        tmp = (struct listitem*) malloc(sizeof(struct listitem));
        if(! tmp) {
            printf("malloc error\n");
            return -1;
        }
        init_listitem(tmp);
        list_add_tail(&tmp->list, list);
        last = list_entry(list->prev, struct listitem, list);
    }

    last->i[last->stored] = item;
    last->stored++;

    return 0;
}

int delete_unrolled(struct list_head *list, int index)
{
    struct listitem *entry = NULL;
    int i = 0;
    list_for_each_entry(entry, list, list) {
        if(i <= index && i + entry->stored > index) {
            memcpy(entry->i + index - i, entry->i + index - i + 1, sizeof(int) * (i + entry->stored - index) - 1);
            entry->stored--;
        }
        i += entry->stored;
    }

    return 0;
}

void print_unrolled(struct list_head *list)
{
    struct listitem *entry = NULL;
    int i = 0;

    if(! list)
        return;
    list_for_each_entry(entry, list, list) {
        i = 0;
        while(i < entry->stored) {
            printf("%d ", entry->i[i]);
            i++;
        }
        printf("\n");
    }
}