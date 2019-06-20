#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "list.h"

struct listitem
{
    struct list_head list;
    int i;
};

int main()
{
    struct list_head list1;
    struct listitem *item = NULL;
    struct listitem *entry = NULL;
    struct timeval start;
    struct timeval end;
    struct timeval result = {0 ,0};
    int i;
    int ret;
    INIT_LIST_HEAD(&list1);
    for(i = 0; i < 100000000; i++) {
        item = (struct listitem *) malloc(sizeof(struct listitem));
        item->i = i;
        list_add_tail(&item->list, &list1);
    }
    gettimeofday(&start, NULL);
    list_for_each_entry(entry, &list1, list) {
        //printf("%d ", entry->i);
    }
    gettimeofday(&end, NULL);
    timersub(&end, &start, &result);
    printf("%lu %lu\n", result.tv_sec, result.tv_usec);
    return 0;
}