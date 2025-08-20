Directx learn application
Requirements:
- clang;
- cmake;
- Windows SDK (for directxk and directxMath support);

in CMakePresets.json edit `WINSDK_PATH`:
```
    "installDir": "${sourceDir}/out/install/${presetName}",
    "cacheVariables": {
        "CMAKE_C_COMPILER": "clang.exe",
        "CMAKE_CXX_COMPILER": "clang.exe",
        "WINSDK_PATH": "C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64"
    },
    "condition": {
```
You can launch using CMakePresets
