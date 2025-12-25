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
- Supports **both command-line arguments and interactive mode**  
- Simple one-shot execution — just run the exe to organize your files  
- Clean modular C++17 design:  
  - **FileWatcher** → Lists files in a directory  
  - **FileClassifier** → Determines file type & date subfolder  
  - **FileMover** → Moves files safely into the target folder  
  - **PathInput** → Interactive path prompts & validation  

---

## Build Instructions  

### Prerequisites  
- C++17 or newer  
- CMake (optional)  

### Compile with g++  
```bash
g++ -std=c++17 -o AutoRelay src/*.cpp
````

---

## Usage

AutoRelay supports **two modes of operation**.

---

### 1️ Argument Mode (Power-user / Script friendly)

Provide source and target paths directly:

```bash
AutoRelay.exe <source_path> <target_path>
```

Example:

```bash
AutoRelay.exe "C:\Users\KIIT\Downloads" "C:\Dwn"
```

> **Note (Windows):** If a path contains spaces, it must be enclosed in double quotes.

---

### 2️ Interactive Mode (Default)

If no arguments are provided, AutoRelay prompts you at runtime:

```bash
AutoRelay.exe
```

Example prompt:

```
Enter SOURCE directory path: C:\Users\KIIT\Downloads
Enter TARGET directory path: C:\Dwn
```

---

Each time you run the executable, your files will be reorganized into structured folders.

---

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

---

## Project Structure

```
AutoRelay/
 ├── src/
 │   ├── FileClassifier.cpp / .h
 │   ├── FileMover.cpp / .h
 │   ├── FileWatcher.cpp / .h
 │   ├── PathInput.cpp / .h
 │   └── main.cpp
 ├── .vscode/
 └── AutoRelay.exe
```

---

## Future Improvements

* Config file support
* Continuous monitoring (true “auto-relay” behavior)
* Cross-platform compatibility (Linux/macOS paths)
* Unified CLI conventions across multiple tools

---

## License

This project is licensed under the MIT License.

```