#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "list.h"
#include "common.h"
#include "insert-sort.c"
#include "quick-sort.c"
#include "merge-sort.c"

double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec - t1.tv_nsec < 0) {
        diff.tv_sec = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

double get_sort_time(void (*sort)(struct list_head *),
                     int *value,
                     struct list_head *head,
                     int size)
{
    struct timespec start, end;
    double time;
    struct listitem *item = NULL, *is;
    int i = 0;

    list_for_each_entry_safe (item, is, head, list) {
        item->i = value[i++];
    }
    clock_gettime(CLOCK_REALTIME, &start);
    sort(head);
    clock_gettime(CLOCK_REALTIME, &end);
    time = diff_in_second(start, end);
    return time;
}

int myexp10(int y)
{
    int ret = 10, tmp;
    for (int i = 1; i < y; i++) {
        tmp = ret;
        ret <<= 3;
        tmp <<= 1;
        ret += tmp;
    }
    return ret;
}

int main()
{
    int *value, test_size = 4;
    double t1, t2, t3;
    struct list_head testlist;
    struct listitem *items;
    srand(time(NULL));
    printf("size\tinsert-sort\tquick-sort\tmerge-sort\n");

    for (int i = 1; i <= test_size; i++) {
        t1 = 0;
        t2 = 0;
        t3 = 0;
        for (int t = 0; t < 100; t++) {
            int array_size = myexp10(i);
            value = malloc(sizeof(int) * array_size);
            INIT_LIST_HEAD(&testlist);
            for (int j = 0; j < array_size; j++) {
                value[j] = rand() % 366 + 1;
                items = (struct listitem *) malloc(sizeof(*items));
                list_add_tail(&items->list, &testlist);
            }
            t1 += get_sort_time(list_insertsort, value, &testlist, array_size);
            t2 += get_sort_time(list_qsort, value, &testlist, array_size);
            t3 += get_sort_time(list_merge_sort, value, &testlist, array_size);

            free(value);
            free(items);
        }
        printf("10^%d\t", i);
        printf("%.10lf\t", t1 / 100);
        printf("%.10lf\t", t2 / 100);
        printf("%.10lf\n", t3 / 100);
    }
    return 0;
}
