#include <Windows.h>
#include <Console.h>
#include <Utils.h>
#include <Cipher.h>

constexpr const wchar_t* kInputFile = L"input.txt";
constexpr const wchar_t* kEncodedFile = L"encoded.txt";
constexpr const wchar_t* kDecodedFile = L"decoded.txt";

std::vector<uint8_t> BruteForceDecode(const std::vector<uint8_t>& encoded, const std::vector<uint8_t>& original, uint64_t& key) {
    std::vector<uint8_t> result;

    for (uint64_t possibleKey = 0; possibleKey < encoded.size() - 256; ++possibleKey) {
        auto table = encoded.begin() + possibleKey;
        bool match = true;

        // Check the range before the table
        for (size_t i = 0; i < possibleKey; ++i) {
            if (original[i] != static_cast<uint8_t>(std::find(table, table + 256, encoded[i]) - table)) {
                match = false;
                break;
            }
        }

        if (!match) {
            continue; 
        }

        // Check the range after the table
        for (size_t i = possibleKey + 256; i < encoded.size(); ++i) {
            if (original[i - 256] != static_cast<uint8_t>(std::find(table, table + 256, encoded[i]) - table)) {
                match = false;
                break;
            }
        }

        if (!match) {
            continue;
        }

        // Key found. Decode
        key = possibleKey;
        result = Decode(encoded, possibleKey);
    }

    return result;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	std::vector<uint8_t> data = ReadFile<uint8_t>(kEncodedFile);
	std::vector<uint8_t> original = ReadFile<uint8_t>(kInputFile);
    uint64_t key = 0;
    std::vector<uint8_t> decoded = BruteForceDecode(data, original, key);
    WriteFile(kDecodedFile, decoded);

    Console::GetInstance()->WPrintF(L"Decoded. Key: %llu.\n", key);
	Console::GetInstance()->Pause();
	return 0;
}
