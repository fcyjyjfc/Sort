#ifndef SORT_H
#define SORT_H

#include <iostream>
#include <functional>
#include <vector>

// 插入排序的实现
template <typename RAIter, typename Comp>
void InsertionSort(RAIter beg, RAIter end, Comp comp)
{
    for (RAIter it = beg + 1; it != end; ++it)
    {
        auto temp(*it);                               // 暂存it指向的值
        RAIter rit = it - 1;                          // it的前一个位置
        for (; rit >= beg && comp(temp, *rit); --rit) // 为temp找到合适的插入位置
            *(rit + 1) = *rit;
        *(rit + 1) = temp;
    }
}

// 希尔排序的实现
// 选择一个合适的增量，并不断减小增量直到为1
// 每次排序相当于对间隔为增量的元素进行插入排序
template <typename RAIter, typename Comp>
void ShellSort(RAIter beg, RAIter end, Comp comp)
{
    // 使用希尔增量增量进行
    for (int increment = (end - beg) / 2; increment > 0; increment /= 2)
    {
        // 希尔排序的内部也是插入排序，只是每次比较相距increment处的元素而非相邻
        for (RAIter it = beg + increment; it != end; ++it) // 从增量处开始
        {
            auto temp(*it); // 暂存值
            RAIter rit = it;
            for (; rit >= beg + increment; rit -= increment)
            {
                if (comp(temp, *(rit - increment))) // 若前面的值与后面的值为逆序
                    *rit = *(rit - increment);      // 将前面的值移到后面
                else
                    break; // 找到了temp的位置
            }
            *rit = temp; // 将temp放入合理位置
        }
    }
}

// 用于计算堆中节点的左孩子下标
#define LeftChild(cur, beg) ((beg) + (2) * ((cur) - (beg)) + (1))

// 堆排序的辅助函数，用于使输入序列的cur节点向下部分具有正确的堆序性
template <typename RAIter, typename Comp>
void PercolateDown(RAIter beg, RAIter end, RAIter cur, Comp comp)
{
    auto temp(*cur); // 保存当前的值
    RAIter child;
    for (; LeftChild(cur, beg) < end; cur = child)
    {
        child = LeftChild(cur, beg);
        if (child + 1 < end && comp(*(child + 1), *child))
            ++child; // 选择合适的孩子

        if (comp(*child, temp))
            *cur = *child; // 将cur与child交换
        else
            break;
    }
    *cur = temp;
}

#undef LeftChild // 不再使用该宏定义

// 堆排序的实现
template <typename RAIter, typename Comp>
void HeapSort(RAIter beg, RAIter end, Comp comp)
{ // 生成一个新的比较器，交换comp的2个参数顺序
    auto rcomp = bind(comp, std::placeholders::_2, std::placeholders::_1);

    for (RAIter it = beg + (end - beg) / 2 - 1; it >= beg; --it)
        PercolateDown(beg, end, it, rcomp); // 执行下滤将输入序列构造成堆

    for (RAIter it = end; it != beg; --it)
    {
        using std::swap;
        swap(*beg, *--end);                  // 交换堆顶元素和堆的最后一个元素
        PercolateDown(beg, end, beg, rcomp); // 为堆顶元素执行下滤
    }
}

// 归并左右2个已排序的序列，归并后的结果保存至原位置
template <typename RAIter, typename Comp>
void Merge(RAIter beg, RAIter end, RAIter rpos, std::vector<decltype(*beg + 0)> &vec, Comp comp)
{
    RAIter lit = beg, rit = rpos;
    int i(0);
    while (lit != rpos && rit != end)
    {
        if (comp(*lit, *rit))    // 比较
            vec[i++] = *(lit++); // 存放正确的值
        else
            vec[i++] = *(rit++);
    }

    while (lit != rpos) // 某个序列还有值，全部复制过去
        vec[i++] = *(lit++);
    while (rit != end)
        vec[i++] = *(rit++);

    for (int i = 0; i < end - beg; ++i)
        *(beg + i) = vec[i]; // 复制到原序列处
}

template <typename RAIter, typename Comp>
void MSort(RAIter beg, RAIter end, RAIter mid, std::vector<decltype(*beg + 0)> &vec, Comp comp)
{
    if (end - beg > 1) // 序列长度大于1
    {
        MSort(beg, mid, beg + (mid - beg) / 2, vec, comp); // 左侧排序
        MSort(mid, end, mid + (end - mid) / 2, vec, comp); // 右侧排序
        Merge(beg, end, mid, vec, comp); // 归并左右（各自已排序）部分
    }
}

// 归并排序的接口
template <typename RAIter, typename Comp>
void MergeSort(RAIter beg, RAIter end, Comp comp)
{
    std::vector<decltype(*beg + 0)> temp(end - beg);
    MSort(beg, end, beg + (end - beg) / 2, temp, comp);
}


// 快速排序的辅助函数（三数中值分割法）
// 求序列最左点、最右点、中间点的中值
template <typename RAIter, typename Comp>
auto Median3(RAIter beg, RAIter end, Comp comp) -> decltype(*beg + 0)
{
    RAIter mid = beg + (end - beg) / 2; // 取序列中间的点

    using std::swap;
    if (!comp(*beg, *mid))
        swap(*beg, *mid);
    if (!comp(*beg, *(end - 1)))
        swap(*beg, *(end - 1));
    if (!comp(*mid, *(end - 1)))
        swap(*mid, *(end - 1));
    // 使*beg < *mid < *(end - 1)

    swap(*mid, *(end - 2)); // 将中值交换到end - 2处
    return *(end - 2); // 返回中值
}

#define CutOff 5

template <typename RAIter, typename Comp>
void QuickSort(RAIter beg, RAIter end, Comp comp)
{
    if (beg + CutOff <= end)
    {
        auto mid_val = Median3(beg, end, comp); // 求三数中值
        RAIter left = beg, right = end - 2;
        using std::swap;
        while (true)
        {
            while (comp(*++left, mid_val)) // 左边的值小于mid_val
                ;
            while (!comp(*--right, mid_val)) // 右边的值大于中值mid_val
                ;

            if (left < right)
                swap(*left, *right); // 左右未交叉，将小于mid_val的值交换到左侧，大于mid_val的值交换到右侧
            else
                break;
        }
        swap(*left, *(end - 2)); // 交换中值与大于中值的最小值

        QuickSort(beg, left, comp); // 分别对左、右部分进行排序
        QuickSort(left + 1, end, comp);
    }
    else
        InsertionSort(beg, end, comp); // 序列长度小于5时，执行插入排序
}


#endif