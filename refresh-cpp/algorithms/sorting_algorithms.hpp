#pragma once

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
        if (!sorted) // no more steps left to sort
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
} // namespace SortingAlgorithms