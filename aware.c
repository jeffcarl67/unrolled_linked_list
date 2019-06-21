#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define CAPACITY 16

struct listitem {
  struct list_head list;
  int i[CAPACITY];
  int stored;
  int cap;
};

int insert_unrolled(struct list_head *list, int item);
int delete_unrolled(struct list_head *list, int index);
void print_unrolled(struct list_head *list);
void test_traverse(struct list_head *list, int num);

int main() {
  struct list_head list1;
  int i;
  for (i = 1; i < 1000000000; i *= 10) {
    test_traverse(&list1, i);
  }

  return 0;
}

void init_listitem(struct listitem *item) {
  if (!item)
    return;
  item->cap = CAPACITY;
  item->stored = 0;
}

int insert_unrolled(struct list_head *list, int item) {
  struct listitem *tmp = NULL;
  struct listitem *last = NULL;
  if (!list)
    return 0;
  last = list_entry(list->prev, struct listitem, list);
  if (list_empty(list) || last->stored >= (last->cap / 2 + 1)) {
    tmp = (struct listitem *)malloc(sizeof(struct listitem));
    if (!tmp) {
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

int delete_unrolled(struct list_head *list, int index) {
  struct listitem *entry = NULL;
  int i = 0;
  list_for_each_entry(entry, list, list) {
    if (i <= index && i + entry->stored > index) {
      memcpy(entry->i + index - i, entry->i + index - i + 1,
             sizeof(int) * (i + entry->stored - index) - 1);
      entry->stored--;
    }
    i += entry->stored;
  }

  return 0;
}

void print_unrolled(struct list_head *list) {
  struct listitem *entry = NULL;
  int i = 0;

  if (!list)
    return;
  list_for_each_entry(entry, list, list) {
    i = 0;
    while (i < entry->stored) {
      printf("%d ", entry->i[i]);
      i++;
    }
    printf("\n");
  }
}

void test_traverse(struct list_head *list, int num) {
  struct timeval start;
  struct timeval end;
  struct timeval result = {0, 0};
  struct listitem *entry, *next;
  int i;
  int ret;
  INIT_LIST_HEAD(list);
  for (i = 0; i < num; i++) {
    ret = insert_unrolled(list, i);
  }

  system("echo 3 > /proc/sys/vm/drop_caches");

  gettimeofday(&start, NULL);
  list_for_each_entry(entry, list, list) {
    i = 0;
    while (i < entry->stored) {
      i++;
    }
  }
  gettimeofday(&end, NULL);
  timersub(&end, &start, &result);
  printf("%lu %lu\n", result.tv_sec, result.tv_usec);

  list_for_each_entry_safe(entry, next, list, list) {
    list_del(&entry->list);
    free(entry);
  }
}