#pragma once

#include <vector>

namespace SearchingAlgorithms
{
int linearSearch(const std::vector<int> &data, int target)
{
    for (const auto &i : data)
    {
        if (i == target)
        {
            return i;
        }
    }

    return -1;
};

// data should be sorted
int binarySearch(const std::vector<int> &data, int target)
{
    int left = 0;
    int right = data.size() - 1;

    while (left <= right)
    {
        int mid = (left + right) / 2;
        int mid_d = data[mid];

        if (target == mid_d)
        {
            return mid_d;
        }

        if (target < mid_d)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return -1;
}

template <class ForwardIt, class T>
ForwardIt lowerBound(ForwardIt first, ForwardIt last, const T &val)
{
    ForwardIt it;
    typename std::iterator_traits<ForwardIt>::difference_type count, step; // difference_type: 두 iter 사이의 거리를 나타냄
    count = std::distance(first, last);

    while (count > 0)
    {
        it = first;
        step = count / 2;
        std::advance(it, step); // advance: iter 이동 함수
    }

    if (*it < val)
    {
        first = ++it;
        count -= step + 1;
    }
    else
    {
        count = step;
    }

    return first;
}

template <class ForwardIt, class T>
ForwardIt upperBound(ForwardIt first, ForwardIt last, const T &val)
{
    ForwardIt it;
    typename std::iterator_traits<ForwardIt>::difference_type count, step;
    count = std::distance(first, last);

    while (count > 0)
    {
        it = first;
        step = count / 2;
        std::advance(it, step);

        if (!(val < *it))
        {
            first = ++it;
            count -= step + 1;
        }
        else
        {
            count = step;
        }
    }
    return first;
}
} // namespace SearchingAlgorithms