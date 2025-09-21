#pragma once

#include <vector>

namespace SortingAlgorithms
{
enum class QuickSortPartitionType
{
    LomutoPartitionScheme,
    HoarePartitionScheme,
};

enum class MergeType
{
    TopDown,
    BottomUp,
};

std::vector<int> bubbleSort(const std::vector<int> &data);
std::vector<int> selectionSort(const std::vector<int> &data);
std::vector<int> insertionSort(const std::vector<int> &data);
std::vector<int> quickSort(const std::vector<int> &data, QuickSortPartitionType pivot_type, bool using_median_pivot = false);
std::vector<int> mergeSort(const std::vector<int> &data, MergeType merge_type);
std::vector<int> heapSort(const std::vector<int> &data);
} // namespace SortingAlgorithms