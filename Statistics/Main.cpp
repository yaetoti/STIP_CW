#define NOMINMAX
#include <Windows.h>
#include <Console.h>
#include <Utils.h>
#include <Cipher.h>
#include <ScopedTimer.h>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <array>
#include <map>

constexpr const wchar_t* kInputFile = L"input.txt";
constexpr const wchar_t* kAnalysisFile = L"analysis.txt"; // File for statistical analysis
constexpr const wchar_t* kEncodedFile = L"encoded.txt";
constexpr const wchar_t* kDecodedFile = L"decoded.txt";
constexpr const uint64_t kKey = 608; // Used for correctness checking

double CalculateCorrectness(const std::vector<uint8_t>& text1, const std::vector<uint8_t>& text2, size_t size) {
    uint64_t matches = 0;
    for (int i = 0; i < size; ++i) {
        if (text1[i] == text2[i]) {
            ++matches;
        }
    }

    return static_cast<double>(matches) / static_cast<double>(size);
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    std::vector<uint8_t> data = ReadFile<uint8_t>(kInputFile);
    std::array<std::vector<uint8_t>, 2> text = { ReadFile<uint8_t>(kEncodedFile), ReadFile<uint8_t>(kAnalysisFile) };
    std::array<std::map<uint8_t, uint64_t>, 2> probabilities;
    std::array<std::vector<std::pair<uint8_t, uint64_t>>, 2> sorted;

    {
        ScopedTimer timer(L"[INFO]: Decoding");
        // Calculate probabilities
        for (int i = 0; i < 2; ++i) {
            for (uint8_t byte : text[i]) {
                ++probabilities[i][byte];
            }

            sorted[i].assign(probabilities[i].begin(), probabilities[i].end());
            std::sort(sorted[i].begin(), sorted[i].end(), [](const auto& lhs, const auto& rhs) {
                return lhs.second > rhs.second;
            });
        }

        // Statistical substitution
        size_t symbolsCount = std::min(sorted[0].size(), sorted[1].size());
        std::unordered_map<uint8_t, uint8_t> substitution;
        for (size_t i = 0; i < symbolsCount; ++i) {
            substitution[sorted[0][i].first] = sorted[1][i].first;
        }

        for (auto iter = text[0].begin(); iter != text[0].end(); ++iter) {
            bool match = false;

            for (int i = 0; i < sorted[0].size() && i < sorted[1].size(); ++i) {
                if (*iter == sorted[0][i].first) {
                    *iter = substitution[*iter];
                    match = true;
                    break;
                }
            }

            if (!match) {
                *iter = '_';
            }
        }
    }

    // Write decoded file
    WriteFile(kDecodedFile, text[0]);

    // Print info
    Console::GetInstance()->WPrintF(L"[INFO]: Text 0:\n\n\n");
    for (auto curr : sorted[0]) {
        Console::GetInstance()->WPrintF(L"\'%c\' %03d: %llu\n", curr.first, curr.first, curr.second);
    }

    Console::GetInstance()->WPrintF(L"\n\n[INFO]: Text 1:\n\n\n");
    for (auto curr : sorted[0]) {
        Console::GetInstance()->WPrintF(L"\'%c\' %03d: %llu\n", curr.first, curr.first, curr.second);
    }

    Console::GetInstance()->WPrintF(L"\n\n[INFO]: Decoded:\n\n\n");
    VecPrint(L"%c", text[0].begin(), text[0].end());

    // Correctness checking
    std::copy(text[0].begin() + kKey + 256, text[0].end(), text[0].begin() + kKey); // Remove table
    Console::GetInstance()->WPrintF(L"\n\n[INFO]: Correctness: %f\n", CalculateCorrectness(text[0], data, data.size()));

    Console::GetInstance()->Pause();
    return 0;
}
