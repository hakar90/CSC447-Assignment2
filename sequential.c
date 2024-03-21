#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NARRAY 100
#define NBUCKET 10
#define INTERVAL 100

struct Node {
    int data;
    struct Node *next;
};

void BucketSort(int arr[]);
struct Node *InsertionSort(struct Node *list);
void print(int arr[]);
void printBuckets(struct Node *list);
int getBucketIndex(int value);

void BucketSort(int arr[]) {
    int i, j;
    struct Node **buckets;
    buckets = (struct Node **)malloc(sizeof(struct Node *) * NBUCKET);
    for (i = 0; i < NBUCKET; ++i) {
        buckets[i] = NULL;
    }
    printf("Elements in each bucket before sorting:\n");
    for (i = 0; i < NARRAY; ++i) {
        struct Node *current;
        int pos = getBucketIndex(arr[i]);
        current = (struct Node *)malloc(sizeof(struct Node));
        current->data = arr[i];
        current->next = buckets[pos];
        buckets[pos] = current;
    }
    for (i = 0; i < NBUCKET; ++i) {
        buckets[i] = InsertionSort(buckets[i]);
    }
    for (j = 0, i = 0; i < NBUCKET; ++i) {
        struct Node *node;
        node = buckets[i];
        while (node) {
            arr[j++] = node->data;
            node = node->next;
        }
    }
    for (i = 0; i < NBUCKET; ++i) {
        struct Node *node = buckets[i];
        while (node) {
            struct Node *temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(buckets);
}
struct Node *InsertionSort(struct Node *list) {
    struct Node *k, *nodeList;
    if (list == 0 || list->next == 0) {
        return list;
    }
    nodeList = list;
    k = list->next;
    nodeList->next = 0;
    while (k != 0) {
        struct Node *ptr;
        if (nodeList->data > k->data) {
            struct Node *tmp;
            tmp = k;
            k = k->next;
            tmp->next = nodeList;
            nodeList = tmp;
            continue;
        }

        for (ptr = nodeList; ptr->next != 0; ptr = ptr->next) {
            if (ptr->next->data > k->data)
                break;
        }
        if (ptr->next != 0) {
            struct Node *tmp;
            tmp = k;
            k = k->next;
            tmp->next = ptr->next;
            ptr->next = tmp;
            continue;
        } else {
            ptr->next = k;
            k = k->next;
            ptr->next->next = 0;
            continue;
        }
    }
    return nodeList;
}
int getBucketIndex(int value) {
    return value / INTERVAL;
}
void print(int ar[]) {
    int i;
    for (i = 0; i < NARRAY; ++i) {
        printf("%d ", ar[i]);
    }
    printf("\n");
}
int main(void) {
    srand(time(NULL));
    int array[NARRAY];
    for (int i = 0; i < NARRAY; ++i) {
        array[i] = rand() % 1000;
    }
    printf("Initial array: ");
    print(array);
    printf("-------------\n");
    clock_t start = clock();
    BucketSort(array);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("-------------\n");
    printf("Sorted array: ");
    print(array);
    printf("-------------\n");
    printf("Time taken: %f seconds\n", time_spent);
    return 0;
}
