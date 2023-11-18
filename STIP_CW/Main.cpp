#include <Windows.h>
#include <Console.h>
#include <Utils.h>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <random>

constexpr const wchar_t* kInputFile = L"input.txt";
constexpr const wchar_t* kEncodedFile = L"encoded.txt";
constexpr const wchar_t* kDecodedFile = L"decoded.txt";
constexpr const wchar_t* kKeyFile = L"key.txt";

// TODO: 3 analysis algorithms

template <typename Iterator, typename = std::enable_if_t<std::is_integral_v<typename std::iterator_traits<Iterator>::value_type>>>
void VecPrint(const wchar_t* format, Iterator begin, Iterator end) {
	for (Iterator it = begin; it != end; ++it) {
		Console::GetInstance()->WPrintF(format, *it);
	}

	Console::GetInstance()->WPrintF(L"\n");
}

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
		byte = static_cast<uint8_t>(std::distance(table, std::find(table, table + 256, byte)));
	});

	return result;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	// Load data & Handle
	std::vector<uint8_t> data = ReadFile<uint8_t>(kInputFile);
	uint64_t key = 0;
	std::vector<uint8_t> encoded = Encode(data, key);
	std::vector<uint8_t> decoded = Decode(encoded, key);

	// Save data
	WriteFile(kEncodedFile, encoded);
	WriteFile<uint64_t>(kKeyFile, { key });
	WriteFile(kDecodedFile, decoded);

	return 0;
}
