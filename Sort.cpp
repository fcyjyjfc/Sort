#include "Sort.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::less;

int main()
{
    vector<int> ivec{234,26,67,34,124,457,87,352,987,3525,246,24354};
    InsertionSort(ivec.begin(), ivec.end(),
        [] (const int &a, const int &b) { return a < b; });
    cout << "升序排列：";
    for (const int i : ivec)
        cout << i << " ";
    cout << endl;

    InsertionSort(ivec.begin(), ivec.end(),
        [] (const int &a, const int &b) { return a > b; });
    cout << "降序排列：";
    for (const int i : ivec)
        cout << i << " ";
    cout << endl;

    ShellSort(ivec.begin(), ivec.end(),
        [] (const int &a, const int &b) { return a < b; });
    cout << "升序排列：";
    for (const int i : ivec)
        cout << i << " ";
    cout << endl;

    ShellSort(ivec.begin(), ivec.end(),
        [] (const int &a, const int &b) { return a > b; });
    cout << "降序排列：";
    for (const int i : ivec)
        cout << i << " ";
    cout << endl;

    HeapSort(ivec.begin(), ivec.end(),
        [] (const int &a, const int &b) { return a < b; });
    cout << "升序排列：";
    for (const int i : ivec)
        cout << i << " ";
    cout << endl;

    MergeSort(ivec.begin(), ivec.end(),
        [] (const int &a, const int &b) { return a > b; });
    cout << "降序排列：";
    for (const int i : ivec)
        cout << i << " ";
    cout << endl;

    QuickSort(ivec.begin(), ivec.end(),
        [] (const int &a, const int &b) { return a < b; });
    cout << "升序排列：";
    for (const int i : ivec)
        cout << i << " ";
    cout << endl;

    return 0;
}