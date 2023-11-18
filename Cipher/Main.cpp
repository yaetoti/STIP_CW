#include <Windows.h>
#include <Console.h>
#include <Utils.h>
#include <Cipher.h>

constexpr const wchar_t* kInputFile = L"input.txt";
constexpr const wchar_t* kEncodedFile = L"encoded.txt";
constexpr const wchar_t* kDecodedFile = L"decoded.txt";
constexpr const wchar_t* kKeyFile = L"key.txt";

// TODO: Text statistics
// TODO: Usage of known text segment and its position

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
