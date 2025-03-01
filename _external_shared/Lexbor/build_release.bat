@echo off

:: This batch file builds Lexbor from source code using MSVC (cl.exe) for Release.

:: Set the root folder as the location of this script
set "LEXBOR_ROOT=%~dp0"
set "BUILD_DIR=%LEXBOR_ROOT%build-release"
set "INSTALL_DIR=%LEXBOR_ROOT%install-release"

:: Ensure Visual Studio environment is set up correctly
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" || (
    echo [ERROR] Failed to set up Visual Studio environment.
	pause
    exit /b 1
)

:: Record start time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_START=%%a:%%b:%%c"

:: Create the build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%" || (
    echo [ERROR] Failed to access build directory: %BUILD_DIR%
	pause
    exit /b 1
)

:: Configure Lexbor with CMake
echo [INFO] Configuring Lexbor with CMake...
cmake -G "Ninja" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_C_COMPILER=cl ^
  -DBUILD_SHARED_LIBS=ON ^
  -DLEXBOR_BUILD_STATIC=OFF ^
  -DLEXBOR_BUILD_SEPARATELY=OFF ^
  -DLEXBOR_BUILD_EXAMPLES=OFF ^
  -DLEXBOR_BUILD_TESTS=OFF ^
  -DLEXBOR_BUILD_TESTS_CPP=OFF ^
  -DLEXBOR_BUILD_UTILS=OFF ^
  -DLEXBOR_BUILD_WITH_ASAN=OFF ^
  -DLEXBOR_BUILD_WITH_FUZZER=OFF ^
  -DLEXBOR_WITH_PERF=OFF ^
  -DLEXBOR_MAKE_PACKAGES_FILES=OFF ^
  -DCMAKE_C_FLAGS="/O2 /GL /DNDEBUG /MD" ^
  -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
  "%LEXBOR_ROOT%" || (
    echo [ERROR] CMake configuration failed.
	pause
    exit /b 1
)

:: Build Lexbor with Ninja
echo [INFO] Building Lexbor...
ninja -j%NUMBER_OF_PROCESSORS% || (
    echo [ERROR] Build process failed.
	pause
    exit /b 1
)

:: Install Lexbor
echo [INFO] Installing Lexbor...
ninja install || (
    echo [ERROR] Install process failed.
	pause
    exit /b 1
)

:: Record end time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_END=%%a:%%b:%%c"

:: Success message
echo [SUCCESS] Lexbor built and installed successfully.
echo ---------------------------------------------
echo Static library: %INSTALL_DIR%\lib

echo Include headers: %INSTALL_DIR%\include

echo Build duration: %TIME_START% - %TIME_END%
echo ---------------------------------------------

pause
exit /b 0