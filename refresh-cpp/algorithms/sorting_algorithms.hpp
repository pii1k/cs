#pragma once

#include <vector>

namespace SortingAlgorithms
{
enum class QuickSortPartitionType
{
    LomutoPartitionScheme,
    HoarePartitionScheme,
};

std::vector<int> bubbleSort(const std::vector<int> &data);
std::vector<int> selectionSort(const std::vector<int> &data);
std::vector<int> insertionSort(const std::vector<int> &data);
std::vector<int> quickSort(const std::vector<int> &data,
     QuickSortPartitionType pivot_type,
     bool using_median_pivot = false);
} // namespace SortingAlgorithms