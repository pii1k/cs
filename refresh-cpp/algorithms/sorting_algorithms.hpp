#pragma once

#include <vector>

namespace SortingAlgorithms
{
void bubbleSort(std::vector<int> &data)
{
    for (auto i = 0; i < data.size(); i++)
    {
        for (auto j = 0; j < data.size() - i - 1; j++)
        {
            auto &first = data[j];
            auto &second = data[j + 1];
            if (first > second)
            {
                std::swap(first, second);
            }
        }
    }
}

void selectionSort(std::vector<int> &data)
{
    for (auto i = 0; i < data.size(); i++)
    {
        for (auto j = i + 1; j < data.size(); j++)
        {
            if (data[j] < data[i])
            {
                std::swap(data[i], data[j]);
            }
        }
    }
}
} // namespace SortingAlgorithms