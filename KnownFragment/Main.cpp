#include <Windows.h>
#include <Console.h>
#include <Utils.h>
#include <Cipher.h>
#include <ScopedTimer.h>

constexpr const wchar_t* kInputFile = L"input.txt";
constexpr const wchar_t* kEncodedFile = L"encoded.txt";
constexpr const wchar_t* kDecodedFile = L"decoded.txt";

std::vector<uint8_t> DecodeWithKnownFragment(const std::vector<uint8_t>& encoded, const std::vector<uint8_t>& original, const std::vector<uint8_t>& knownFragment, size_t fragmentPosition, uint64_t& key) {
    ScopedTimer timer(L"Decoding");
    std::vector<uint8_t> result;

    for (uint64_t possibleKey = 0; possibleKey < encoded.size() - 256; ++possibleKey) {
        auto table = encoded.begin() + possibleKey;
        bool match = true;

        // Check table
        for (size_t i = 0; i < knownFragment.size(); ++i) {
            size_t index = fragmentPosition + i;
            if (index >= possibleKey && index < possibleKey + 256) {
                index += 256;
            }

            if (knownFragment[i] != static_cast<uint8_t>(std::find(table, table + 256, encoded[index]) - table)) {
                match = false;
                break;
            }
        }
        
        if (!match) {
            continue;
        }

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
    std::string _knownFragment = "Dear Karl";
    std::vector<uint8_t> knownFragment(_knownFragment.begin(), _knownFragment.end());
    size_t fragmentPosition = 86; // File uses \r\n
    uint64_t key = 0;
    std::vector<uint8_t> decoded = DecodeWithKnownFragment(data, original, knownFragment, fragmentPosition, key);
    WriteFile(kDecodedFile, decoded);

    Console::GetInstance()->WPrintF(L"Decoded. Key: %llu.\n", key);
    Console::GetInstance()->Pause();
    return 0;
}
