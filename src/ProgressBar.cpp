#include "ProgressBar.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdio>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

bool ProgressBar::isRendered = false;

namespace {
bool isInteractiveTerminal() {
    return ISATTY(FILENO(stdout)) != 0;
}
} // namespace

void ProgressBar::enableAnsiSupport() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return;
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void ProgressBar::update(std::size_t current, std::size_t total, const std::string& currentFile) {
    if (total == 0) return;

    if (!isInteractiveTerminal()) {
        // Log-friendly fallback: one sequential line, no cursor control.
        std::cout << "[" << current << "/" << total << "] " << currentFile << "\n";
        return;
    }

    constexpr int barWidth = 40;
    double fraction = std::min(1.0, static_cast<double>(current) / static_cast<double>(total));
    int filled = static_cast<int>(barWidth * fraction);

    if (isRendered) {
        std::cout << "\033[2A"; // move cursor up to the start of our last render
    }

    std::cout << "\r\033[K[";
    for (int i = 0; i < barWidth; ++i) {
        std::cout << (i < filled ? '=' : (i == filled ? '>' : ' '));
    }
    std::cout << "] " << std::setw(3) << static_cast<int>(fraction * 100)
               << "% (" << current << "/" << total << ")\n";

    std::cout << "\r\033[K  " << currentFile << "\n";
    std::cout.flush();

    isRendered = true;
}

void ProgressBar::clear() {
    if (!isRendered) return;
    if (isInteractiveTerminal()) {
        std::cout << "\033[2A\033[J"; // up two lines, erase to end of screen
        std::cout.flush();
    }
    isRendered = false;
}

void ProgressBar::finish() {
    isRendered = false;
}