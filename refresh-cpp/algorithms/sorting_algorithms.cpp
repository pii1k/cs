#include "sorting_algorithms.hpp"
#include <algorithm>
#include <cstddef>
#include <vector>

namespace SortingAlgorithms
{
std::vector<int> bubbleSort(const std::vector<int> &data)
{
    if (data.size() < 2)
        return data;

    bool sorted;
    std::vector<int> v = data;
    for (size_t i = 0; i < v.size(); i++)
    {
        sorted = false;
        for (size_t j = 0; j < v.size() - i - 1; j++)
        {
            if (v[j] > v[j + 1])
            {
                std::swap(v[j], v[j + 1]);
                sorted = true;
            }
        }
        if (!sorted)
            break;
    }
    return v;
}

std::vector<int> selectionSort(const std::vector<int> &data)
{
    if (data.size() < 2)
        return data;

    std::vector<int> v = data;

    for (size_t i = 0; i < v.size(); i++)
    {
        size_t min_idx = i;
        for (size_t j = i + 1; j < v.size(); j++)
        {
            if (v[j] < v[min_idx])
                min_idx = j;
        }

        if (min_idx != i)
            std::swap(v[i], v[min_idx]);
    }
    return v;
}

std::vector<int> insertionSort(const std::vector<int> &data)
{
    if (data.size() < 2)
        return data;

    std::vector<int> v = data;

    for (size_t i = 1; i < v.size(); i++)
    {
        int key = v[i];
        size_t j = i;
        while (0 < j && key < v[j - 1])
        {
            v[j] = v[j - 1];
            j--;
        }
        v[j] = key;
    }

    return v;
}

int lomutoPartition(std::vector<int> &v, int low, int high, int pivot_idx)
{
    if (pivot_idx != high)
    {
        std::swap(v[pivot_idx], v[high]);
        pivot_idx = high;
    }

    int pivot = v[pivot_idx];
    int iter_idx = low;

    for (int k = low; k < high; k++)
    {
        if (v[k] < pivot)
        {
            std::swap(v[iter_idx], v[k]);
            iter_idx++;
        }
    }
    std::swap(v[iter_idx], v[pivot_idx]);
    return iter_idx;
}

int hoarePartition(std::vector<int> &v, int low, int high, int pivot_idx)
{
    int pivot = v[pivot_idx];
    int left = low - 1;
    int right = high + 1;

    while (true)
    {
        while (true)
        {
            left++;
            if (v[left] >= pivot)
                break;
        }

        while (true)
        {
            right--;
            if (v[right] <= pivot)
                break;
        }

        if (left >= right)
            return right;

        std::swap(v[left], v[right]);
    }
}

int medianOfThreeIndex(const std::vector<int> &v, int low, int high)
{
    int mid = low + (high - low) / 2;

    const int &a = v[low];
    const int &b = v[mid];
    const int &c = v[high];

    if ((a <= b && b <= c) || (c <= b && b <= a))
        return mid;
    if ((b <= a && a <= c) || (c <= a && a <= b))
        return low;
    return high;
}

void quickSortImpl(std::vector<int> &v,
                   int low,
                   int high,
                   QuickSortPartitionType pivot_type,
                   bool using_median_pivot)
{
    if (low >= high)
        return;

    switch (pivot_type)
    {
    case QuickSortPartitionType::LomutoPartitionScheme:
    {
        int pivot_idx = using_median_pivot ? medianOfThreeIndex(v, low, high) : high;
        int partition_idx = lomutoPartition(v, low, high, pivot_idx);
        quickSortImpl(v, low, partition_idx - 1, pivot_type, using_median_pivot);
        quickSortImpl(v, partition_idx + 1, high, pivot_type, using_median_pivot);
        break;
    }
    case QuickSortPartitionType::HoarePartitionScheme:
    {
        int pivot_idx = using_median_pivot ? medianOfThreeIndex(v, low, high) : low;
        int partition_idx = hoarePartition(v, low, high, pivot_idx);
        quickSortImpl(v, low, partition_idx, pivot_type, using_median_pivot);
        quickSortImpl(v, partition_idx + 1, high, pivot_type, using_median_pivot);
        break;
    }
    }
}

std::vector<int> quickSort(const std::vector<int> &data, QuickSortPartitionType pivot_type, bool using_median_pivot)
{
    if (data.size() < 2)
        return data;

    std::vector<int> v = data;
    quickSortImpl(v, 0, static_cast<int>(v.size()) - 1, pivot_type, using_median_pivot);
    return v;
}

std::vector<int> mergeVectors(const std::vector<int> &left_vec, const std::vector<int> &right_vec)
{
    std::vector<int> sorted;
    sorted.reserve(left_vec.size() + right_vec.size());

    size_t l = 0;
    size_t r = 0;
    while (l < left_vec.size() && r < right_vec.size())
    {
        if (left_vec[l] <= right_vec[r])
            sorted.push_back(left_vec[l++]);
        else
            sorted.push_back(right_vec[r++]);
    }

    while (l < left_vec.size())
        sorted.push_back(left_vec[l++]);
    while (r < right_vec.size())
        sorted.push_back(right_vec[r++]);

    return sorted;
}

std::vector<int> mergeSortTopDownImpl(const std::vector<int> &data, size_t left, size_t right)
{
    if (right - left <= 1)
        return std::vector<int>(data.begin() + static_cast<std::ptrdiff_t>(left),
                                data.begin() + static_cast<std::ptrdiff_t>(right));

    size_t mid = left + (right - left) / 2;
    auto left_vec = mergeSortTopDownImpl(data, left, mid);
    auto right_vec = mergeSortTopDownImpl(data, mid, right);
    return mergeVectors(left_vec, right_vec);
}

std::vector<int> mergeSortBottomUpImpl(const std::vector<int> &data)
{
    if (data.size() < 2)
        return data;

    std::vector<int> src = data;
    std::vector<int> dest(data.size());

    for (size_t width = 1; width < data.size(); width *= 2)
    {
        for (size_t i = 0; i < data.size(); i += 2 * width)
        {
            size_t left = i;
            size_t mid = std::min(i + width, data.size());
            size_t right = std::min(i + 2 * width, data.size());

            size_t l = left;
            size_t r = mid;
            size_t idx = left;

            while (l < mid && r < right)
            {
                if (src[l] <= src[r])
                    dest[idx++] = src[l++];
                else
                    dest[idx++] = src[r++];
            }

            while (l < mid)
                dest[idx++] = src[l++];
            while (r < right)
                dest[idx++] = src[r++];
        }

        std::swap(src, dest);
    }

    return src;
}

std::vector<int> mergeSort(const std::vector<int> &data, MergeType merge_type)
{
    if (data.size() < 2)
        return data;

    std::vector<int> sorted{};
    switch (merge_type)
    {
    case MergeType::TopDown:
        sorted = mergeSortTopDownImpl(data, 0, data.size());
        break;
    case MergeType::BottomUp:
        sorted = mergeSortBottomUpImpl(data);
        break;
    }

    return sorted;
}

void heapify(std::vector<int> &v, size_t root_idx, size_t heap_size)
{
    while (true)
    {
        size_t left_child_idx = 2 * root_idx + 1;
        if (left_child_idx >= heap_size)
            break;

        size_t right_child_idx = left_child_idx + 1;
        size_t larger_child_idx = left_child_idx;

        if (right_child_idx < heap_size && v[right_child_idx] > v[left_child_idx])
            larger_child_idx = right_child_idx;

        if (v[root_idx] >= v[larger_child_idx])
            break;

        std::swap(v[root_idx], v[larger_child_idx]);
        root_idx = larger_child_idx;
    }
}

void buildMaxHeap(std::vector<int> &v)
{
    if (v.size() < 2)
        return;

    // 0-based 인덱스에서 자식노드는 2*i+1, 2*i+2 이므로 마지막 내부노드: (n - 2) / 2
    int n = v.size();
    for (size_t i = n / 2; i-- > 0;)
        heapify(v, i, n);
}

std::vector<int> heapSort(const std::vector<int> &data)
{
    if (data.size() < 2)
        return data;

    std::vector<int> v = data;
    buildMaxHeap(v);

    for (size_t heap_size = v.size(); heap_size > 1; heap_size--)
    {
        std::swap(v[0], v[heap_size - 1]);
        heapify(v, 0, heap_size - 1);
    }

    return v;
}

} // namespace SortingAlgorithms
