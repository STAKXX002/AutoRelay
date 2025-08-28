#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <vector>
#include <string>
#include <filesystem>

class FileWatcher{
    public:
        static std::vector<std::filesystem::path> getFiles(const std::string& directory);
};

#endif // FILEWATCHER_H