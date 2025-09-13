#include "gtest/gtest.h"

#include "searching_algorithms.hpp"
#include "sorting_algorithms.hpp"
#include <vector>

namespace
{

} // namespace

class TestAlgorithms : public testing::Test
{
};

TEST_F(TestAlgorithms, BinarySearch)
{
    std::vector<int> test_data{1, 2, 3, 4, 6, 17, 28, 39, 410, 1354, 1312, 3222};
    int target = 4;
    int result = SearchingAlgorithms::binarySearch(test_data, target);
    EXPECT_EQ(target, result);
};

TEST_F(TestAlgorithms, BubbleSort)
{
    std::vector<int> test_data{1, 3, 13, 7, 23, 2, 67, 8, 9};
    std::vector<int> expect_res{1, 2, 3, 7, 8, 9, 13, 23, 67};
    SortingAlgorithms::bubbleSort(test_data);
    EXPECT_EQ(test_data, expect_res);
};

TEST_F(TestAlgorithms, SelectionSort)
{
    std::vector<int> test_data{1, 3, 13, 7, 23, 2, 67, 8, 9};
    std::vector<int> expect_res{1, 2, 3, 7, 8, 9, 13, 23, 67};
    SortingAlgorithms::selectionSort(test_data);
    EXPECT_EQ(test_data, expect_res);
};