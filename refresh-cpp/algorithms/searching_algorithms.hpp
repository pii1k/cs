#pragma once

#include <vector>

namespace SearchingAlgorithms
{
int linearSearch(const std::vector<int> &data, int target);
int binarySearch(const std::vector<int> &data, int target);

template <class ForwardIt, class T>
ForwardIt lowerBound(ForwardIt first, ForwardIt last, const T &val);

template <class ForwardIt, class T>
ForwardIt upperBound(ForwardIt first, ForwardIt last, const T &val);
} // namespace SearchingAlgorithms