#include "FileWatcher.h"
#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

std::vector<fs::path> FileWatcher::getFiles(const std::string& directory){
    std::vector<fs::path> files;

    try{
        for(const auto& entry : fs::directory_iterator(directory)){
            files.push_back(entry.path());
        }
    } catch (const std::exception& e){
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }

    return files;
}

