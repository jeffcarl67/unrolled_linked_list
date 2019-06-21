#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct listitem {
  struct list_head list;
  int i;
};

void test_traverse(struct list_head *list, int num);

int main() {
  struct list_head list1;
  int i;
  for (i = 0; i < 1000000; i += 1000) {
    test_traverse(&list1, i);
  }

  return 0;
}

void test_traverse(struct list_head *list, int num) {
  struct timeval start;
  struct timeval end;
  struct timeval result = {0, 0};
  struct listitem *entry, *next, *item;
  int i;
  int ret;
  INIT_LIST_HEAD(list);
  for (i = 0; i < num; i++) {
    item = (struct listitem *)malloc(sizeof(struct listitem));
    item->i = i;
    list_add_tail(&item->list, list);
  }

  system("echo 3 > /proc/sys/vm/drop_caches");

  gettimeofday(&start, NULL);
  list_for_each_entry(entry, list, list) {}
  gettimeofday(&end, NULL);
  timersub(&end, &start, &result);
  printf("%d %lu %lu\n", num, result.tv_sec, result.tv_usec);

  list_for_each_entry_safe(entry, next, list, list) {
    list_del(&entry->list);
    free(entry);
  }
}