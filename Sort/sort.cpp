#include "sort.h"
#include<vector>
#include <iostream>
#include <functional>
#include <cstring>
namespace templates {

    //----------------------MERGE-----------------------------------------
    void merge(void **arr, int left, int mid, int right, CompareSortType pCompareFunc, void **tmp) {
        int nLeft = mid - left + 1;
        int nRight = right - mid;
        void **left_p = tmp + left;
        void **right_p = tmp + mid + 1;

        std::memcpy(left_p, arr + left, nLeft * sizeof(void *));
        std::memcpy(right_p, arr + mid + 1, nRight * sizeof(void *));

        int i, j, k;
        for (i = 0, j = 0, k = left; i < nLeft && j < nRight; k++) {
            if (pCompareFunc(left_p[i], right_p[j]) <= 0) {
                arr[k] = left_p[i];
                i++;
            } else {
                arr[k] = right_p[j];
                j++;
            }
        }

        std::memcpy(arr + k, left_p + i, (nLeft - i) * sizeof(void *));
        k += (nLeft - i);
        std::memcpy(arr + k, right_p + j, (nRight - j) * sizeof(void *));

    }

    void mergeHelper(void **ppArray, int left, int right, CompareSortType pCompareFunc, void **tmp) {
        if (right <= left)
            return;

        int mid = (right + left) / 2;

        mergeHelper(ppArray, left, mid, pCompareFunc, tmp);
        mergeHelper(ppArray, mid + 1, right, pCompareFunc, tmp);
        merge(ppArray, left, mid, right, pCompareFunc, tmp);
    }

    void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc) {
        void **tmp = new void *[length];
        mergeHelper(ppArray, 0, length - 1, pCompareFunc, tmp);
        delete[] tmp;
    }

    //----------------------------------------------------------------------------
    //---------------------------HEAP-----------------------------------------------
    class BinaryHeap {
     private:
        std::function<int(const void *, const void *)> pCompareFunc;
        void **heap_;
        int size_;

        void heapify(int i) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < size_ && pCompareFunc(heap_[left], heap_[smallest]) < 0)
                smallest = left;

            if (right < size_ && pCompareFunc(heap_[right], heap_[smallest]) < 0)
                smallest = right;

            if (smallest != i) {
                std::swap(heap_[i], heap_[smallest]);
                heapify(smallest);
            }
        }

     public:

        BinaryHeap(std::function<int(const void *, const void *)> comp) : pCompareFunc(comp) {}

        void buildHeap(void **arr, int size) {
            size_ = size;
            heap_ = arr;
            for (int i = size / 2 - 1; i >= 0; i--)
                heapify(i);
        }

        void *extractMin() {
            void *min = heap_[0];
            heap_[0] = heap_[size_ - 1];
            --size_;
            heapify(0);
            return min;
        }
        void sort() {
            int arr_size = size_;
            for (int i = arr_size - 1; i >= 1; i--) {
                std::swap(heap_[0],heap_[i]);
                size_ = i;
                heapify(0);
            }
            for (int i = 0; i < arr_size / 2; i++) {
                std::swap(heap_[i],heap_[arr_size - i - 1]);
            }
        }
    };

    void heapSort(void **ppArray, int length, CompareSortType pCompareFunc) {
        BinaryHeap heap(pCompareFunc);
        heap.buildHeap(ppArray, length);
        heap.sort();
    }

    //----------------------------------------------------------
}