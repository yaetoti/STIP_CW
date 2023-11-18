#include "Cipher.h"
#include <array>
#include <algorithm>
#include <numeric>
#include <random>

std::vector<uint8_t> Encode(const std::vector<uint8_t>& data, uint64_t& key) {
	std::vector<uint8_t> result(data);
	auto random = std::mt19937_64(std::random_device()());

	// Create random ASCII table
	std::array<uint8_t, 256> table;
	std::iota(table.begin(), table.end(), 0);
	std::shuffle(table.begin(), table.end(), random);

	// Encode data
	std::for_each(result.begin(), result.end(), [&table](uint8_t& byte) {
		byte = table[byte];
		});

	// Insert table
	key = std::uniform_int_distribution<uint64_t>(0, result.size())(random);
	result.reserve(result.size() + 256);
	result.insert(result.begin() + key, table.begin(), table.end());

	return result;
}

std::vector<uint8_t> Decode(const std::vector<uint8_t>& data, uint64_t key) {
	if (data.size() < 256) {
		throw std::runtime_error("Wrong format.");
	}

	std::vector<uint8_t> result(data.size() - 256);
	auto table = data.begin() + key;

	// Copy data without table
	std::copy(data.begin(), table, result.begin());
	std::copy(table + 256, data.end(), result.begin() + key);

	// Decode data
	std::for_each(result.begin(), result.end(), [&table](uint8_t& byte) {
		byte = static_cast<uint8_t>(std::find(table, table + 256, byte) - table);
		});

	return result;
}