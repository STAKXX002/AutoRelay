#include "FileClassifier.h"
#include <map>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cctype>

namespace fs = std::filesystem;

std::string FileClassifier::getFileType(const fs::path& filePath) {
    static const std::map<std::string, std::string> extensionMap = {
        {".pdf", "PDF"},
        {".zip", "ZIP"},
        {".rar", "ZIP"},
        {".7z", "ZIP"},
        {".png", "IMG"},
        {".jpg", "IMG"},
        {".jpeg", "IMG"},
        {".gif", "IMG"},
        {".mp4", "VIDEO"},
        {".mkv", "VIDEO"},
        {".exe", "EXE"},
        {".txt", "DOC"},
        {".docx", "DOC"},
        {".pptx", "DOC"}
    };

    std::string ext = filePath.extension().string();

    // Normalize to lowercase. std::tolower's behavior is only defined for
    // values representable as unsigned char (or EOF) -- passing a plain
    // (possibly signed) char with the high bit set is undefined behavior,
    // so cast through unsigned char first.
    for (auto& ch : ext) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    auto it = extensionMap.find(ext);
    return (it != extensionMap.end()) ? it->second : "OTHER";
}

std::string FileClassifier::getDateSubfolder(const fs::path& filePath) {
    try {
        auto ftime = fs::last_write_time(filePath);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );

        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
        std::tm* tm = std::localtime(&cftime);

        std::ostringstream oss;
        oss << std::put_time(tm, "%Y-%m");  // e.g., "2025-07"
        return oss.str();
    } catch (...) {
        return "unknown-date";
    }
}