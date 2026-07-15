# AutoRelay  

## Overview  

**AutoRelay** is a lightweight C++ application that automatically organizes your files **when you run it**.  
It scans a source directory (e.g., your **Downloads** folder), classifies files by their **type** (PDF, DOC, IMG, VIDEO, EXE, ZIP, etc.), and moves them into a target root directory. Inside the target, files are neatly arranged by **file type** and further sorted into **date-based subfolders (YYYY-MM)**.  

**Note:** AutoRelay is not a background service. You need to manually run the executable each time you want to organize files.  

## Features  

- Classifies files by type (PDF, DOC, IMG, VIDEO, EXE, ZIP, etc.)  
- Sorts into **year-month subfolders** based on last modified date  
- Prevents overwriting (renames duplicates automatically)  
- Safe to run from inside the folder being organized -AutoRelay excludes its own executable  
- Supports **both command-line arguments and interactive mode**  
- Simple one-shot execution -just run the exe to organize your files  
- Clean modular C++17 design:  
  - **FileWatcher** → Lists files in a directory  
  - **FileClassifier** → Determines file type & date subfolder  
  - **FileMover** → Moves files safely into the target folder  
  - **PathInput** → Interactive path prompts & validation  

## Build Instructions  

### Prerequisites  
- C++17 or newer  
- CMake 3.15+ (recommended)  

### Build with CMake (recommended)
```bash
cmake -B build
cmake --build build
```
The binary will be at `build/AutoRelay` (or `build/AutoRelay.exe` on Windows). CMake generates the right build files for your platform -Unix Makefiles on Linux/macOS, or a Visual Studio solution on Windows -from the same `CMakeLists.txt`.

### Or compile directly with g++
```bash
g++ -std=c++17 -o AutoRelay src/*.cpp
```

## Usage

AutoRelay supports **two modes of operation**.

### 1️ Argument Mode (Power-user / Script friendly)

Provide source and target paths directly:

```bash
AutoRelay.exe <source_path> <target_path>
```

Example:

```bash
AutoRelay.exe "C:\Users\ABC\Downloads" "C:\Dwn"
```

> **Note (Windows):** If a path contains spaces, it must be enclosed in double quotes.

### 2️ Interactive Mode (Default)

If no arguments are provided, AutoRelay prompts you at runtime:

```bash
AutoRelay.exe
```

Example prompt:

```
Enter SOURCE directory path: C:\Users\ABC\Downloads
Enter TARGET directory path: C:\Dwn
```

Each time you run the executable, your files will be reorganized into structured folders.

## Example Output Structure

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

## Testing

A manual test harness lives in `tests/`. It copies a set of fixture files (covering every classification bucket) into a scratch sandbox, runs AutoRelay against it, and prints the resulting tree -useful for confirming behavior after any change without touching real files.

```bash
cd tests
./run_manual_test.sh              # fresh run; also verifies the self-move exclusion
./run_manual_test.sh --no-reset   # re-run without wiping the target, to test duplicate-name handling
```

`tests/sandbox/` is generated and gitignored -only `tests/fixtures/` and the script itself are tracked.

## Project Structure

```
AutoRelay/
 ├── CMakeLists.txt
 ├── src/
 │   ├── FileClassifier.cpp / .h
 │   ├── FileMover.cpp / .h
 │   ├── FileWatcher.cpp / .h
 │   ├── PathInput.cpp / .h
 │   └── main.cpp
 └── tests/
     ├── fixtures/
     └── run_manual_test.sh
```

## Future Improvements

* Config file support for the type→folder mapping
* Continuous monitoring (true “auto-relay” behavior)
* Automated unit tests (current harness in `tests/` is manual/integration-only)
* Packaged release binaries for Linux/macOS/Windows
* Unified CLI conventions across multiple tools

