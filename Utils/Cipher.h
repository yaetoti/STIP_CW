#pragma once
#include <vector>

std::vector<uint8_t> Encode(const std::vector<uint8_t>& data, uint64_t& key);
std::vector<uint8_t> Decode(const std::vector<uint8_t>& data, uint64_t key);
