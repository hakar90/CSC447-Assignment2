#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 100 
#define BUCKET_COUNT 10  
#define RANGE 100  

struct Node {
    int value;
    struct Node *next;
};

void sort(int arr[]);
struct Node *sortList(struct Node *list);
void display(int arr[]);
void displayBuckets(struct Node *list);
int getIndex(int value);

void sort(int arr[]) {
    int i, j;
    struct Node **buckets;
    buckets = (struct Node **)malloc(sizeof(struct Node *) * BUCKET_COUNT);
    for (i = 0; i < BUCKET_COUNT; ++i) {
        buckets[i] = NULL;
    }
    for (i = 0; i < SIZE; ++i) {
        struct Node *current;
        int pos = getIndex(arr[i]);
        current = (struct Node *)malloc(sizeof(struct Node));
        current->value = arr[i];
        current->next = buckets[pos];
        buckets[pos] = current;
    }
    #pragma omp parallel for shared(buckets)
    for (i = 0; i < BUCKET_COUNT; ++i) {
        buckets[i] = sortList(buckets[i]);
    }
    for (j = 0, i = 0; i < BUCKET_COUNT; ++i) {
        struct Node *node;
        node = buckets[i];
        while (node) {
            arr[j++] = node->value;
            node = node->next;
        }
    }
    for (i = 0; i < BUCKET_COUNT; ++i) {
        struct Node *node = buckets[i];
        while (node) {
            struct Node *temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(buckets);
}
struct Node *sortList(struct Node *list) {
    struct Node *k, *nodeList;
    if (list == 0 || list->next == 0) {
        return list;
    }
    nodeList = list;
    k = list->next;
    nodeList->next = 0;
    while (k != 0) {
        struct Node *ptr;
        if (nodeList->value > k->value) {
            struct Node *tmp;
            tmp = k;
            k = k->next;
            tmp->next = nodeList;
            nodeList = tmp;
            continue;
        }
        for (ptr = nodeList; ptr->next != 0; ptr = ptr->next) {
            if (ptr->next->value > k->value)
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
int getIndex(int value) {
    return value / RANGE;
}
void display(int ar[]) {
    int i;
    for (i = 0; i < SIZE; ++i) {
        printf("%d ", ar[i]);
    }
    printf("\n");
}
void displayBuckets(struct Node *list) {
    struct Node *cur = list;
    while (cur) {
        printf("%d ", cur->value);
        cur = cur->next;
    }
}
int main(void) {
    srand(time(NULL));
    int array[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        array[i] = rand() % 1000;
    }
    printf("Unsorted array: ");
    display(array);
    printf("-------------\n");
    clock_t start = clock(); 
    sort(array);
    clock_t end = clock(); 
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("-------------\n");
    printf("Array after sorting: ");
    display(array);
    printf("-------------\n");
    printf("Elapsed time: %f seconds\n", time_spent);
    return 0;
}
