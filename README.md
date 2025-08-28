# AutoRelay  

**GitHub Description:**  
Automated file organizer that classifies files by type and date into structured folders. Run the executable to clean up your directories.  

---

## Overview  

**AutoRelay** is a lightweight C++ application that automatically organizes your files **when you run it**.  
It scans a source directory (e.g., your **Downloads** folder), classifies files by their **type** (PDF, DOC, IMG, VIDEO, EXE, ZIP, etc.), and moves them into a target root directory. Inside the target, files are neatly arranged by **file type** and further sorted into **date-based subfolders (YYYY-MM)**.  

**Note:** AutoRelay is not a background service. You need to manually run the executable each time you want to organize files.  

---

## Features  

- Classifies files by type (PDF, DOC, IMG, VIDEO, EXE, ZIP, etc.)  
- Sorts into **year-month subfolders** based on last modified date  
- Prevents overwriting (renames duplicates automatically)  
- Simple one-shot execution — just run the exe to organize your files  
- Clean modular C++17 design:  
  - **FileWatcher** → Lists files in a directory  
  - **FileClassifier** → Determines file type & date subfolder  
  - **FileMover** → Moves files safely into the target folder  

---

## Build Instructions  

### Prerequisites  
- C++17 or newer  
- CMake (optional, for easier building)  

### Compile with g++  
```bash
g++ -std=c++17 -o AutoRelay src/*.cpp
```

### Run  
```bash
./AutoRelay
```

---

## Usage  

Currently, you must **set your source and target paths in the code** before compiling.  

In `main.cpp`:  
```cpp
int main() {
    // Change these paths as per your system
    std::string sourceDir = "C:\\Users\\KIIT\\Downloads"; // Source directory
    std::string targetRoot = "C:\\Dwn";                   // Target root directory
```

- Update `sourceDir` and `targetRoot`  
- Compile  
- Run the generated `AutoRelay.exe`  

Each time you run the executable, your files will be reorganized into structured folders.  

Example output structure:  
```
C:\Dwn\
   ├── PDF\
   │    └── 2025-07\
   │         ├── resume.pdf
   │         └── project.pdf
   ├── IMG\
   │    └── 2025-08\
   │         ├── photo.jpg
   │         └── screenshot.png
   └── VIDEO\
        └── 2025-06\
             └── lecture.mp4
```

---

## Project Structure  

```
AutoRelay/
 ├── src/
 │   ├── FileClassifier.cpp / .h
 │   ├── FileMover.cpp / .h
 │   ├── FileWatcher.cpp / .h
 │   └── main.cpp
 ├── .vscode/
 └── AutoRelay.exe   (built binary)
```

---

## Future Improvements  

- Accept **command-line arguments** (`AutoRelay.exe <sourceDir> <targetDir>`)  
- Config file support (set paths and mappings without editing code)  
- Continuous monitoring (true “auto-relay” behavior)  
- Cross-platform compatibility (Linux/macOS paths)  

---

## License  

This project is licensed under the MIT License.  
