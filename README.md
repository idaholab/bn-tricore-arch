# TriCore Architecture Plugin for Binary Ninja
A Binary Ninja architecture plugin providing support for the TriCore family of instructions.

## Overview
This architecture plugin provides support for the disassembling and lifting of TriCore instructions.

### Features
- Full disassembly support for the TriCore v1.3 and v1.6 ISAs
- Partial lifting to Binary Ninja's low level IL

## Installation
### Manual Build and Installation
#### Prerequisites
- Binary Ninja (minimum version: `5.1.8104`)
- CMake 3.15 or higher
- C++20 compatible compiler

#### Building from Source & Install
```bash
# Clone the binja API
git clone https://github.com/Vector35/binaryninja-api.git
cd binaryninja-api

# Set up CMake files
echo -e "\nadd_subdirectory(plugins)" >> CMakeLists.txt
echo -e "\nadd_subdirectory(tricore)" >> plugins/CMakeLists.txt

# Download TriCore architecture source
cd plugins
git clone https://github.com/idaholab/bn-tricore-arch.git
cd ..

# Build
cmake -DCMAKE_BUILD_TYPE=release -DHEADLESS=yes .
cmake --build . --target all -j

# Install
cp out/bin/libtricore.so ~/.binaryninja/plugins/libtricore.so
```

## Usage
### Opening Files
1. Open Binary Ninja and select "Open with Options..."
2. Under "Load Options", set your entry point offset and image base
3. Choose "tricore" from the "Platform" dropdown menu
4. Open the binary file and wait for auto-analysis to complete

> Note that you may need to manually define functions or customize your binary view for the file to load properly

## Development
### Building for Development
To build with debug symbols, follow the instructions above to build from source but change the build type to debug:
```bash
cmake -DCMAKE_BUILD_TYPE=debug -DHEADLESS=yes .
```

### Contributing
We are not accepting pull requests for this repository as Binary Ninja now provides native TriCore support.

## Limitations
While every effort was made to ensure the accuracy of this plugin, you may still encounter bugs such as:
- Improperly disassembled instructions
- Missing instructions
- Typos

## Resources
- [Binary Ninja API Documentation](https://api.binary.ninja/cpp/index.html)
- [TriCore 1.3 Instruction Set Reference](https://www.infineon.com/gated/tc-v131-corearchitecture-v--138_3ce9dbce-e1dd-40fe-b4b9-c37d141bb866)
- [TriCore 1.6 Instruction Set Reference](https://www.infineon.com/gated/tc1-6--architecture-vol2_4cbbe9ae-b82b-4f77-bbf2-f151ce0850a5)

## License
Licensed under MIT.

See [LICENSE](LICENSE) file for details.

## Credits
Please see the [NOTICE](NOTICE.txt) file for details.
