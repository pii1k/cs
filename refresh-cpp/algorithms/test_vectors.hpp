#pragma once

#include <vector>

namespace test_vec
{
namespace sorting
{
const std::vector<int> vec_1{38, 27, 43, 3, 9, 82, 10};
const std::vector<int> vec_2{1, 2, 3, 4, 5, 6, 7};
const std::vector<int> vec_3{7, 6, 5, 4, 3, 2, 1};
const std::vector<int> vec_4{5, 3, 8, 3, 9, 1, 5, 3};
const std::vector<int> vec_5{42};
const std::vector<int> vec_6{};

const std::vector<int> vec_1_expect{3, 9, 10, 27, 38, 43, 82};
const std::vector<int> vec_2_expect{1, 2, 3, 4, 5, 6, 7};
const std::vector<int> vec_3_expect{1, 2, 3, 4, 5, 6, 7};
const std::vector<int> vec_4_expect{1, 3, 3, 3, 5, 5, 8, 9};
const std::vector<int> vec_5_expect{42};
const std::vector<int> vec_6_expect{};
} // namespace sorting
} // namespace test_vec