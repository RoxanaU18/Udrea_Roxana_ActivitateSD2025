#include <stdio.h>

#define MAX 100

int heap[MAX];
int heapSize = 0;

void swap(int* a, int* b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void inserareHeap(int val) {
    heap[heapSize] = val;
    int i = heapSize;
    heapSize++;

    // Heapify up
    while (i > 0 && heap[i] > heap[(i - 1) / 2]) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void afisareHeap() {
    printf("Heap: ");
    for (int i = 0; i < heapSize; i++) {
        printf("%d ", heap[i]);
    }
    printf("\n");
}

int main() {
    inserareHeap(10);
    inserareHeap(4);
    inserareHeap(15);
    inserareHeap(20);
    inserareHeap(2);
    inserareHeap(17);

    afisareHeap();

    return 0;
}
