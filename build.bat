@echo off


echo cleaning old build crap...
rmdir /s /q build 2>nul

echo configuring with clang...
cmake -B build -G "Ninja" ^
-DCMAKE_CXX_COMPILER="C:/Users/adria/Downloads/clangidfk/clang+llvm-22.1.0-x86_64-pc-windows-msvc/bin/clang++.exe" ^
-DCMAKE_C_COMPILER="C:/Users/adria/Downloads/clangidfk/clang+llvm-22.1.0-x86_64-pc-windows-msvc/bin/clang.exe" ^
-DCMAKE_BUILD_TYPE=RelWithDebInfo

if %errorlevel% neq 0 (
    echo cmake config failed you clown 💀
    pause
    exit /b
)

echo building with ninja clang beast...
cmake --build build --config RelWithDebInfo

if %errorlevel% neq 0 (
    echo build exploded wtf 😭
    pause
    exit /b
)

echo zipping new .geode...
geode package install .

echo packaged successfully, upgrading sdk back to latest...
geode sdk update

if %errorlevel% neq 0 (
    echo sdk update failed but the mod built fine so whatever 😭
    pause
    exit /b
)

echo done idiot, launch gd now 🙏
pause