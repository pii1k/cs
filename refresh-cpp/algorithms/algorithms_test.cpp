#include "searching_algorithms.hpp"
#include "sorting_algorithms.hpp"
#include "test_vectors.hpp"
#include "gtest/gtest.h"
#include <vector>

namespace search = SearchingAlgorithms;
namespace sort = SortingAlgorithms;

class TestAlgorithms : public testing::Test
{
};

TEST_F(TestAlgorithms, BinarySearch)
{
    std::vector<int> test_data{1, 2, 3, 4, 6, 17, 28, 39, 410, 1354, 1312, 3222};
    int target = 4;
    int result = search::binarySearch(test_data, target);
    ASSERT_EQ(target, result);
};

TEST_F(TestAlgorithms, BubbleSort)
{
    ASSERT_EQ(sort::bubbleSort(test_vec::sorting::vec_1), test_vec::sorting::vec_1_expect);
    ASSERT_EQ(sort::bubbleSort(test_vec::sorting::vec_2), test_vec::sorting::vec_2_expect);
    ASSERT_EQ(sort::bubbleSort(test_vec::sorting::vec_3), test_vec::sorting::vec_3_expect);
    ASSERT_EQ(sort::bubbleSort(test_vec::sorting::vec_4), test_vec::sorting::vec_4_expect);
    ASSERT_EQ(sort::bubbleSort(test_vec::sorting::vec_5), test_vec::sorting::vec_5_expect);
    ASSERT_EQ(sort::bubbleSort(test_vec::sorting::vec_6), test_vec::sorting::vec_6_expect);
};

TEST_F(TestAlgorithms, SelectionSort)
{
    ASSERT_EQ(sort::selectionSort(test_vec::sorting::vec_1), test_vec::sorting::vec_1_expect);
    ASSERT_EQ(sort::selectionSort(test_vec::sorting::vec_2), test_vec::sorting::vec_2_expect);
    ASSERT_EQ(sort::selectionSort(test_vec::sorting::vec_3), test_vec::sorting::vec_3_expect);
    ASSERT_EQ(sort::selectionSort(test_vec::sorting::vec_4), test_vec::sorting::vec_4_expect);
    ASSERT_EQ(sort::selectionSort(test_vec::sorting::vec_5), test_vec::sorting::vec_5_expect);
    ASSERT_EQ(sort::selectionSort(test_vec::sorting::vec_6), test_vec::sorting::vec_6_expect);
};

TEST_F(TestAlgorithms, InsertionSort)
{
    ASSERT_EQ(sort::insertionSort(test_vec::sorting::vec_1), test_vec::sorting::vec_1_expect);
    ASSERT_EQ(sort::insertionSort(test_vec::sorting::vec_2), test_vec::sorting::vec_2_expect);
    ASSERT_EQ(sort::insertionSort(test_vec::sorting::vec_3), test_vec::sorting::vec_3_expect);
    ASSERT_EQ(sort::insertionSort(test_vec::sorting::vec_4), test_vec::sorting::vec_4_expect);
    ASSERT_EQ(sort::insertionSort(test_vec::sorting::vec_5), test_vec::sorting::vec_5_expect);
    ASSERT_EQ(sort::insertionSort(test_vec::sorting::vec_6), test_vec::sorting::vec_6_expect);
};

TEST_F(TestAlgorithms, QuickSortLomutoPartition)
{
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_1, sort::QuickSortPartitionType::LomutoPartitionScheme), test_vec::sorting::vec_1_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_2, sort::QuickSortPartitionType::LomutoPartitionScheme), test_vec::sorting::vec_2_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_3, sort::QuickSortPartitionType::LomutoPartitionScheme), test_vec::sorting::vec_3_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_4, sort::QuickSortPartitionType::LomutoPartitionScheme), test_vec::sorting::vec_4_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_5, sort::QuickSortPartitionType::LomutoPartitionScheme), test_vec::sorting::vec_5_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_6, sort::QuickSortPartitionType::LomutoPartitionScheme), test_vec::sorting::vec_6_expect);
};

TEST_F(TestAlgorithms, QuickSortHoarePartition)
{
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_1, sort::QuickSortPartitionType::HoarePartitionScheme), test_vec::sorting::vec_1_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_2, sort::QuickSortPartitionType::HoarePartitionScheme), test_vec::sorting::vec_2_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_3, sort::QuickSortPartitionType::HoarePartitionScheme), test_vec::sorting::vec_3_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_4, sort::QuickSortPartitionType::HoarePartitionScheme), test_vec::sorting::vec_4_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_5, sort::QuickSortPartitionType::HoarePartitionScheme), test_vec::sorting::vec_5_expect);
    ASSERT_EQ(sort::quickSort(test_vec::sorting::vec_6, sort::QuickSortPartitionType::HoarePartitionScheme), test_vec::sorting::vec_6_expect);
};
