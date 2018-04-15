#include <assert.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"

static uint16_t values[256];

static void list_merge_sorted(struct list_head *list1,
                              struct list_head *list2,
                              struct list_head *list_sorted)
{
    struct listitem *item1 = NULL, *item2 = NULL;

    if (list_empty(list1)) {
        list_splice_tail(list2, list_sorted);
        return;
    }
    if (list_empty(list2)) {
        list_splice_tail(list1, list_sorted);
        return;
    }

    item1 = list_first_entry(list1, struct listitem, list);
    item2 = list_first_entry(list2, struct listitem, list);

    if (cmpint(&item1->i, &item2->i) < 0) {
        list_move_tail(&item1->list, list_sorted);
        list_merge_sorted(list1, list2, list_sorted);
    } else {
        list_move_tail(&item2->list, list_sorted);
        list_merge_sorted(list1, list2, list_sorted);
    }
}

static void __list_merge_sort(struct list_head *head,
                              long long int l,
                              long long int u)
{
    struct list_head list1, list2;
    struct listitem *item = NULL, *is = NULL;
    long long int i, mid;

    if (l >= u || list_empty(head) || list_is_singular(head))
        return;
    else {
        INIT_LIST_HEAD(&list1);
        INIT_LIST_HEAD(&list2);
        i = l;
        mid = (l + u) / 2;
        list_for_each_entry_safe (item, is, head, list) {
            if (i++ <= mid)
                list_move_tail(&item->list, &list1);
            else
                list_move_tail(&item->list, &list2);
        }
        __list_merge_sort(&list1, l, mid);
        __list_merge_sort(&list2, mid + 1, u);
        list_merge_sorted(&list1, &list2, head);
    }
}

static void list_merge_sort(struct list_head *head)
{
    struct listitem *item = NULL;
    long long int size = 0;

    list_for_each_entry (item, head, list)
        size++;
    __list_merge_sort(head, 0, size);
}
/*
int main(void)
{
    struct list_head testlist, result;
    struct listitem *item, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);
    INIT_LIST_HEAD(&result);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }
    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    list_merge_sort(&testlist);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}
*/