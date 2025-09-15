#include "sorting_algorithms.hpp"

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
        int pivot_idx = using_median_pivot ? low + (high - low) / 2 : high;
        int partition_idx = lomutoPartition(v, low, high, pivot_idx);
        quickSortImpl(v, low, partition_idx - 1, pivot_type, using_median_pivot);
        quickSortImpl(v, partition_idx + 1, high, pivot_type, using_median_pivot);
        break;
    }
    case QuickSortPartitionType::HoarePartitionScheme:
    {
        int pivot_idx = using_median_pivot ? low + (high - low) / 2 : low;
        if (pivot_idx == high)
            return;

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
} // namespace SortingAlgorithms
