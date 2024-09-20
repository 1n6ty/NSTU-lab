#pragma once

#include <vector>

bool has_duplicates_slow(const std::vector<int> &data);
std::vector<int> get_duplicates_slow(const std::vector<int> &data);

bool has_duplicates_fast(const std::vector<int> &data);
std::vector<int> get_duplicates_fast(const std::vector<int> &data);
