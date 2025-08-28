#include <iostream>
#include "FileWatcher.h"
#include "FileClassifier.h"
#include "FileMover.h"

int main(){
    std::string sourceDir = "C:\\Users\\KIIT\\Downloads";
    std::string targetRoot = "C:\\Dwn";

    std::vector<std::filesystem::path>files = FileWatcher::getFiles(sourceDir);

    for(const auto& file : files){
        if(std::filesystem::is_directory(file)) continue;

        std::string fileType = FileClassifier::getFileType(file);
        std::string datePath = FileClassifier::getDateSubfolder(file);
        std::filesystem::path targetDir = targetRoot + "\\" + fileType + "\\" + datePath;

        FileMover::moveFile(file, targetDir);
    }

    std::cout << "AutoRelay finished moving files." << std::endl;

    return 0;
}