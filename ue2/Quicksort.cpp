#include "Quicksort.h"

std::vector<Point> Quicksort::quicksort(std::vector<Point> list)
{
    Point* p = list.data();
    sort(p, 0, list.size() - 1);
    return list;
}

void Quicksort::sort(Point* list, int l, int r)
{
    int i = l;
    int j = r;
    Point a;
    Point pivot = list[(l + r) / 2];
    while (i <= j) {
        while (list[i] < pivot)
            i++;
        while (list[j] > pivot)
            j--;
        if (i <= j) {
            a = list[i];
            list[i] = list[j];
            list[j] = a;
            i++;
            j--;
        }
    };
    if (l < j)
        sort(list, l, j);
    if (i < r)
        sort(list, i, r);
}