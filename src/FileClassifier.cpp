#include "FileClassifier.h"
#include <map>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

std::string FileClassifier::getFileType(const fs::path& filePath) {
    std::string ext = filePath.extension().string();
    std::map<std::string, std::string> extensionMap = {
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

    // Normalize to lowercase
    for (auto& ch : ext) ch = std::tolower(ch);

    if (extensionMap.count(ext))
        return extensionMap[ext];
    else
        return "OTHER";
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