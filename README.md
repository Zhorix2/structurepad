# StructurePad

A base project template for building cross-platform applications with **PDCurses**, **CMake**, and **vcpkg**. Includes GitHub Actions CI/CD for automated compilation across Windows, macOS, and Linux.

## Features

- ✅ PDCurses integration for terminal UI
- ✅ CMake build system
- ✅ vcpkg package management
- ✅ GitHub Actions CI/CD pipeline
- ✅ Cross-platform support (Windows, macOS, Linux)
- ✅ Both Debug and Release builds

## Project Structure

```
structurepad/
├── CMakeLists.txt           # CMake configuration
├── vcpkg.json              # vcpkg manifest
├── src/
│   └── main.cpp            # Main application entry point
├── include/                # Header files directory
├── .github/workflows/
│   └── build.yml          # GitHub Actions workflow
├── README.md              # This file
└── .gitignore             # Git ignore rules
```

## Prerequisites

### Local Development

- **CMake** 3.20 or higher
- **Git**
- **vcpkg** (automatic via GitHub Actions)
- **C++17 compatible compiler**:
  - GCC 7+
  - Clang 5+
  - MSVC 2017+

### Build Requirements

1. Clone the repository:
```bash
git clone https://github.com/Zhorix2/structurepad.git
cd structurepad
```

2. Initialize vcpkg (first time only):
```bash
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh  # Linux/macOS
# or
./vcpkg/bootstrap-vcpkg.bat  # Windows
```

## Building Locally

### Linux/macOS

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
./structurepad
```

### Windows (MSVC)

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
Release\structurepad.exe
```

## GitHub Actions CI/CD

The project automatically builds on:
- **Ubuntu** (Linux)
- **Windows** (MSVC)
- **macOS** (Clang)

Both **Debug** and **Release** configurations are tested.

Workflows trigger on:
- Pushes to `main` and `develop` branches
- Pull requests to `main`

## Adding Dependencies

To add new dependencies via vcpkg:

1. Update `vcpkg.json`:
```json
{
  "dependencies": [
    "pdcurses",
    "your-new-package"
  ]
}
```

2. Update `CMakeLists.txt` if needed:
```cmake
find_package(your-package CONFIG REQUIRED)
target_link_libraries(structurepad PRIVATE your-package::your-package)
```

## Development

### Adding Header Files

Place your headers in the `include/` directory. They will automatically be available via:
```cpp
#include "your-header.h"
```

### Adding Source Files

Update `CMakeLists.txt` to include additional source files:
```cmake
add_executable(structurepad 
    src/main.cpp
    src/additional_file.cpp
)
```

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Resources

- [PDCurses Documentation](https://pdcurses.org/)
- [CMake Documentation](https://cmake.org/cmake/help/latest/)
- [vcpkg Documentation](https://github.com/Microsoft/vcpkg)
