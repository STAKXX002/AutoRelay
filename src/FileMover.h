#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <filesystem>

class FileMover {
public:
    static void moveFile(const std::filesystem::path& source, const std::filesystem::path& targetDirectory);
};

#endif // FILEMOVER_H