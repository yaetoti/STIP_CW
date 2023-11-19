#pragma once
#include <Console.h>
#include <chrono>
#include <string>

class ScopedTimer final {
public:
    explicit ScopedTimer(const std::wstring& message)
        : message(message) {}

    ~ScopedTimer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        Console::GetInstance()->WPrintF(L"%s took %lld ms.\n", message.c_str(), duration);
    }

private:
    std::wstring message;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
};
