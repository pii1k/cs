#include "sorting_algorithms.hpp"

#include <cstddef>
#include <map>

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

std::vector<int> quickSort(const std::vector<int> &data)
{
    if (data.size() < 2)
        return data;

    std::vector<int> v = data;

    int first = v.front();
    size_t mid_idx = data.size() / 2;
    int mid = v[mid_idx];
    int end = v.back();

    std::vector<int> val_vec{first, mid, end};
    std::map<int, size_t> idx_val_map{{first, 0}, {mid, mid_idx}, {end, v.size() - 1}};

    auto getMedianIdx = [&]() -> size_t
    {
        bubbleSort(val_vec);
        return idx_val_map[val_vec[1]];
    };

    size_t pivot = getMedianIdx();

    std::vector<int> left{}, right{};
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i] < v[pivot])
            left.push_back(v[i]);
        else
            right.push_back(v[i]);
    }

    return {};
}
} // namespace SortingAlgorithms