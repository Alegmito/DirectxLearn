Directx learn application
Requirements:
- clang;
- cmake;
- vcpkg;


in CMakePresets.json edit `CMAKE_TOOLCHAIN_FILE`:
```
    "installDir": "${sourceDir}/out/install/${presetName}",
    "cacheVariables": {
    "CMAKE_C_COMPILER": "clang.exe",
    "CMAKE_CXX_COMPILER": "clang.exe",
    "CMAKE_TOOLCHAIN_FILE": "YOUR PATH TO VCPKG FOLDER",
    "VCPKG_ROOT": "D:/Programms/vcpkg"
    },
    "condition": {
```
You can launch using CMakePresets
