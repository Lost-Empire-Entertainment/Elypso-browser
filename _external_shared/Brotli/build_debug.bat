@echo off

:: This batch file builds Brotli from source code using MSVC (cl.exe) for Debug.

:: Set the root folder as the location of this script
set "BROTLI_ROOT=%~dp0"
set "BUILD_DIR=%BROTLI_ROOT%build-debug"
set "INSTALL_DIR=%BROTLI_ROOT%install-debug"

:: Ensure Visual Studio environment is set up correctly
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" || (
    echo [ERROR] Failed to set up Visual Studio environment.
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

:: Configure Brotli with CMake
echo [INFO] Configuring Brotli with CMake...
cmake -G "Ninja" ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_C_COMPILER=cl ^
  -DBUILD_SHARED_LIBS=ON ^
  -DBROTLI_DISABLE_TESTS=ON ^
  -DCMAKE_C_FLAGS="/Od /Zi /EHsc /MDd" ^
  -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
  "%BROTLI_ROOT%" || (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

:: Build Brotli with Ninja
echo [INFO] Building Brotli...
ninja -j%NUMBER_OF_PROCESSORS% || (
    echo [ERROR] Build process failed.
    pause
    exit /b 1
)

:: Install Brotli
echo [INFO] Installing Brotli...
ninja install || (
    echo [ERROR] Install process failed.
    pause
    exit /b 1
)

:: Record end time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_END=%%a:%%b:%%c"

:: Success message
echo [SUCCESS] Brotli built and installed successfully.
echo -------------------------------------------------
echo Libraries and DLLs are located in: %INSTALL_DIR%\lib
echo Headers are located in: %INSTALL_DIR%\include
echo Build duration: %TIME_START% - %TIME_END%
echo -------------------------------------------------

pause
exit /b 0
