#include <stdio.h>

void swap(int *a, int *b);

void heapify(int *arr, unsigned int len);
void sink(int *arr, unsigned int idx, unsigned int len);

int main() {
    int arr[] = {30, 10, 69, 2, 16, 8, 31, 22};
    int len = 8;

    heapify(arr, len);
    for (unsigned int idx = len - 1; idx > 0; idx -= 1) {
        swap(&arr[0], &arr[idx]);
        sink(arr, 0, idx);
    }

    for (unsigned int idx = 0; idx < len; idx += 1) {
        printf("%d ", arr[idx]);
    }
}

void swap(int *a, int *b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void heapify(int *arr, unsigned int len) {
    if (len == 0) {
        return;
    }
    unsigned int i = (len - 1) / 2;
    for (;;) {
        sink(arr, i, len);
        if (i == 0) {
            break;
        }
        i -= 1;
    }
}

void sink(int *arr, unsigned int idx, unsigned int len) {
    unsigned int hole = idx;
    int value = arr[idx];
    for (;;) {
        unsigned int left_idx = 2 * hole + 1;
        unsigned int right_idx = 2 * hole + 2;
        if (right_idx < len) {
            int left = arr[left_idx];
            int right = arr[right_idx];
            if (left > right && left > value) {
                arr[hole] = left;
                hole = left_idx;
            } else if (right > value) {
                arr[hole] = right;
                hole = right_idx;
            } else {
                break;
            }
        } else if (left_idx < len) {
            int left = arr[left_idx];
            if (left > value) {
                arr[hole] = left;
                hole = left_idx;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    arr[hole] = value;
}